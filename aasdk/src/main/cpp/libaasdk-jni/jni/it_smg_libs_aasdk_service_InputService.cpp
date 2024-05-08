#include "it_smg_libs_aasdk_service_InputService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_projection_InputDevice.h"
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JInputService::handleId = nullptr;

JInputService::JInputService(JNIEnv *env, jobject jinputervice, jobject jmessenger, jobject jinputdevice)
        : JNIBase(env, jinputervice, "JInputService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JInputDevice::Pointer jInputDevice = projection::JInputDevice::getJInputDevice(env, jinputdevice);

    inputService_ = new InputService(JRuntime::ioService(), jMessenger->getMessenger(), jInputDevice);
}

JInputService::Pointer JInputService::getJInputService(JNIEnv *env, jobject jinputervice) {
    return (JInputService::Pointer)env->GetLongField(jinputervice, JInputService::handleId);;
}

IService::Pointer JInputService::getService() {
    return inputService_;
}

void JInputService::start() {
    inputService_->start();
}

void JInputService::stop() {
    inputService_->stop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_InputService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/InputService");
    JInputService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_InputService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jinputdevice) {
    JInputService::Pointer jInputService = new JInputService(env, thiz, jmessenger, jinputdevice);
    return (jlong)((size_t)jInputService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_InputService_nativeStart(JNIEnv *env, jobject thiz) {
    JInputService::Pointer jInputService = JInputService::getJInputService(env, thiz);
    jInputService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_InputService_nativeStop(JNIEnv *env, jobject thiz) {
    JInputService::Pointer jInputService = JInputService::getJInputService(env, thiz);
    jInputService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_InputService_nativeDelete(JNIEnv *env, jobject thiz) {
    JInputService::Pointer jInputService = JInputService::getJInputService(env, thiz);
    delete jInputService;
}