#include "it_smg_libs_aasdk_service_MediaStatusService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_projection_MediaStatusEvent.h"

jfieldID JMediaStatusService::handleId = nullptr;

JMediaStatusService::JMediaStatusService(JNIEnv *env, jobject jmediastatusservice, jobject jmessenger, jobject jmediastatusevent)
        : JNIBase(env, jmediastatusservice, "JMediaStatusService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JMediaStatusEvent::Pointer jMediaStatusEvent = projection::JMediaStatusEvent::getJMediaStatusEvent(env, jmediastatusevent);

    mediaStatusService_ = std::make_shared<MediaStatusService>(JRuntime::ioService(), jMessenger->getMessenger(), jMediaStatusEvent, this);
}

void JMediaStatusService::initJavaMethods() {
//    JNIEnv* env = getJniEnv();
//    jclass cls = env->GetObjectClass(androidClass_);
//
//    onErrorMethodId_ = env->GetMethodID(cls, "onError", "(Ljava/lang/String;I)V");
//
//    env->DeleteLocalRef(cls);
}

JMediaStatusService::~JMediaStatusService() {
//    mediaStatusService_.reset();
}

JMediaStatusService::Pointer JMediaStatusService::getJMediaStatusService(JNIEnv *env, jobject jmediastatusservice) {
    return (JMediaStatusService::Pointer)env->GetLongField(jmediastatusservice, JMediaStatusService::handleId);
}

IService::Pointer JMediaStatusService::getService() {
    return mediaStatusService_;
}

void JMediaStatusService::start() {
    mediaStatusService_->start();
}

void JMediaStatusService::stop() {
    mediaStatusService_->stop();
}

void JMediaStatusService::onError(const aasdk::error::Error &e) {
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
Java_it_smg_libs_aasdk_service_MediaStatusService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/MediaStatusService");
    JMediaStatusService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_MediaStatusService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jnavigationstatusevent) {
    JMediaStatusService::Pointer jMediaStatusService = new JMediaStatusService(env, thiz, jmessenger, jnavigationstatusevent);
    return (jlong)((size_t)jMediaStatusService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_MediaStatusService_nativeStart(JNIEnv *env, jobject thiz) {
    JMediaStatusService::Pointer jMediaStatusService = JMediaStatusService::getJMediaStatusService(env, thiz);
    jMediaStatusService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_MediaStatusService_nativeStop(JNIEnv *env, jobject thiz) {
    JMediaStatusService::Pointer jMediaStatusService = JMediaStatusService::getJMediaStatusService(env, thiz);
    jMediaStatusService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_MediaStatusService_nativeDelete(JNIEnv *env, jobject thiz) {
    JMediaStatusService::Pointer jMediaStatusService = JMediaStatusService::getJMediaStatusService(env, thiz);
    delete jMediaStatusService;
}
