#include <Log.h>
#include "it_smg_libs_aasdk_service_VideoService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_projection_VideoOutput.h"
#include "it_smg_libs_aasdk_Runtime.h"

JVideoEventHandler::JVideoEventHandler(JNIEnv *env, jobject jandroidautoentityeventhandler)
        : JNIBase(env, jandroidautoentityeventhandler, "JAndroidAutoEntityEventHandler")  {

    initJavaMethods();
}

void JVideoEventHandler::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    onAVChannelStopIndicationMethodId_ = env->GetMethodID(cls, "onAVChannelStopIndication", "()V");

    env->DeleteLocalRef(cls);
}

void JVideoEventHandler::onAVChannelStopIndication() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, onAVChannelStopIndicationMethodId_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

jfieldID JVideoService::handleId = nullptr;

JVideoService::JVideoService(JNIEnv *env, jobject jvideoservice, jobject jmessenger, jobject jvideooutput, jobject jvideoeventhandler)
        : JNIBase(env, jvideoservice, "JVideoService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JVideoOutput::Pointer jVideoOutput = projection::JVideoOutput::getJVideoOutput(env, jvideooutput);
    JVideoEventHandler::Pointer jVideoEventHandler = new JVideoEventHandler(env, jvideoeventhandler);
    videoService_ = std::make_shared<VideoService>(JRuntime::ioService(), jMessenger->getMessenger(), jVideoOutput, jVideoEventHandler, this);
}

void JVideoService::initJavaMethods() {
//    JNIEnv* env = getJniEnv();
//    jclass cls = env->GetObjectClass(androidClass_);
//
//    onErrorMethodId_ = env->GetMethodID(cls, "onError", "(Ljava/lang/String;I)V");
//
//    env->DeleteLocalRef(cls);
}

JVideoService::~JVideoService() {
//    videoService_.reset();
}

JVideoService::Pointer JVideoService::getJVideoService(JNIEnv *env, jobject jvideoservice) {
    return (JVideoService::Pointer)env->GetLongField(jvideoservice, JVideoService::handleId);;
}

IService::Pointer JVideoService::getService() {
    return videoService_;
}

void JVideoService::start() {
    videoService_->start();
}

void JVideoService::stop() {
    videoService_->stop();
}

void JVideoService::releaseFocus() {
    aasdk::proto::messages::VideoFocusRequest request;
    request.set_focus_mode(aasdk::proto::enums::VideoFocusMode_Enum_UNFOCUSED);
    videoService_->onVideoFocusRequest(request);
}

void JVideoService::gainFocus() {
    aasdk::proto::messages::VideoFocusRequest request;
    request.set_focus_mode(aasdk::proto::enums::VideoFocusMode_Enum_FOCUSED);
    videoService_->onVideoFocusRequest(request);
}

void JVideoService::onError(const aasdk::error::Error &e) {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);
    jmethodID onErrorMethodId = env->GetMethodID(cls, "onError", "(Ljava/lang/String;I)V");

    jstring jstr = env->NewStringUTF(e.what());
    env->CallVoidMethod(androidClass_, onErrorMethodId, jstr, e.getNativeCode());
    env->DeleteLocalRef(jstr);
    env->DeleteLocalRef(cls);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_VideoService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/VideoService");
    JVideoService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_VideoService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jvideooutput, jobject jvideoeventhandler) {
    JVideoService::Pointer jVideoService = new JVideoService(env, thiz, jmessenger, jvideooutput, jvideoeventhandler);
    return (jlong)((size_t)jVideoService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_VideoService_nativeStart(JNIEnv *env, jobject thiz) {
    JVideoService::Pointer jVideoService = JVideoService::getJVideoService(env, thiz);
    jVideoService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_VideoService_nativeStop(JNIEnv *env, jobject thiz) {
    JVideoService::Pointer jVideoService = JVideoService::getJVideoService(env, thiz);
    jVideoService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_VideoService_nativeReleaseFocus(JNIEnv *env, jobject thiz) {
    JVideoService::Pointer jVideoService = JVideoService::getJVideoService(env, thiz);
    jVideoService->releaseFocus();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_VideoService_nativeGainFocus(JNIEnv *env, jobject thiz) {
    JVideoService::Pointer jVideoService = JVideoService::getJVideoService(env, thiz);
    jVideoService->gainFocus();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_VideoService_nativeDelete(JNIEnv *env, jobject thiz) {
    JVideoService::Pointer jVideoService = JVideoService::getJVideoService(env, thiz);
    delete jVideoService;
}