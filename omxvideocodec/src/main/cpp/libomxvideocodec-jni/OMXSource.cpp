#include "OMXSource.h"
#include "H264NalParser.h"
#include "Log.h"

#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/openmax/OMX_IVCommon.h>
#include <media/stagefright/openmax/OMX_Video.h>
#include <cinttypes>
#include <sstream>

using namespace android;

namespace {

constexpr std::size_t kMaxQueuedBuffers = 8;
constexpr int64_t kMaxQueuedDurationUs = 250000;
constexpr std::size_t kMaxSyncWaitBuffers = 8;
constexpr auto kMaxSyncWait = std::chrono::milliseconds(250);

} // namespace

// https://www.programmersought.com/article/87712558400/
// https://vec.io/posts/use-android-hardware-decoder-with-omxcodec-in-ndk
// https://stackoverflow.com/questions/9832503/android-include-native-stagefright-features-in-my-own-project
OMXSource::OMXSource(int width, int height, int fps):
        format_(nullptr), quitFlag_(false), waitingForSync_(false), droppedBuffers_(0),
        syncWaitDroppedBuffers_(0)
{

    int32_t bufferSize = (width * height * 3) / 2;
    if (Log::isVerbose()) Log_v("BufferSize: %d", bufferSize);

    format_ = new MetaData();
    format_->setInt32(kKeyWidth, width);
    format_->setInt32(kKeyHeight, height);
    format_->setInt32(kKeyStride, width);
    format_->setInt32(kKeySliceHeight, height);
    format_->setInt32(kKeyFrameRate, fps);
//    format_->setInt32(kKeyIFramesInterval, 1);
    format_->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
    format_->setInt32(kKeyVideoLevel, OMX_VIDEO_AVCLevel31);
    format_->setInt32(kKeyVideoProfile, OMX_VIDEO_AVCProfileBaseline);
    format_->setInt32(kKeyMaxInputSize, bufferSize);
    format_->setInt32(kKeyColorFormat, OMX_COLOR_FormatYUV420Planar);
}

OMXSource::~OMXSource() {
    if (Log::isDebug()) Log_d("destructor");
}

void OMXSource::queueBuffer(MediaBuffer* buffer){
    if (Log::isVerbose()) Log_v("add buffer to queue");
    std::unique_lock<std::mutex> l(mutex_);

    if (quitFlag_) {
        buffer->release();
        return;
    }

    const auto* bufferData = static_cast<const uint8_t*>(buffer->data());
    const auto* data = bufferData == nullptr ? nullptr : bufferData + buffer->range_offset();
    const h264::NalInfo nalInfo = h264::inspectNalUnits(data, buffer->range_length());
    int64_t timestampUs = 0;
    int64_t oldestTimestampUs = 0;
    buffer->meta_data()->findInt64(kKeyTime, &timestampUs);

    bool durationExceeded = false;
    if (!pbuffers_.empty() && timestampUs > 0 &&
        pbuffers_.front()->meta_data()->findInt64(kKeyTime, &oldestTimestampUs) &&
        oldestTimestampUs > 0 && timestampUs > oldestTimestampUs) {
        durationExceeded = timestampUs - oldestTimestampUs > kMaxQueuedDurationUs;
    }

    if (!waitingForSync_ &&
        (pbuffers_.size() >= kMaxQueuedBuffers || durationExceeded)) {
        droppedBuffers_ += pbuffers_.size();
        clearQueuedBuffers();
        waitingForSync_ = true;
        syncWaitDroppedBuffers_ = 0;
        syncWaitStarted_ = std::chrono::steady_clock::now();
        if (Log::isWarn()) {
            Log_w("video decoder backlog exceeded %lld us; briefly waiting for an IDR frame",
                  static_cast<long long>(kMaxQueuedDurationUs));
        }
    }

    if (waitingForSync_ && !nalInfo.hasCodecConfig && !nalInfo.hasIdr) {
        const bool waitExpired = syncWaitDroppedBuffers_ >= kMaxSyncWaitBuffers ||
            std::chrono::steady_clock::now() - syncWaitStarted_ >= kMaxSyncWait;
        if (!waitExpired) {
            ++droppedBuffers_;
            ++syncWaitDroppedBuffers_;
            buffer->release();
            return;
        }

        // Some transitions do not emit an IDR promptly. The decoder still owns its
        // previous reference frames, so resume feeding instead of freezing forever.
        waitingForSync_ = false;
        syncWaitDroppedBuffers_ = 0;
        if (Log::isWarn()) {
            Log_w("video decoder IDR wait expired after dropping %d buffers; resuming stream",
                  static_cast<int>(droppedBuffers_));
        }
        droppedBuffers_ = 0;
        buffer->meta_data()->setInt32(kKeyIsSyncFrame, 1);
    } else {
        buffer->meta_data()->setInt32(kKeyIsSyncFrame, nalInfo.hasIdr ? 1 : 0);
    }

    pbuffers_.push(buffer);

    if (nalInfo.hasIdr && waitingForSync_) {
        waitingForSync_ = false;
        syncWaitDroppedBuffers_ = 0;
        if (Log::isWarn()) {
            Log_w("video decoder resynchronized after dropping %d buffers",
                  static_cast<int>(droppedBuffers_));
        }
        droppedBuffers_ = 0;
    }

    if (Log::isVerbose()) Log_v("queueBuffer new size %d", pbuffers_.size());
    cond_.notify_one();
}

sp<MetaData>  OMXSource::getFormat(){
    return format_;
}

MediaBuffer* OMXSource::nextBuffer(){
    if (Log::isVerbose()) Log_v("nextBuffer");
    std::unique_lock<std::mutex> l(mutex_);

    while (!quitFlag_) {
        if (pbuffers_.empty()){
            cond_.wait_until(l, std::chrono::steady_clock::now() + std::chrono::milliseconds(1000));
            continue;
        }

        if (Log::isVerbose()) Log_v("buffer size %d", pbuffers_.size());
        MediaBuffer *buffer = pbuffers_.front();
        pbuffers_.pop();
        if (Log::isVerbose()) Log_v("found buffer %d", pbuffers_.size());
        return buffer;
    }

    return nullptr;
}

status_t OMXSource::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) {
    if (Log::isVerbose()) Log_v("read");

    MediaBuffer* mBuffer = nextBuffer();
    if (!mBuffer){
        return ERROR_END_OF_STREAM;
    }

    mBuffer->setObserver(this);
    mBuffer->add_ref();
    (*buffer) = mBuffer;

    return OK;

}

void OMXSource::signalBufferReturned(MediaBuffer *buffer) {
    if (Log::isVerbose()) Log_v("signalBufferReturned: %p", buffer);
    buffer->setObserver(nullptr);
    buffer->release();
}

status_t OMXSource::start(MetaData *params){
    if (Log::isInfo()) Log_i("start");
    return OK;
}

status_t OMXSource::stop() {
    if (Log::isDebug()) Log_d("stopping");
    std::unique_lock<std::mutex> l(mutex_);

    if (!quitFlag_) {
        if (Log::isInfo()) Log_i("stop");
        quitFlag_ = true;
        if (Log::isVerbose()) Log_v("quit flag to true");
        if (Log::isDebug()) Log_d("delete pbuffer: %d", pbuffers_.size());
        clearQueuedBuffers();
        cond_.notify_one();
    }
    if (Log::isDebug()) Log_d("stopped");
    return OK;
}

void OMXSource::clearQueuedBuffers() {
    while (!pbuffers_.empty()) {
        MediaBuffer* buffer = pbuffers_.front();
        pbuffers_.pop();
        buffer->release();
    }
}
