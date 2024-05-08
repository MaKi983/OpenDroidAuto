#pragma once

#include <jni.h>

using namespace aasdk::usb;

class JLibUsb : JNIBase {
public:
    typedef JLibUsb* Pointer;

    JLibUsb(JNIEnv *env, jobject jlibusb);

    static jfieldID handleId;
    static JLibUsb::Pointer getJLibUsb(JNIEnv* env, jobject jlibusb);

    LibUsbDevice::Pointer createDevice();

private:
    LibUsb::Pointer libUsb_;
};

