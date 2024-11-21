#include "OMXDecoder.h"
#include <Log.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/OMXCodec.h>

using namespace android;

OMXDecoder::OMXDecoder():
        source_(nullptr),
        client_(new OMXClient()),
        frameRate_(0),
        status_(-1){
}

void OMXDecoder::setNativeWindow(sp<ANativeWindow> &nativeWindow){
    nativeWindow_ = nativeWindow;
}

void OMXDecoder::setSource(OMXSource *source){
    source_ = source;
}

int32_t OMXDecoder::frameRate() const{
    return frameRate_;
}

status_t OMXDecoder::getStatus() const {
    return status_;
}

status_t OMXDecoder::init(){
    if (Log::isInfo()) Log_i("init decoder");

    status_ = client_->connect();
    if (Log::isInfo()) Log_i("client connect %d", status_);

    if (status_ == OK) {
        uint32_t flags = OMXCodec::kHardwareCodecsOnly;
        decoder_ = OMXCodec::Create(client_->interface(), source_->getFormat(), false, source_,
                                    nullptr, flags, nativeWindow_);
        status_ = decoder_->start();

        if (Log::isInfo()) Log_i("decoder start %d", status_);

    }
    return status_;
}

void OMXDecoder::stop(){
    if (Log::isInfo()) Log_i("stop decoder");
    status_t ret = decoder_->stop();
    if (Log::isDebug()) Log_d("stopped %d", ret);
    if (Log::isInfo()) Log_i("disconnect client");
    client_->disconnect();
}

OMXDecoder::~OMXDecoder(){
    if (Log::isDebug()) Log_d("destructor");

    if (Log::isDebug()) Log_d("clear mDecoder");
    decoder_.clear();
    if (Log::isDebug()) Log_d("all done");
}

status_t OMXDecoder::read(){
    MediaBuffer *videoBuffer;
    MediaSource::ReadOptions options;
    options.setLateBy(0);
    status_t ret = decoder_->read(&videoBuffer, &options);
    if (ret == OK) {
        if (videoBuffer->range_length() > 0) {
            // If video frame availabe, render it to mNativeWindow
            sp<MetaData> metaData = videoBuffer->meta_data();
            int64_t timeUs = 0;
            metaData->findInt64(kKeyTime, &timeUs);
            native_window_set_buffers_timestamp(nativeWindow_.get(), timeUs); //NATIVE_WINDOW_TIMESTAMP_AUTO
            ret = nativeWindow_->queueBuffer(nativeWindow_.get(), videoBuffer->graphicBuffer().get());
            if (ret == 0) {
                metaData->setInt32(kKeyRendered, 1);
            }
        }
        videoBuffer->release();
    }
    return ret;
}



