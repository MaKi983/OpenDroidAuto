#pragma once

#include <thread>
#include <JNIBase.h>
#include <libusb.h>
#include "LibUsbDevice.h"

namespace aasdk {
namespace usb {

class LibUsb {

public:
    typedef LibUsb* Pointer;

    LibUsb();
    LibUsbDevice::Pointer createDevice();

private:
    void initWorker();

    libusb_context* ctx_;
    std::thread worker_;

    std::atomic_bool flagQuit_;
};

}
}
