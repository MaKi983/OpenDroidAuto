#include <service/MediaAudioService.hpp>
#include "it_smg_libs_aasdk_service_MediaAudioService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_projection_AudioOutput.h"

jfieldID JMediaAudioService::handleId = nullptr;

JMediaAudioService::JMediaAudioService(JNIEnv *env, jobject jmediaaudioservice, jobject jmessenger, jobject jaudiooutput)
        : JNIBase(env, jmediaaudioservice, "JMediaAudioService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JAudioOutput::Pointer jAudioOutput = projection::JAudioOutput::getJAudioOutput(env, jaudiooutput);

    audioService_ = new MediaAudioService(JRuntime::ioService(), jMessenger->getMessenger(), jAudioOutput);
}

JMediaAudioService::~JMediaAudioService() {
    delete audioService_;
}

JMediaAudioService::Pointer JMediaAudioService::getJMediaAudioService(JNIEnv *env, jobject jmediaaudioservice) {
    return (JMediaAudioService::Pointer)env->GetLongField(jmediaaudioservice, JMediaAudioService::handleId);;
}

IService::Pointer JMediaAudioService::getService() {
    return audioService_;
}

void JMediaAudioService::start() {
    audioService_->start();
}

void JMediaAudioService::stop() {
    audioService_->stop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_MediaAudioService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/MediaAudioService");
    JMediaAudioService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_MediaAudioService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jaudiooutput) {
    JMediaAudioService::Pointer jMediaAudioService = new JMediaAudioService(env, thiz, jmessenger, jaudiooutput);
    return (jlong)((size_t)jMediaAudioService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_MediaAudioService_nativeStart(JNIEnv *env, jobject thiz) {
    JMediaAudioService::Pointer jMediaAudioService = JMediaAudioService::getJMediaAudioService(env, thiz);
    jMediaAudioService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_MediaAudioService_nativeStop(JNIEnv *env, jobject thiz) {
    JMediaAudioService::Pointer jMediaAudioService = JMediaAudioService::getJMediaAudioService(env, thiz);
    jMediaAudioService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_MediaAudioService_nativeDelete(JNIEnv *env, jobject thiz) {
    JMediaAudioService::Pointer jMediaAudioService = JMediaAudioService::getJMediaAudioService(env, thiz);
    delete jMediaAudioService;
}