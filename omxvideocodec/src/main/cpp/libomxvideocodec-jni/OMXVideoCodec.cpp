#include "OMXVideoCodec.h"
#include "Log.h"

#include <unistd.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <inttypes.h>
#include <chrono>
#include <sstream>
#include <iomanip>

OMXVideoCodec::OMXVideoCodec(JNIEnv* env, jobject androidApp) : JNIBase(env, androidApp, "OMXVideoCodec")
    , fps_(0), omxDecoderThreadRunning_(false), decoderThreadInitialized_(false), decoderThreadQuitFlag_(false)
{
}

OMXVideoCodec::~OMXVideoCodec() {
    Log_i("destructor");
}

status_t OMXVideoCodec::init(int fps){
    fps_ = fps;
    if (Log::isInfo()) Log_i("FPS %d", fps_);
    // Create decoder thread and wait until ready
    if (Log::isInfo()) Log_i("start omxDecoderThread");

    std::unique_lock<std::mutex> codeclock(codecMutex_);
    omxDecoderThread_ = std::thread(&OMXVideoCodec::init_omxDecoderThread, this);
    while (!decoderThreadInitialized_){
        omxDecoderCond_.wait(codeclock);
    }
    codeclock.unlock();
    Log_i("omxDecoderThread ready");

    return omxDecoder_->getStatus();
}

void OMXVideoCodec::shutdown(){
    if (decoderThreadInitialized_) {
        if (Log::isInfo()) Log_i("Waiting for app thread to finish...");

        decoderThreadQuitFlag_ = true;

        if (Log::isDebug()) Log_d("Stop omxSource");
        omxSource_->stop();

        if (omxDecoderThreadRunning_) {
            if (Log::isDebug()) Log_d("Stopping omxDecoderThread");

            if (omxDecoderThread_.joinable()){
                if (Log::isDebug()) Log_d("joining decoder thread");
                omxDecoderThread_.join();
            }
        }

        if (Log::isDebug()) Log_d("release native window");
        ANativeWindow_release(nativeWindow_.get());

        if (Log::isDebug()) Log_d("delete decoder");
        delete omxDecoder_;
    }

    if (Log::isInfo()) Log_i("all done");
}

void OMXVideoCodec::init_omxDecoderThread() {
    if (Log::isInfo()) Log_i("omxDecoderThread begins");

    JNIEnv* env;
    JNIBase::javaAttachThread("OMXCodec thread", &env);

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

    if (Log::isDebug()) Log_d("Stop omxDecoder");
    omxDecoder_->stop();

    if (Log::isInfo()) Log_i("omxDecoderThread ends");
    omxDecoderThreadRunning_ = false;

    JNIBase::javaDetachThread();
}

void OMXVideoCodec::setNativeWindow(ANativeWindow* nativeWindow, int height, int width){
    if (Log::isDebug()) Log_d("set native window size w: %d, h: %d", width, height);
    nativeWindow_ = nativeWindow;
    screenSize_.width = width;
    screenSize_.height = height;
}

void OMXVideoCodec::setSps(unsigned char* buf, int len){
    if (Log::isDebug()) Log_d("sps packet len %d", len);
    omxSource_->getFormat()->setData(kKeyAVCC, kTypeAVCC, buf, len);
}

OMXSource::Pointer OMXVideoCodec::source() {
    return omxSource_;
}

OMXDecoder::Pointer OMXVideoCodec::decoder() {
    return omxDecoder_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_createNativeApp(JNIEnv* env, jobject application) {
    OMXVideoCodec::Pointer codec = new OMXVideoCodec(env, application);
    return (jlong)((size_t)codec);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeSurfaceInit(JNIEnv *env, jobject thiz, jlong handle, jobject surface,
                                                                     jint width, jint height) {
    auto omxCodec = (OMXVideoCodec::Pointer) handle;
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    omxCodec->setNativeWindow(nativeWindow, height, width);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeFinalize(JNIEnv *env, jobject thiz, jlong handle) {
    auto omxCodec = (OMXVideoCodec::Pointer) handle;
    omxCodec->shutdown();
    delete omxCodec;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeInit(JNIEnv *env, jobject thiz, jlong handle, jint fps) {
    status_t ret = 1;

    auto omxCodec = (OMXVideoCodec::Pointer) handle;
    ret = omxCodec->init(fps);

    return ret == 0 ? JNI_TRUE : JNI_FALSE;
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeConsume(JNIEnv *env, jobject thiz, jlong handle,
                                                                 jobject buf, int len, jlong t) {
    auto omxCodec = (OMXVideoCodec::Pointer) handle;
    jbyte *bufferData = (jbyte *) env->GetDirectBufferAddress(buf);

    Packet* buffer = omxCodec->source()->getWriteBuffer();

    if (Log::isVerbose()) Log_v("mediabuffer: %p", buffer);

    memcpy(buffer->buf, bufferData, len);

    buffer->timestamp = t;
    if (t <= 0){
        buffer->timestamp = 0;
    }
    buffer->len = len;

    omxCodec->source()->queueReadBuffer(buffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeSetSps(JNIEnv *env, jobject thiz,
                                                                jlong handle, jobject buf,
                                                                jint len) {
    auto omxCodec = (OMXVideoCodec::Pointer) handle;

    unsigned char* tmp = new unsigned char[len];

    jbyte *bufferData = (jbyte *) env->GetDirectBufferAddress(buf);
    memcpy(tmp, bufferData, len);

    omxCodec->setSps(tmp, len);
}
