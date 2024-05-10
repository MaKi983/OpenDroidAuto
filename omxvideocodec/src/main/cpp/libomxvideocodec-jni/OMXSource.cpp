#include "OMXSource.h"
#include "Log.h"

#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/openmax/OMX_IVCommon.h>
#include <media/stagefright/openmax/OMX_Video.h>
#include <inttypes.h>
#include <sstream>

#define BUFFERS_SIZE 10

using namespace android;

// https://www.programmersought.com/article/87712558400/
// https://vec.io/posts/use-android-hardware-decoder-with-omxcodec-in-ndk
// https://stackoverflow.com/questions/9832503/android-include-native-stagefright-features-in-my-own-project
OMXSource::OMXSource(int width, int height, int fps, std::mutex& mutex):
        format_(NULL), mutex_(mutex), quitFlag_(false){

    bufferSize_ = (width * height * 3) / 2;

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
    format_->setInt32(kKeyMaxInputSize, bufferSize_);
    format_->setInt32(kKeyColorFormat, OMX_COLOR_FormatYUV420Planar);

    for (int i = 0; i < BUFFERS_SIZE; i++) {
        group_.add_buffer(new MediaBuffer(bufferSize_));
        writeBuffers_.push(new Packet(bufferSize_));
    }
}

OMXSource::~OMXSource() {
    if (Log::isDebug()) Log_d("destructor");

    while (!writeBuffers_.empty()){
        if (Log::isDebug()) Log_d("delete write buffers");
        Packet::Pointer p = writeBuffers_.front();
        writeBuffers_.pop();
        delete p;
    }

    while (!readBuffers_.empty()){
        if (Log::isDebug()) Log_d("delete read buffers");
        Packet* p = readBuffers_.front();
        readBuffers_.pop();
        delete p;
    }
}

void OMXSource::waitForReadBuffer(){
    std::unique_lock<std::mutex> codeclock(mutex_);
    while (readBuffers_.empty() && !quitFlag_) {
        auto t1 = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        cond_.wait_until(codeclock, t1);
    }
}

void OMXSource::waitForWriteBuffer(){
    std::unique_lock<std::mutex> codeclock(mutex_);
    while (writeBuffers_.empty() && !quitFlag_) {
        auto t1 = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        cond_.wait_until(codeclock, t1);
    }
}

Packet::Pointer OMXSource::getWriteBuffer(){
    waitForWriteBuffer();
    return nextWriteBuffer();
}

int OMXSource::queueReadBuffer(Packet* buffer){
    std::unique_lock<std::mutex> codeclock(mutex_);
    if (!quitFlag_) {
        readBuffers_.push(buffer);
        if (Log::isVerbose()) Log_v("readbuffers size: %d", readBuffers_.size());
        if (Log::isVerbose()) Log_v("writebuffer size: %d", writeBuffers_.size());
        codeclock.unlock();
        cond_.notify_one();
    }
    return OK;
}

sp<MetaData>  OMXSource::getFormat(){
    return format_;
}

Packet::Pointer OMXSource::nextReadBuffer(){
    std::unique_lock<std::mutex> codeclock(mutex_);

    Packet::Pointer buffer;
    buffer = readBuffers_.front();
    readBuffers_.pop();

    return buffer;
}

Packet::Pointer OMXSource::nextWriteBuffer(){
    std::unique_lock<std::mutex> codeclock(mutex_);

    Packet::Pointer buffer;
    buffer = writeBuffers_.front();
    writeBuffers_.pop();

    return buffer;
}

status_t OMXSource::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) {
    if (Log::isVerbose()) Log_v("read");
    waitForReadBuffer();

    if (Log::isVerbose()) Log_v("found buffer");
    if (!quitFlag_) {
        Packet* p = nextReadBuffer();

        status_t ret = group_.acquire_buffer(buffer);
        if (Log::isVerbose()) Log_v("buffer_ %p", p);

        if (ret == OK) {

            memcpy((*buffer)->data(), p->buf, p->len);

            (*buffer)->set_range(0, (size_t) p->len);
            (*buffer)->meta_data()->clear();
            (*buffer)->meta_data()->setInt32(kKeyIsSyncFrame, 1);
            (*buffer)->meta_data()->setInt64(kKeyTime, p->timestamp);

        }

        p->clear();
        writeBuffers_.push(p);

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
        std::unique_lock<std::mutex> codeclock(mutex_);
        quitFlag_ = true;
        if (Log::isVerbose()) Log_v("quit flag to true");
    }
    return OK;
}

