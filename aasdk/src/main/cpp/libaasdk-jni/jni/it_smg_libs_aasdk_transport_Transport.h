#pragma once

#include <jni.h>
#include <transport/ITransport.hpp>

using namespace aasdk::transport;

class JTransport {
public:
    typedef JTransport* Pointer;

    static jfieldID handleId;
    static JTransport::Pointer getJTransport(JNIEnv* env, jobject jtransport);

    ITransport::Pointer getTransport();
protected:
    ITransport::Pointer transport_;
};