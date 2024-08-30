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
        format_(NULL), quitFlag_(false), pbuffers_(20)
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
        MediaBuffer* b = new MediaBuffer(cChunkSize);
//        MediaBuffer* b = new MediaBuffer(bufferSize);
        if (Log::isVerbose()) Log_v("creating mediabuffer %p", b);
        group_.add_buffer(b);
    }
}

OMXSource::~OMXSource() {
    if (Log::isDebug()) Log_d("destructor");
}

void OMXSource::queueBuffer(Packet::Pointer packet){
    if (!quitFlag_) {
        pbuffers_.push(packet);
        if (Log::isVerbose()) Log_v("queueBuffer new size %d", pbuffers_.was_size());
    }
}

sp<MetaData>  OMXSource::getFormat(){
    return format_;
}

Packet::Pointer OMXSource::nextBuffer(){
    if (Log::isVerbose()) Log_v("nextBuffer pop new buffer from pool");
    Packet::Pointer buffer = pbuffers_.pop();

    if (Log::isVerbose()) Log_v("nextBuffer new buffer isnull %s", (buffer == nullptr ? "true" : "false"));
//    if (buffer != nullptr) {
//        if (Log::isVerbose()) Log_v("nextBuffer new buffer -> size %d", buffer->size);
//        if (Log::isVerbose()) Log_v("nextBuffer new size %d", pbuffers_.was_size());
//    }

    return buffer;
}

status_t OMXSource::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) {
    if (Log::isVerbose()) Log_v("read");

    if (Log::isVerbose()) Log_v("found buffer");
    if (!quitFlag_) {
        Packet::Pointer p = nextBuffer();
        common::DataConstBuffer b(p->buffer);

        status_t ret = group_.acquire_buffer(buffer);
        if (Log::isVerbose()) Log_v("found MediaBuffer %p", *buffer);
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
        quitFlag_ = true;
        if (Log::isVerbose()) Log_v("quit flag to true");
        if (Log::isDebug()) Log_d("delete pbuffer: %d", pbuffers_.was_size());
        while (!pbuffers_.was_empty()) {
            Packet::Pointer buffer = pbuffers_.pop();
            delete buffer;
        }
        pbuffers_.push(new Packet(common::Data(0), 0, 0));
    }
    return OK;
}


