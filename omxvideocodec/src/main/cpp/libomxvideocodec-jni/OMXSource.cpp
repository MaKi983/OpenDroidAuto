#include "OMXSource.h"
#include "Log.h"

#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/openmax/OMX_IVCommon.h>
#include <media/stagefright/openmax/OMX_Video.h>
#include <inttypes.h>
#include <sstream>

#define BUFFERS_SIZE 5

using namespace android;

// https://www.programmersought.com/article/87712558400/
// https://vec.io/posts/use-android-hardware-decoder-with-omxcodec-in-ndk
// https://stackoverflow.com/questions/9832503/android-include-native-stagefright-features-in-my-own-project
OMXSource::OMXSource(int width, int height, int fps, std::mutex& mutex):
        format_(NULL), mutex_(mutex), quitFlag_(false) /*, bufferPool_(50, cChunkSize)*/
{

    size_t bufferSize = (width * height * 3) / 2;
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

    for (int i = 0; i < BUFFERS_SIZE; i++) {
        group_.add_buffer(new MediaBuffer(cChunkSize));
    }
}

OMXSource::~OMXSource() {
    if (Log::isDebug()) Log_d("destructor");
}

void OMXSource::waitForBuffer(){
    std::unique_lock<std::mutex> codeclock(mutex_);
    while (pbuffers_.empty() && !quitFlag_) {
        if (Log::isVerbose()) Log_v("buffers empty");
        auto t1 = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        cond_.wait_until(codeclock, t1);
    }
}

void OMXSource::queueBuffer(Packet::Pointer packet){
    std::unique_lock<std::mutex> codeclock(mutex_);
    if (!quitFlag_) {
        pbuffers_.push(packet);
        codeclock.unlock();
        cond_.notify_one();
    }
}

sp<MetaData>  OMXSource::getFormat(){
    return format_;
}

Packet::Pointer OMXSource::nextBuffer(){
    std::unique_lock<std::mutex> codeclock(mutex_);
    Packet::Pointer buffer = pbuffers_.front();
    pbuffers_.pop();
    return buffer;
}

status_t OMXSource::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) {
    if (Log::isVerbose()) Log_v("read");
    waitForBuffer();

    if (Log::isVerbose()) Log_v("found buffer");
    if (!quitFlag_) {
        Packet::Pointer p = nextBuffer();
        common::DataConstBuffer b(p->buffer);

        status_t ret = group_.acquire_buffer(buffer);

        if (ret == OK) {
            memcpy((*buffer)->data(), b.cdata, b.size);

            (*buffer)->set_range(0, (size_t) b.size);
            (*buffer)->meta_data()->clear();
            (*buffer)->meta_data()->setInt32(kKeyIsSyncFrame, 1);
            (*buffer)->meta_data()->setInt64(kKeyTime, p->timestamp);
//            (*buffer)->meta_data()->setInt64(kKeyTime, 0);

        }

        delete p;

        return ret;
    }

    if (Log::isDebug()) Log_d("quit flag set to true, stop");

    return -1;
}

status_t OMXSource::start(MetaData *params){
    if (Log::isInfo()) Log_i("start");
    return OK;
}

status_t OMXSource::stop() {
    if (!quitFlag_) {
        if (Log::isInfo()) Log_i("stop");
        {
            std::unique_lock<std::mutex> codeclock(mutex_);
            quitFlag_ = true;
        }
        if (Log::isVerbose()) Log_v("quit flag to true");
    }
    return OK;
}


