#include "it_smg_libs_aasdk_projection_BluetoothDevice.h"

using namespace projection;

jfieldID JBluetoothDevice::handleId = nullptr;

JBluetoothDevice::JBluetoothDevice(JNIEnv *env, jobject jbluetoothdevice) :
        JNIBase(env, jbluetoothdevice, "JBluetoothDevice"){

    initJavaMethods();
}

JBluetoothDevice::Pointer JBluetoothDevice::getJBluetoothDevice(JNIEnv *env, jobject jbluetoothdevice) {
    return (JBluetoothDevice::Pointer)env->GetLongField(jbluetoothdevice, JBluetoothDevice::handleId);;
}

void JBluetoothDevice::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    stopMethodId_ = env->GetMethodID(cls, "stop", "()V");
    isPairedMethodId_ = env->GetMethodID(cls, "isPaired", "(Ljava/lang/String;)Z");
    pairMethodId_ = env->GetMethodID(cls, "pair", "(Ljava/lang/String;)Z");
    getLocalAddressMethodId_ = env->GetMethodID(cls, "getLocalAddress", "()Ljava/lang/String;" );
    isAvailableMethodId_ = env->GetMethodID(cls, "isAvailable", "()Z");

    env->DeleteLocalRef(cls);
}

void JBluetoothDevice::stop() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, stopMethodId_);
}

bool JBluetoothDevice::isPaired(const std::string& address) {
    JNIEnv* env = getJniEnv();
    jstring jstr = env->NewStringUTF(address.c_str());
    jboolean ret = env->CallBooleanMethod(androidClass_, isPairedMethodId_, jstr);
    env->DeleteLocalRef(jstr);
    return ret == JNI_TRUE;
}

void JBluetoothDevice::pair(const std::string& address, PairingPromise::Pointer promise) {
    JNIEnv* env = getJniEnv();
    jstring jstr = env->NewStringUTF(address.c_str());
    jboolean ret = env->CallBooleanMethod(androidClass_, pairMethodId_, jstr);
    if (ret == JNI_TRUE){
        promise->resolve();
    } else {
        promise->reject();
    }
}

std::string JBluetoothDevice::getLocalAddress() {
    JNIEnv* env = getJniEnv();
    jstring jaddr = (jstring) env->CallObjectMethod(androidClass_, getLocalAddressMethodId_);

    const char *strReturn = env->GetStringUTFChars(jaddr, 0);
    std::string address(strReturn);

    env->ReleaseStringUTFChars(jaddr, strReturn);

    return address;
}

bool JBluetoothDevice::isAvailable() {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, isAvailableMethodId_);
    return ret == JNI_TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_BluetoothDevice_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/projection/BluetoothDevice");
    JBluetoothDevice::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_projection_BluetoothDevice_nativeSetup(JNIEnv *env, jobject thiz) {
    JBluetoothDevice::Pointer jBluetoothDevice = new JBluetoothDevice(env, thiz);
    return (jlong)((size_t)jBluetoothDevice);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_BluetoothDevice_nativeDelete(JNIEnv *env, jobject thiz) {
    JBluetoothDevice::Pointer jBluetoothDevice = JBluetoothDevice::getJBluetoothDevice(env, thiz);
    delete jBluetoothDevice;
}