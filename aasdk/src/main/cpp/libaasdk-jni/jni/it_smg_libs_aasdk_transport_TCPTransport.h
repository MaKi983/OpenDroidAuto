#pragma once

#include <JNIBase.h>
#include <transport/TCPTransport.hpp>

using namespace aasdk::transport;

class JTCPTransport : JNIBase {
public:
    typedef JTCPTransport* Pointer;

    JTCPTransport(JNIEnv *env, jobject jtcptransport, jobject jtcpendpoint);
    ~JTCPTransport();

    static jfieldID handleId;
    static JTCPTransport::Pointer getJTCPTransport(JNIEnv* env, jobject jtcptransport);
    ITransport::Pointer getTransport();

private:
    ITransport::Pointer transport_;
};
