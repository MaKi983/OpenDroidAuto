#include "LibUsb.h"
#include <JNIBase.h>
#include <Log.h>
#include <jni.h>

namespace aasdk {
namespace usb {

LibUsb::LibUsb() :
    ctx_(nullptr){

    libusb_set_option(ctx_, LIBUSB_OPTION_NO_DEVICE_DISCOVERY, NULL);
    if(libusb_init(&ctx_) != LIBUSB_SUCCESS)
    {
        Log_e("libusb init failed");
    }
    libusb_set_option(ctx_, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO); //LIBUSB_LOG_LEVEL_DEBUG
    if(Log::isVerbose()) Log_v("usb context inizialized %p", ctx_);

    initWorker();
}

void LibUsb::initWorker(){
    flagQuit_ = false;
    worker_ = std::thread([this]() {
        // Attach jni thread
        JNIEnv* env;
        JNIBase::javaAttachThread("LibUsb Worker", &env);

        timeval libusbEventTimeout{1, 0};
        int completed = 0;

        while(!flagQuit_ && !completed)
        {
            int ret1 = libusb_handle_events_timeout_completed(ctx_, &libusbEventTimeout, &completed);
//            if(Log::isVerbose()) Log_v("libusb_handle_events_timeout_completed events completed: %s (%d)", libusb_error_name(ret1), ret1);
        }

        // Detach thread
        JNIBase::javaDetachThread();

        if(Log::isVerbose()) Log_v("usbWorker stopped");
    });
}

LibUsbDevice::Pointer LibUsb::createDevice(){
    LibUsbDevice* libUsbDevice = new LibUsbDevice(ctx_);

    return libUsbDevice;
}

}
}
