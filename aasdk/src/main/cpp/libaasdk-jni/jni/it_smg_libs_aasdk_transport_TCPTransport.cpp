#include "it_smg_libs_aasdk_transport_TCPTransport.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_tcp_TCPEndpoint.h"

jfieldID JTCPTransport::handleId = nullptr;

JTCPTransport::JTCPTransport(JNIEnv *env, jobject jtcptransport, jobject jtcpendpoint)
        : JNIBase(env, jtcptransport, "JTCPTransport"){

    JTCPEndpoint::Pointer jTCPEndpoint = JTCPEndpoint::getJTCPEndpoint(env, jtcpendpoint);

    transport_ = std::make_shared<TCPTransport>(JRuntime::ioService(), jTCPEndpoint->getTCPEnpoint());
}

JTCPTransport::Pointer JTCPTransport::getJTCPTransport(JNIEnv *env, jobject jtcptransport) {
    return (JTCPTransport::Pointer)env->GetLongField(jtcptransport, JTCPTransport::handleId);
}

ITransport::Pointer JTCPTransport::getTransport() {
    return transport_;
}

JTCPTransport::~JTCPTransport() {
//    transport_.reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_transport_TCPTransport_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/transport/TCPTransport");
    JTCPTransport::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_transport_TCPTransport_nativeSetup(JNIEnv *env, jobject thiz, jobject jtcpendpoint) {
    auto jtcptransport = new JTCPTransport(env, thiz, jtcpendpoint);
    return (jlong)((size_t)jtcptransport);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_transport_TCPTransport_nativeStop(JNIEnv *env, jobject thiz) {
    JTCPTransport::Pointer jTCPTransport = JTCPTransport::getJTCPTransport(env, thiz);
    jTCPTransport->getTransport()->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_transport_TCPTransport_nativeDelete(JNIEnv *env, jobject thiz) {
    JTCPTransport::Pointer jTCPTransport = JTCPTransport::getJTCPTransport(env, thiz);
    delete jTCPTransport;
}