#pragma once

#include <JNIBase.h>
#include <tcp/ITCPEndpoint.hpp>
#include <tcp/TCPWrapper.hpp>

using namespace aasdk::tcp;

class JTCPEndpoint : JNIBase {
public:
    typedef JTCPEndpoint* Pointer;

    JTCPEndpoint(JNIEnv *env, jobject jaoapDevice, std::string ipAddress);
    ~JTCPEndpoint();

    static jfieldID handleId;
    static JTCPEndpoint::Pointer getJTCPEndpoint(JNIEnv* env, jobject jTCPEndpoint);

    ITCPEndpoint::Pointer getTCPEnpoint();
private:
    ITCPEndpoint::SocketPointer connect(JNIEnv* env, boost::asio::io_service& ioService, std::string ipAddress);
    void throwConnectException(JNIEnv *env, char *message );

    TCPWrapper tcpWrapper_;
    ITCPEndpoint::Pointer tcpEndpoint_;
};
