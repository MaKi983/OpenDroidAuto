#include <usb/LibUsbDevice.h>
#include "it_smg_libs_aasdk_usb_AOAPDevice.h"
#include "it_smg_libs_aasdk_usb_LibUsbDevice.h"
#include "it_smg_libs_aasdk_Runtime.h"

using namespace aasdk::usb;

jfieldID JAOAPDevice::handleId = nullptr;

JAOAPDevice::JAOAPDevice(JNIEnv *env, jobject jaoapdevice, jobject jlibusbdevice)
    : JNIBase(env, jaoapdevice, "JAOAPDevice"){

    JLibUsbDevice::Pointer jLibUsbDevice = JLibUsbDevice::getJLibUsbDevice(env, jlibusbdevice);

    aoapDevice_ = AOAPDevice::create(jLibUsbDevice->getLibUsbDevice(), JRuntime::ioService());
}

JAOAPDevice::Pointer JAOAPDevice::getJAOAPDevice(JNIEnv *env, jobject jaoapDevice) {
    return (JAOAPDevice::Pointer)env->GetLongField(jaoapDevice, JAOAPDevice::handleId);;
}

void JAOAPDevice::create(LibUsbDevice::Pointer libUsbDevice, boost::asio::io_service& ioService) {
    aoapDevice_ = AOAPDevice::create(libUsbDevice, ioService);
}

IAOAPDevice::Pointer JAOAPDevice::getAOAPDevice() {
    return aoapDevice_;
}

JAOAPDevice::~JAOAPDevice() {
    delete aoapDevice_;
//    aoapDevice_.reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_usb_AOAPDevice_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/usb/AOAPDevice");
    JAOAPDevice::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_usb_AOAPDevice_nativeSetup(JNIEnv *env, jobject thiz, jobject jlibusbdevice) {
    JAOAPDevice::Pointer jaoapDevice = new JAOAPDevice(env, thiz, jlibusbdevice);
    return (jlong)((size_t)jaoapDevice);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_usb_AOAPDevice_nativeDelete(JNIEnv *env, jobject thiz) {
    JAOAPDevice::Pointer jaoapDevice = JAOAPDevice::getJAOAPDevice(env, thiz);
    delete jaoapDevice;
}