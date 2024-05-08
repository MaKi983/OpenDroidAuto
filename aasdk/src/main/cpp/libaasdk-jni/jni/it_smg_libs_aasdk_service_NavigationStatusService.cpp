#include "it_smg_libs_aasdk_service_NavigationStatusService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_projection_NavigationStatusEvent.h"

jfieldID JNavigationStatusService::handleId = nullptr;

JNavigationStatusService::JNavigationStatusService(JNIEnv *env, jobject jnavigationstatusservice, jobject jmessenger, jobject jnavigationstatusevent)
        : JNIBase(env, jnavigationstatusservice, "JNavigationStatusService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JNavigationStatusEvent::Pointer jNavigationStatusEvent = projection::JNavigationStatusEvent::getJNavigationStatusEvent(env, jnavigationstatusevent);

    navigationStatusService_ = new NavigationStatusService(JRuntime::ioService(), jMessenger->getMessenger(), jNavigationStatusEvent);
}

JNavigationStatusService::~JNavigationStatusService() {
    delete navigationStatusService_;
}

JNavigationStatusService::Pointer JNavigationStatusService::getJNavigationStatusService(JNIEnv *env, jobject jnavigationstatusservice) {
    return (JNavigationStatusService::Pointer)env->GetLongField(jnavigationstatusservice, JNavigationStatusService::handleId);
}

IService::Pointer JNavigationStatusService::getService() {
    return navigationStatusService_;
}

void JNavigationStatusService::start() {
    navigationStatusService_->start();
}

void JNavigationStatusService::stop() {
    navigationStatusService_->stop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_NavigationStatusService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/NavigationStatusService");
    JNavigationStatusService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_NavigationStatusService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jnavigationstatusevent) {
    JNavigationStatusService::Pointer jNavigationStatusService = new JNavigationStatusService(env, thiz, jmessenger, jnavigationstatusevent);
    return (jlong)((size_t)jNavigationStatusService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_NavigationStatusService_nativeStart(JNIEnv *env, jobject thiz) {
    JNavigationStatusService::Pointer jNavigationStatusService = JNavigationStatusService::getJNavigationStatusService(env, thiz);
    jNavigationStatusService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_NavigationStatusService_nativeStop(JNIEnv *env, jobject thiz) {
    JNavigationStatusService::Pointer jNavigationStatusService = JNavigationStatusService::getJNavigationStatusService(env, thiz);
    jNavigationStatusService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_NavigationStatusService_nativeDelete(JNIEnv *env, jobject thiz) {
JNavigationStatusService::Pointer jNavigationStatusService = JNavigationStatusService::getJNavigationStatusService(env, thiz);
    delete jNavigationStatusService;
}
