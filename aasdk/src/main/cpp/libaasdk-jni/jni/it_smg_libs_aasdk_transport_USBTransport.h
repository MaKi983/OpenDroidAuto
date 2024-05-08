#pragma once

#include <JNIBase.h>
#include <transport/USBTransport.hpp>

using namespace aasdk::transport;

class JUSBTransport : JNIBase {
public:
    typedef JUSBTransport* Pointer;

    JUSBTransport(JNIEnv *env, jobject jusbtransport, jobject jaoapdevice);
    ~JUSBTransport();

    static jfieldID handleId;
    static JUSBTransport::Pointer getJUSBTransport(JNIEnv* env, jobject jusbtransport);
    ITransport::Pointer getTransport();

private:
    ITransport::Pointer transport_;
};
