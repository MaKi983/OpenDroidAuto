#include <service/SystemAudioService.h>
#include "it_smg_libs_aasdk_service_SystemAudioService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_projection_AudioOutput.h"

jfieldID JSystemAudioService::handleId = nullptr;

JSystemAudioService::JSystemAudioService(JNIEnv *env, jobject jsystemaudioservice, jobject jmessenger, jobject jaudiooutput)
        : JNIBase(env, jsystemaudioservice, "JSystemAudioService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JAudioOutput::Pointer jAudioOutput = projection::JAudioOutput::getJAudioOutput(env, jaudiooutput);

    audioService_ = new SystemAudioService(JRuntime::ioService(), jMessenger->getMessenger(), jAudioOutput);
}

JSystemAudioService::~JSystemAudioService() {
    delete audioService_;
}

JSystemAudioService::Pointer JSystemAudioService::getJSystemAudioService(JNIEnv *env, jobject jsystemaudioservice) {
    return (JSystemAudioService::Pointer)env->GetLongField(jsystemaudioservice, JSystemAudioService::handleId);;
}

IService::Pointer JSystemAudioService::getService() {
    return audioService_;
}

void JSystemAudioService::start() {
    audioService_->start();
}

void JSystemAudioService::stop() {
    audioService_->stop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SystemAudioService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/SystemAudioService");
    JSystemAudioService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_SystemAudioService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jaudiooutput) {
    JSystemAudioService::Pointer jSystemAudioService = new JSystemAudioService(env, thiz, jmessenger, jaudiooutput);
    return (jlong)((size_t)jSystemAudioService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SystemAudioService_nativeStart(JNIEnv *env, jobject thiz) {
    JSystemAudioService::Pointer jSystemAudioService = JSystemAudioService::getJSystemAudioService(env, thiz);
    jSystemAudioService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SystemAudioService_nativeStop(JNIEnv *env, jobject thiz) {
    JSystemAudioService::Pointer jSystemAudioService = JSystemAudioService::getJSystemAudioService(env, thiz);
    jSystemAudioService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SystemAudioService_nativeDelete(JNIEnv *env, jobject thiz) {
    JSystemAudioService::Pointer jSystemAudioService = JSystemAudioService::getJSystemAudioService(env, thiz);
    delete jSystemAudioService;
}