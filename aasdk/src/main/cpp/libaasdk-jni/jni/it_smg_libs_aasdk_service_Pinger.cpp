#include <service/Pinger.h>
#include "it_smg_libs_aasdk_service_Pinger.h"
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JPinger::handleId = nullptr;

JPinger::JPinger(JNIEnv *env, jobject jpinger, int duration)
        : JNIBase(env, jpinger, "JPinger")  {

    pinger_ = std::make_shared<Pinger>(JRuntime::ioService(), duration);
}

JPinger::~JPinger() {
//    pinger_.reset();
}

JPinger::Pointer JPinger::getJPinger(JNIEnv *env, jobject jpinger) {
    return (JPinger::Pointer)env->GetLongField(jpinger, JPinger::handleId);;
}

IPinger::Pointer JPinger::getPinger() {
    return pinger_;
}

void JPinger::cancel() {
    pinger_->cancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_Pinger_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/Pinger");
    JPinger::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_Pinger_nativeSetup(JNIEnv *env, jobject thiz, jint duration) {
    JPinger::Pointer jPinger = new JPinger(env, thiz, duration);
    return (jlong)((size_t)jPinger);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_Pinger_nativeDelete(JNIEnv *env, jobject thiz) {
    JPinger::Pointer jPinger = JPinger::getJPinger(env, thiz);
    delete jPinger;
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_Pinger_cancel(JNIEnv *env, jobject thiz) {
    JPinger::Pointer jPinger = JPinger::getJPinger(env, thiz);
    jPinger->cancel();
}