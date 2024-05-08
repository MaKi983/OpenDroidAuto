#include "it_smg_libs_aasdk_service_VideoService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_projection_VideoOutput.h"
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JVideoService::handleId = nullptr;

JVideoService::JVideoService(JNIEnv *env, jobject jvideoservice, jobject jmessenger, jobject jvideooutput)
        : JNIBase(env, jvideoservice, "JVideoService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JVideoOutput::Pointer jVideoOutput = projection::JVideoOutput::getJVideoOutput(env, jvideooutput);

    videoService_ = new VideoService(JRuntime::ioService(), jMessenger->getMessenger(), jVideoOutput);
}

JVideoService::~JVideoService() {
    delete videoService_;
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
Java_it_smg_libs_aasdk_service_VideoService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jvideooutput) {
    JVideoService::Pointer jVideoService = new JVideoService(env, thiz, jmessenger, jvideooutput);
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