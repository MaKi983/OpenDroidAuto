#include "OMXVideoCodec.h"
#include <Log.h>

#include <unistd.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <cinttypes>
#include <chrono>
#include <sstream>
#include <iomanip>

OMXVideoCodec::OMXVideoCodec()
    : omxSource_(nullptr)
    , omxDecoder_(nullptr)
    , fps_(0)
    , decoderThreadInitialized_(false)
    , decoderThreadQuitFlag_(false){
}

OMXVideoCodec::~OMXVideoCodec() {
    if (Log::isVerbose()) Log_v("destructor");

    if (Log::isDebug()) Log_d("delete decoder");
    delete omxDecoder_;
    omxDecoder_ = nullptr;

    omxSource_ = nullptr;
}

status_t OMXVideoCodec::init(int fps){
    fps_ = fps;
    if (Log::isInfo()) Log_i("FPS %d", fps_);
    // Create decoder thread and wait until ready
    if (Log::isInfo()) Log_i("start omxDecoderThread");

    omxDecoderThread_ = std::thread(&OMXVideoCodec::init_omxDecoderThread, this);

    std::unique_lock<std::mutex> codeclock(codecMutex_);
    while (!decoderThreadInitialized_){
        omxDecoderCond_.wait(codeclock);
    }
    codeclock.unlock();
    if (Log::isInfo()) Log_i("omxDecoderThread ready");

    return omxDecoder_->getStatus();
}

void OMXVideoCodec::shutdown(){
    if (decoderThreadInitialized_) {
        if (Log::isInfo()) Log_i("Waiting for app thread to finish...");

        if (Log::isDebug()) Log_d("Stopping omxDecoderThread");
        decoderThreadQuitFlag_ = true;

        if (Log::isDebug()) Log_d("Stop omxSource");
        omxSource_->stop();

        if (omxDecoderThread_.joinable()){
            if (Log::isDebug()) Log_d("joining decoder thread");
            omxDecoderThread_.join();
        }

        if (Log::isDebug()) Log_d("release native window");
        ANativeWindow_release(nativeWindow_.get());
    }

    if (Log::isInfo()) Log_i("all done");
}

void OMXVideoCodec::init_omxDecoderThread() {
    if (Log::isInfo()) Log_i("omxDecoderThread begins");

    omxSource_ = new OMXSource(screenSize_.width, screenSize_.height, fps_);

    if (Log::isDebug()) Log_d("Created OMXSource %p", omxSource_);

    omxDecoder_ = new OMXDecoder();
    omxDecoder_->setNativeWindow(nativeWindow_);
    omxDecoder_->setSource(omxSource_);
    omxDecoder_->init();

    std::unique_lock<std::mutex> codeclock(codecMutex_);
    decoderThreadInitialized_ = true;
    codeclock.unlock();
    omxDecoderCond_.notify_one();

    while (!decoderThreadQuitFlag_) {
        status_t ret = omxDecoder_->read();
        if (Log::isVerbose()) Log_v("omxDecoder_ read ret = %d", ret);
    }

    if (Log::isInfo()) Log_i("omxDecoderThread ends");

    if (Log::isDebug()) Log_d("Stop omxDecoder");
    omxDecoder_->stop();
}

void OMXVideoCodec::setNativeWindow(ANativeWindow* nativeWindow, int height, int width){
    if (Log::isDebug()) Log_d("set native window size w: %d, h: %d", width, height);
    nativeWindow_ = nativeWindow;
    screenSize_.width = width;
    screenSize_.height = height;
}

void OMXVideoCodec::setSps(common::DataConstBuffer &b){
    if (Log::isDebug()) Log_d("sps packet len %d", b.size);
    omxSource_->getFormat()->setData(kKeyAVCC, kTypeAVCC, b.cdata, b.size);
}

OMXSource::Pointer OMXVideoCodec::source() {
    return omxSource_;
}

OMXDecoder::Pointer OMXVideoCodec::decoder() {
    return omxDecoder_;
}

void OMXVideoCodec::queueBuffer(common::DataConstBuffer& buffer, int64_t timestamp) {
    if (decoderThreadQuitFlag_){
        if(Log::isVerbose()) Log_v("decoder is stopping, return");
        return;
    }

    auto* mBuffer = new MediaBuffer(buffer.size);
    if (Log::isVerbose()) Log_v("create buffer %p", mBuffer);
    memcpy(mBuffer->data(), buffer.cdata, buffer.size);
    mBuffer->set_range(0, (size_t) buffer.size);
    mBuffer->meta_data()->clear();
    mBuffer->meta_data()->setInt64(kKeyTime, timestamp);

    source()->queueBuffer(mBuffer);
}
