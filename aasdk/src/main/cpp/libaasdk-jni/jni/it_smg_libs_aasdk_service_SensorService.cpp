#include "it_smg_libs_aasdk_service_SensorService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JSensorService::handleId = nullptr;

JSensorService::JSensorService(JNIEnv *env, jobject jsensorservice, jobject jmessenger, bool isnight)
        : JNIBase(env, jsensorservice, "JSensorService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);

    sensorService_ = std::make_shared<SensorService>(JRuntime::ioService(), jMessenger->getMessenger(), this, isnight);
}

void JSensorService::initJavaMethods() {
//    JNIEnv* env = getJniEnv();
//    jclass cls = env->GetObjectClass(androidClass_);
//
//    onErrorMethodId_ = env->GetMethodID(cls, "onError", "(Ljava/lang/String;I)V");
//
//    env->DeleteLocalRef(cls);
}

JSensorService::~JSensorService() {
//    sensorService_.reset();
}

JSensorService::Pointer JSensorService::getJSensorService(JNIEnv *env, jobject jinputservice) {
    return (JSensorService::Pointer)env->GetLongField(jinputservice, JSensorService::handleId);
}

IService::Pointer JSensorService::getService() {
    return sensorService_;
}

void JSensorService::sendNightMode(bool isNight) {
    sensorService_->setNightMode(isNight);
}

void JSensorService::start() {
    sensorService_->start();
}

void JSensorService::stop() {
    sensorService_->stop();
}

void JSensorService::onError(const aasdk::error::Error &e) {
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
Java_it_smg_libs_aasdk_service_SensorService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/SensorService");
    JSensorService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_SensorService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jboolean isnight) {
    JSensorService::Pointer jSensorService = new JSensorService(env, thiz, jmessenger, isnight);
    return (jlong)((size_t)jSensorService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SensorService_sendNightMode(JNIEnv *env, jobject thiz,
                                                          jboolean isnight) {
    JSensorService::Pointer jSensorService = JSensorService::getJSensorService(env, thiz);
    jSensorService->sendNightMode(isnight);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SensorService_nativeStart(JNIEnv *env, jobject thiz) {
    JSensorService::Pointer jSensorService = JSensorService::getJSensorService(env, thiz);
    jSensorService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SensorService_nativeStop(JNIEnv *env, jobject thiz) {
    JSensorService::Pointer jSensorService = JSensorService::getJSensorService(env, thiz);
    jSensorService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_SensorService_nativeDelete(JNIEnv *env, jobject thiz) {
    JSensorService::Pointer jSensorService = JSensorService::getJSensorService(env, thiz);
    delete jSensorService;
}