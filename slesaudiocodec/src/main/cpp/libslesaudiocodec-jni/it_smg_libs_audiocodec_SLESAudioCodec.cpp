#include <Log.h>
#include "it_smg_libs_audiocodec_SLESAudioCodec.h"

jfieldID JSLESAudioCodec::handleId = nullptr;

JSLESAudioCodec::JSLESAudioCodec(JNIEnv *env, jobject jaudioinput, int sampleRate, int channelConfig, int sampleSize) :
        JNIBase(env, jaudioinput, "JSLESAudioCodec"), slesAudioCodec_(new SLESAudioCodec(sampleRate, channelConfig, sampleSize)){

    initJavaMethods();
}

JSLESAudioCodec::Pointer JSLESAudioCodec::getJSLESAudioCodec(JNIEnv *env, jobject jaudioinput) {
    return (JSLESAudioCodec::Pointer)env->GetLongField(jaudioinput, JSLESAudioCodec::handleId);;
}

void JSLESAudioCodec::initJavaMethods() {
}

void JSLESAudioCodec::queueBuffer(common::DataConstBuffer &b, int64_t timestamp) {
    Log_v("queueBuffer buffer size %d", b.size);
    slesAudioCodec_->queueBuffer(b, timestamp);
}

void JSLESAudioCodec::stop() {
    slesAudioCodec_->shutdown();
}

JSLESAudioCodec::~JSLESAudioCodec() {
    delete slesAudioCodec_;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_slesaudiocodec_SLESAudioCodec_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/slesaudiocodec/SLESAudioCodec");
    JSLESAudioCodec::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_slesaudiocodec_SLESAudioCodec_nativeSetup(JNIEnv *env, jobject thiz, jint sampleRate, jint channelConfig, jint sampleSize) {
    JSLESAudioCodec::Pointer jSLESAudioCodec = new JSLESAudioCodec(env, thiz, sampleRate, channelConfig, sampleSize);
    return (jlong)((size_t)jSLESAudioCodec);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_slesaudiocodec_SLESAudioCodec_nativeConsume(JNIEnv *env, jobject thiz, jobject buf, jint len, jlong t) {
    JSLESAudioCodec::Pointer jSLESAudioCodec = JSLESAudioCodec::getJSLESAudioCodec(env, thiz);
    jbyte *bufferData = (jbyte *) env->GetDirectBufferAddress(buf);
    common::DataConstBuffer buffer(bufferData, len);
    int64_t timestamp = static_cast<int64_t>(t);

    jSLESAudioCodec->queueBuffer(buffer, timestamp);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_slesaudiocodec_SLESAudioCodec_nativeDelete(JNIEnv *env, jobject thiz) {
    JSLESAudioCodec::Pointer jSLESAudioCodec = JSLESAudioCodec::getJSLESAudioCodec(env, thiz);
    jSLESAudioCodec->stop();
    delete jSLESAudioCodec;
}