#include <usb/LibUsb.h>
#include "it_smg_libs_aasdk_usb_LibUsb.h"

using namespace aasdk::usb;

jfieldID JLibUsb::handleId = nullptr;

JLibUsb::JLibUsb(JNIEnv *env, jobject jlibusb)
        : JNIBase(env, jlibusb, "JLibUsb"){
    libUsb_ = new LibUsb();
}

JLibUsb::Pointer JLibUsb::getJLibUsb(JNIEnv *env, jobject jlibusb) {
    return (JLibUsb::Pointer)env->GetLongField(jlibusb, JLibUsb::handleId);
}

LibUsbDevice::Pointer JLibUsb::createDevice() {
    return libUsb_->createDevice();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL Java_it_smg_libs_aasdk_usb_LibUsb_nativeInit(JNIEnv* env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/usb/LibUsb");
    JLibUsb::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL Java_it_smg_libs_aasdk_usb_LibUsb_nativeSetup(JNIEnv* env, jobject thiz) {
    auto jlibusb = new JLibUsb(env, thiz);
    return (jlong)((size_t)jlibusb);
}
