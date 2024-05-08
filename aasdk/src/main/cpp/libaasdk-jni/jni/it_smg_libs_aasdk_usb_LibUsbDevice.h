#pragma once

#include <JNIBase.h>

using namespace aasdk::usb;

class JLibUsbDevice : JNIBase {
public:
    typedef JLibUsbDevice* Pointer;

    static jfieldID handleId;
    static JLibUsbDevice::Pointer getJLibUsbDevice(JNIEnv* env, jobject jlibusbDevice);

    JLibUsbDevice(JNIEnv *env, jobject jlibusbDevice, jobject jlibusb);

    int open(int fd);
    void close();
    int deviceDescriptor(libusb_device_descriptor &descriptor);
    LibUsbDevice::Pointer getLibUsbDevice();

    unsigned char* manufacter();
    unsigned char* product();
    unsigned char* serial();
private:
    LibUsbDevice::Pointer libUsbDevice_;
};
