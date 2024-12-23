#pragma once

#include <JNIBase.h>
#include <usb/AOAPDevice.hpp>

using namespace aasdk::usb;

class JAOAPDevice : JNIBase {
public:
    typedef JAOAPDevice* Pointer;

    JAOAPDevice(JNIEnv *env, jobject jaoapDevice, jobject jlibusbdevice);
    ~JAOAPDevice();

    static jfieldID handleId;
    static JAOAPDevice::Pointer getJAOAPDevice(JNIEnv* env, jobject jaoapDevice);

    void create(LibUsbDevice::Pointer libUsbDevice, aasdk::io::ioService& ioService);
    IAOAPDevice::Pointer getAOAPDevice();

private:
    IAOAPDevice::Pointer aoapDevice_;
};