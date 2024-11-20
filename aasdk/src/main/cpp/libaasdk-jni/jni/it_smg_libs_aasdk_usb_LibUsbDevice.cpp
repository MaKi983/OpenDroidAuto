#include <usb/LibUsb.h>
#include <usb/LibUsbDevice.h>
#include "it_smg_libs_aasdk_usb_LibUsbDevice.h"
#include "it_smg_libs_aasdk_usb_LibUsb.h"

jfieldID JLibUsbDevice::handleId = nullptr;

JLibUsbDevice::JLibUsbDevice(JNIEnv *env, jobject jlibusbDevice, jobject jlibusb)
        : JNIBase(env, jlibusbDevice, "JLibUsbDevice"){

    JLibUsb::Pointer jLibUsb = JLibUsb::getJLibUsb(env, jlibusb);
    libUsbDevice_ = jLibUsb->createDevice();
}

JLibUsbDevice::Pointer JLibUsbDevice::getJLibUsbDevice(JNIEnv *env, jobject jlibusbDevice) {
    return (JLibUsbDevice::Pointer)env->GetLongField(jlibusbDevice, JLibUsbDevice::handleId);
}

int JLibUsbDevice::open(int fd) {
    return libUsbDevice_->open(fd);
}

void JLibUsbDevice::close() {
    libUsbDevice_->close();
}

int JLibUsbDevice::deviceDescriptor(libusb_device_descriptor &descriptor) {
    return libUsbDevice_->deviceDescriptor(descriptor);
}

unsigned char* JLibUsbDevice::manufacter() {
    return libUsbDevice_->manufacter;
}

unsigned char* JLibUsbDevice::product() {
    return libUsbDevice_->product;
}

unsigned char* JLibUsbDevice::serial() {
    return libUsbDevice_->serial;
}

LibUsbDevice::Pointer JLibUsbDevice::getLibUsbDevice() {
    return libUsbDevice_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/usb/LibUsbDevice");
    JLibUsbDevice::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeSetup(JNIEnv *env, jobject thiz, jobject jlibUsb) {
    auto jlibusbdevice = new JLibUsbDevice(env, thiz, jlibUsb);
    return (jlong)((size_t)jlibusbdevice);
}

extern "C"
JNIEXPORT jint JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeOpen(JNIEnv *env, jobject thiz, jint fd) {
    JLibUsbDevice::Pointer jlibUsbDevice = JLibUsbDevice::getJLibUsbDevice(env, thiz);
    return jlibUsbDevice->open(fd);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeClose(JNIEnv *env, jobject thiz) {
    JLibUsbDevice::Pointer jlibUsbDevice = JLibUsbDevice::getJLibUsbDevice(env, thiz);
    jlibUsbDevice->close();
}

extern "C"
JNIEXPORT jint JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeDeviceDescriptor(JNIEnv *env, jobject thiz) {
    JLibUsbDevice::Pointer jlibUsbDevice = JLibUsbDevice::getJLibUsbDevice(env, thiz);

    libusb_device_descriptor desc;
    int result = jlibUsbDevice->deviceDescriptor(desc);

    return result;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeManufacter(JNIEnv *env, jobject thiz) {
    JLibUsbDevice::Pointer jlibUsbDevice = JLibUsbDevice::getJLibUsbDevice(env, thiz);

	int size = strlen((char*)jlibUsbDevice->manufacter());
    jbyteArray data = env->NewByteArray(size);
    env->SetByteArrayRegion (data, 0, size, reinterpret_cast<const jbyte *>(jlibUsbDevice->manufacter()));
    return data;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeProduct(JNIEnv *env, jobject thiz) {
    JLibUsbDevice::Pointer jlibUsbDevice = JLibUsbDevice::getJLibUsbDevice(env, thiz);

    int size = strlen((char*)jlibUsbDevice->product());
    jbyteArray data = env->NewByteArray(size);
    env->SetByteArrayRegion (data, 0, size, reinterpret_cast<const jbyte *>(jlibUsbDevice->product()));
    return data;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_it_smg_libs_aasdk_usb_LibUsbDevice_nativeSerial(JNIEnv *env, jobject thiz) {
    JLibUsbDevice::Pointer jlibUsbDevice = JLibUsbDevice::getJLibUsbDevice(env, thiz);

    int size = strlen((char*)jlibUsbDevice->serial());
    jbyteArray data = env->NewByteArray(size);
    env->SetByteArrayRegion (data, 0, size, reinterpret_cast<const jbyte *>(jlibUsbDevice->serial()));
    return data;
}