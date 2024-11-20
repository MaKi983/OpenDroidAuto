#include "it_smg_libs_aasdk_service_BluetoothService.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_projection_BluetoothDevice.h"
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JBluetoothService::handleId = nullptr;

JBluetoothService::JBluetoothService(JNIEnv *env, jobject jbluetoothservice, jobject jmessenger, jobject jbluetoothdevice)
        : JNIBase(env, jbluetoothservice, "JBluetoothService")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);
    projection::JBluetoothDevice::Pointer jBluetoothDevice = projection::JBluetoothDevice::getJBluetoothDevice(env, jbluetoothdevice);

    bluetoothService_ = std::make_shared<BluetoothService>(JRuntime::ioService(), jMessenger->getMessenger(), jBluetoothDevice, this);
}

void JBluetoothService::initJavaMethods() {
//    JNIEnv* env = getJniEnv();
//    jclass cls = env->GetObjectClass(androidClass_);
//
//    onErrorMethodId_ = env->GetMethodID(cls, "onError", "(Ljava/lang/String;I)V");
//
//    env->DeleteLocalRef(cls);
}

JBluetoothService::~JBluetoothService() {
//    bluetoothService_.reset();
}

JBluetoothService::Pointer JBluetoothService::getJBluetoothService(JNIEnv *env, jobject jbluetoothservice) {
    return (JBluetoothService::Pointer)env->GetLongField(jbluetoothservice, JBluetoothService::handleId);;
}

IService::Pointer JBluetoothService::getService() {
    return bluetoothService_;
}

void JBluetoothService::start() {
    bluetoothService_->start();
}

void JBluetoothService::stop() {
    bluetoothService_->stop();
}

void JBluetoothService::onError(const aasdk::error::Error &e) {
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
Java_it_smg_libs_aasdk_service_BluetoothService_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/BluetoothService");
    JBluetoothService::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_BluetoothService_nativeSetup(JNIEnv *env, jobject thiz, jobject jmessenger, jobject jbluetoothdevice) {
    auto jBluetoothService = new JBluetoothService(env, thiz, jmessenger, jbluetoothdevice);
    return (jlong)((size_t)jBluetoothService);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_BluetoothService_nativeStart(JNIEnv *env, jobject thiz) {
    JBluetoothService::Pointer jBluetoothService = JBluetoothService::getJBluetoothService(env, thiz);
    jBluetoothService->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_BluetoothService_nativeStop(JNIEnv *env, jobject thiz) {
    JBluetoothService::Pointer jBluetoothService = JBluetoothService::getJBluetoothService(env, thiz);
    jBluetoothService->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_BluetoothService_nativeDelete(JNIEnv *env, jobject thiz) {
    JBluetoothService::Pointer jBluetoothService = JBluetoothService::getJBluetoothService(env, thiz);
    delete jBluetoothService;
}