#include <service/SpeechAudioService.h>
#include "it_smg_libs_aasdk_service_SpeechAudioService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_projection_AudioOutput.h"

jfieldID JSpeechAudioService::handleId = nullptr;

JSpeechAudioService::JSpeechAudioService(JNIEnv *env, jobject jspeechaudioservice, jobject jmessenger, jobject jaudiooutput)
        : JNIBase(env, jspeechaudioservice, "JSpeechAudioService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JAudioOutput::Pointer jAudioOutput = projection::JAudioOutput::getJAudioOutput(env, jaudiooutput);

    audioService_ = std::make_shared<SpeechAudioService>(JRuntime::ioService(), jMessenger->getMessenger(), jAudioOutput, this);
}

void JSpeechAudioService::initJavaMethods() {
//    JNIEnv* env = getJniEnv();
//    jclass cls = env->GetObjectClass(androidClass_);
//
//    onErrorMethodId_ = env->GetMethodID(cls, "onError", "(Ljava/lang/String;I)V");
//
//    env->DeleteLocalRef(cls);
}

JSpeechAudioService::~JSpeechAudioService() {
    if (Log::isVerbose()) Log_v("destructor");
    audioService_.reset();
}

JSpeechAudioService::Pointer JSpeechAudioService::getJSpeechAudioService(JNIEnv *env, jobject jspeechaudioservice) {
    return (JSpeechAudioService::Pointer)env->GetLongField(jspeechaudioservice, JSpeechAudioService::handleId);;
}

IService::Pointer JSpeechAudioService::getService() {
    return audioService_;
}

void JSpeechAudioService::start() {
    audioService_->start();
}

void JSpeechAudioService::stop() {
    audioService_->stop();
}

void JSpeechAudioService::onError(const aasdk::error::Error &e) {
    JNIEnv* env = getJniEnv();
    jstring jstr = env->NewStringUTF(e.what());

    jclass cls = env->GetObjectClass(androidClass_);
    jmethodID onErrorMethodId = env->GetMethodID(cls, "onError", "(Ljava/lang/String;I)V");
    env->CallVoidMethod(androidClass_, onErrorMethodId, jstr, e.getNativeCode());

    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(jstr);
}
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SpeechAudioService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/SpeechAudioService");
    JSpeechAudioService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_SpeechAudioService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jaudiooutput) {
    JSpeechAudioService::Pointer jSpeechAudioService = new JSpeechAudioService(env, thiz, jmessenger, jaudiooutput);
    return (jlong)((size_t)jSpeechAudioService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SpeechAudioService_nativeStart(JNIEnv *env, jobject thiz) {
    JSpeechAudioService::Pointer jSpeechAudioService = JSpeechAudioService::getJSpeechAudioService(env, thiz);
    jSpeechAudioService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SpeechAudioService_nativeStop(JNIEnv *env, jobject thiz) {
    JSpeechAudioService::Pointer jSpeechAudioService = JSpeechAudioService::getJSpeechAudioService(env, thiz);
    jSpeechAudioService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SpeechAudioService_nativeDelete(JNIEnv *env, jobject thiz) {
    JSpeechAudioService::Pointer jSpeechAudioService = JSpeechAudioService::getJSpeechAudioService(env, thiz);
    delete jSpeechAudioService;
}