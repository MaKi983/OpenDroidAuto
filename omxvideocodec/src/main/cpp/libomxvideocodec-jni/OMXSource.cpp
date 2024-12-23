#include "OMXSource.h"
#include "Log.h"

#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/openmax/OMX_IVCommon.h>
#include <media/stagefright/openmax/OMX_Video.h>
#include <cinttypes>
#include <sstream>

#define BUFFERS_SIZE 5

using namespace android;

// https://www.programmersought.com/article/87712558400/
// https://vec.io/posts/use-android-hardware-decoder-with-omxcodec-in-ndk
// https://stackoverflow.com/questions/9832503/android-include-native-stagefright-features-in-my-own-project
OMXSource::OMXSource(int width, int height, int fps, std::mutex& mutex):
        format_(nullptr), quitFlag_(false), mutex_(mutex)
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
    std::unique_lock<std::mutex> l(mutex_);
    if (!quitFlag_) {
        pbuffers_.push(buffer);
        if (Log::isVerbose()) Log_v("queueBuffer new size %d", pbuffers_.size());
        cond_.notify_one();
    }
}

sp<MetaData>  OMXSource::getFormat(){
    return format_;
}

//MediaBuffer* OMXSource::nextBuffer(){
//    if (Log::isVerbose()) Log_v("nextBuffer pop new buffer from pool");
//    MediaBuffer* buffer = pbuffers_.front();
//    pbuffers_.pop();
//
//    return buffer;
//}

status_t OMXSource::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) {
    std::unique_lock<std::mutex> l(mutex_);
    if (Log::isVerbose()) Log_v("read");

    while (!quitFlag_ && pbuffers_.empty()) {
        cond_.wait_until(l, std::chrono::steady_clock::now() + std::chrono::milliseconds(100));
    }

    if (!quitFlag_) {
        if (Log::isVerbose()) Log_v("found buffer");
        MediaBuffer* mBuffer = pbuffers_.front();
        pbuffers_.pop();
        mBuffer->setObserver(this);
        mBuffer->add_ref();
        mBuffer->meta_data()->setInt32(kKeyIsSyncFrame, 1);
        (*buffer) = mBuffer;

        return OK;
    }

    if (Log::isDebug()) Log_d("quit flag set to true, stop");

    return ERROR_END_OF_STREAM;
}

void OMXSource::signalBufferReturned(MediaBuffer *buffer) {
    if (Log::isVerbose()) Log_v("signalBufferReturned: %p", buffer);
    std::unique_lock<std::mutex> l(mutex_);
    buffer->setObserver(nullptr);
    buffer->release();
}

status_t OMXSource::start(MetaData *params){
    if (Log::isInfo()) Log_i("start");
    return OK;
}

status_t OMXSource::stop() {
    std::unique_lock<std::mutex> l(mutex_);
    if (!quitFlag_) {
        if (Log::isInfo()) Log_i("stop");
        quitFlag_ = true;
        if (Log::isVerbose()) Log_v("quit flag to true");
        if (Log::isDebug()) Log_d("delete pbuffer: %d", pbuffers_.size());
        while (!pbuffers_.empty()) {
            MediaBuffer* buffer = pbuffers_.front();
            pbuffers_.pop();
            buffer->release();
        }
    }
    return OK;
}


