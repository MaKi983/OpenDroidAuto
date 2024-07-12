#include <tcp/TCPEndpoint.hpp>
#include <Log.h>
#include "it_smg_libs_aasdk_tcp_TCPEndpoint.h"
#include "it_smg_libs_aasdk_Runtime.h"

using namespace aasdk::tcp;

jfieldID JTCPEndpoint::handleId = nullptr;

JTCPEndpoint::JTCPEndpoint(JNIEnv *env, jobject jtcpendpoint, std::string ipAddress)
        : JNIBase(env, jtcpendpoint, "JTCPEndpoint"){
    ITCPEndpoint::SocketPointer socket = connect(env, JRuntime::ioService(), ipAddress);
    tcpEndpoint_ = std::make_shared<TCPEndpoint>(tcpWrapper_, socket);
}

JTCPEndpoint::~JTCPEndpoint(){
//    tcpEndpoint_.reset();
}

JTCPEndpoint::Pointer JTCPEndpoint::getJTCPEndpoint(JNIEnv *env, jobject jtcpendpoint) {
    return (JTCPEndpoint::Pointer)env->GetLongField(jtcpendpoint, JTCPEndpoint::handleId);;
}

ITCPEndpoint::SocketPointer JTCPEndpoint::connect(JNIEnv* env, boost::asio::io_service& ioService, std::string ipAddress){
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioService);

    try {
        boost::system::error_code ec = tcpWrapper_.connect(*socket, ipAddress, 5277);

        if (!ec){
            if(Log::isInfo()) Log_i("Tcp connection established to ipAddress: %s", ipAddress.c_str());
            return socket;
        } else {
            char msg [512] = {0};
            snprintf (msg, sizeof (msg), "Error in tcp connection, ip: %s -> error: %s", ipAddress.c_str(), ec.message().c_str());
            Log_e(msg);
            socket.reset();
            throwConnectException(env, msg);
            return nullptr;
        }

    } catch(const boost::system::system_error& se) {
        char msg [512] = {0};
        snprintf (msg, sizeof (msg), "Error in tcp connection, ip: %s -> error: %s", ipAddress.c_str(), se.what());
        Log_e(msg);
        socket.reset();
        throwConnectException(env, msg);
        return nullptr;
    }
}

void JTCPEndpoint::throwConnectException(JNIEnv *env, char *message ){
    jclass exClass = env->FindClass("it/smg/libs/aasdk/tcp/TCPConnectException");
    env->ThrowNew(exClass, message);

    env->DeleteLocalRef(exClass);
    exClass = nullptr;
}

ITCPEndpoint::Pointer JTCPEndpoint::getTCPEnpoint() {
    return tcpEndpoint_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_tcp_TCPEndpoint_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/tcp/TCPEndpoint");
    JTCPEndpoint::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_tcp_TCPEndpoint_nativeSetup(JNIEnv *env, jobject thiz, jstring jipaddress) {
    const char* ip_ = env->GetStringUTFChars(jipaddress, NULL);
    std::string ipAddress(ip_);
    env->ReleaseStringUTFChars(jipaddress, ip_);

    JTCPEndpoint::Pointer jTCPEndpoint = new JTCPEndpoint(env, thiz, ipAddress);
    return (jlong)((size_t)jTCPEndpoint);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_tcp_TCPEndpoint_nativeDelete(JNIEnv *env, jobject thiz) {
    JTCPEndpoint::Pointer jTCPEndpoint = JTCPEndpoint::getJTCPEndpoint(env, thiz);
    delete jTCPEndpoint;
}