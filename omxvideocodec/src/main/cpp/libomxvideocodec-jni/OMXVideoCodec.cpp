#include "OMXVideoCodec.h"
#include "Log.h"

#include <unistd.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <inttypes.h>
#include <chrono>
#include <sstream>
#include <iomanip>

OMXVideoCodec::OMXVideoCodec()
    : fps_(0), omxDecoderThreadRunning_(false), decoderThreadInitialized_(false), decoderThreadQuitFlag_(false){
}

OMXVideoCodec::~OMXVideoCodec() {
    if (Log::isVerbose()) Log_v("destructor");
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

        decoderThreadQuitFlag_ = true;

        if (Log::isDebug()) Log_d("Stop omxSource");
        omxSource_->stop();

        if (Log::isDebug()) Log_d("Stop omxDecoder");
        omxDecoder_->stop();

        if (Log::isDebug()) Log_d("Stopping omxDecoderThread");

        if (omxDecoderThread_.joinable()){
            if (Log::isDebug()) Log_d("joining decoder thread");
            omxDecoderThread_.join();
        }

        if (Log::isDebug()) Log_d("release native window");
        ANativeWindow_release(nativeWindow_.get());

        if (Log::isDebug()) Log_d("delete decoder");
        delete omxDecoder_;
        omxDecoder_ = nullptr;

        omxSource_ = nullptr;
    }

    if (Log::isInfo()) Log_i("all done");
}

void OMXVideoCodec::init_omxDecoderThread() {
    if (Log::isInfo()) Log_i("omxDecoderThread begins");

//    JNIEnv* env;
//    JNIBase::javaAttachThread("OMXCodec thread", &env);

    omxSource_ = new OMXSource(screenSize_.width, screenSize_.height, fps_, codecMutex_);

    if (Log::isDebug()) Log_d("Created OMXSource %p", omxSource_);

    omxDecoder_ = new OMXDecoder(codecMutex_);
    omxDecoder_->setNativeWindow(nativeWindow_);
    omxDecoder_->setSource(omxSource_);
    omxDecoder_->init();

    std::unique_lock<std::mutex> codeclock(codecMutex_);
    omxDecoderThreadRunning_ = true;
    decoderThreadInitialized_ = true;
    codeclock.unlock();
    omxDecoderCond_.notify_one();

    while (!decoderThreadQuitFlag_) {
        omxDecoder_->read();
    }

    if (Log::isInfo()) Log_i("omxDecoderThread ends");
    omxDecoderThreadRunning_ = false;

//    JNIBase::javaDetachThread();
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

    common::Data d;
    common::copy(d, buffer);

    Packet::Pointer p = new Packet(d, timestamp, d.size());
    source()->queueBuffer(p);
}
