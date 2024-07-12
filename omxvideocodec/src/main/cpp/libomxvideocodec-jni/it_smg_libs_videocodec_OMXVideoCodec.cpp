#include "it_smg_libs_videocodec_OMXVideoCodec.h"

jfieldID JOMXVideoCodec::handleId = nullptr;

JOMXVideoCodec::JOMXVideoCodec(JNIEnv *env, jobject jvideocodec) :
        JNIBase(env, jvideocodec, "JOMXVideoCodec"), omxVideoCodec_(new OMXVideoCodec()){

    initJavaMethods();
}

void JOMXVideoCodec::initJavaMethods() {
}

JOMXVideoCodec::Pointer JOMXVideoCodec::getJOMXVideoCodec(JNIEnv *env, jobject jvideocodec) {
    return (JOMXVideoCodec::Pointer)env->GetLongField(jvideocodec, JOMXVideoCodec::handleId);
}

void JOMXVideoCodec::setNativeWindow(ANativeWindow* nativeWindow, int height, int width){
    omxVideoCodec_->setNativeWindow(nativeWindow, height, width);
}

status_t JOMXVideoCodec::init(int fps){
    return omxVideoCodec_->init(fps);
}

void JOMXVideoCodec::shutdown(){
    omxVideoCodec_->shutdown();
}

void JOMXVideoCodec::queueBuffer(common::DataConstBuffer &b, int64_t timestamp) {
    omxVideoCodec_->queueBuffer(b, timestamp);
}

void JOMXVideoCodec::setSps(common::DataConstBuffer &b){
    omxVideoCodec_->setSps(b);
}

JOMXVideoCodec::~JOMXVideoCodec() {
    delete omxVideoCodec_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/omxvideocodec/OMXVideoCodec");
    JOMXVideoCodec::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeSetup(JNIEnv* env, jobject thiz) {
    JOMXVideoCodec::Pointer jOMXVideoCodec = new JOMXVideoCodec(env, thiz);
    return (jlong)((size_t)jOMXVideoCodec);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeSurfaceInit(JNIEnv *env, jobject thiz, jobject surface, jint width, jint height) {
    JOMXVideoCodec::Pointer jOMXVideoCodec = JOMXVideoCodec::getJOMXVideoCodec(env, thiz);

    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    jOMXVideoCodec->setNativeWindow(nativeWindow, height, width);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeDecoderInit(JNIEnv *env, jobject thiz, jint fps) {
    status_t ret = 1;

    JOMXVideoCodec::Pointer jOMXVideoCodec = JOMXVideoCodec::getJOMXVideoCodec(env, thiz);
    ret = jOMXVideoCodec->init(fps);

    return ret == 0 ? JNI_TRUE : JNI_FALSE;
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeDelete(JNIEnv *env, jobject thiz) {
    JOMXVideoCodec::Pointer jOMXVideoCodec = JOMXVideoCodec::getJOMXVideoCodec(env, thiz);
    jOMXVideoCodec->shutdown();
    delete jOMXVideoCodec;
}


extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeConsume(JNIEnv *env, jobject thiz, jobject buf, int len, jlong t) {
    JOMXVideoCodec::Pointer jOMXVideoCodec = JOMXVideoCodec::getJOMXVideoCodec(env, thiz);

    jbyte *bufferData = (jbyte *) env->GetDirectBufferAddress(buf);
    common::DataConstBuffer buffer(bufferData, len);
    int64_t timestamp = static_cast<int64_t>(t);

    jOMXVideoCodec->queueBuffer(buffer, timestamp);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_omxvideocodec_OMXVideoCodec_nativeSetSps(JNIEnv *env, jobject thiz, jobject buf, jint len) {
    JOMXVideoCodec::Pointer jOMXVideoCodec = JOMXVideoCodec::getJOMXVideoCodec(env, thiz);

    jbyte *bufferData = (jbyte *) env->GetDirectBufferAddress(buf);
    common::DataConstBuffer buffer(bufferData, len);

    jOMXVideoCodec->setSps(buffer);
}
