#include <messenger/Messenger.hpp>
#include <messenger/MessageInStream.hpp>
#include <messenger/MessageOutStream.hpp>
#include <transport/ISSLWrapper.hpp>
#include <messenger/Cryptor.hpp>
#include <transport/SSLWrapper.hpp>
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_transport_Transport.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_messenger_Cryptor.h"
#include "it_smg_libs_aasdk_transport_TCPTransport.h"
#include "it_smg_libs_aasdk_transport_USBTransport.h"

jfieldID JMessenger::handleId = nullptr;

JMessenger::JMessenger(JNIEnv *env, jobject jmessenger, aasdk::transport::ITransport::Pointer transport, jobject jcryptor)
    : JNIBase(env, jmessenger, "JMessenger")  {

    JCryptor::Pointer jCryptor = JCryptor::getJCryptor(env, jcryptor);

    IMessageInStream::Pointer messageInStream = std::make_shared<MessageInStream>(JRuntime::ioService(), transport, jCryptor->getCryptor());
    IMessageOutStream::Pointer messageOutStream = std::make_shared<MessageOutStream>(JRuntime::ioService(), transport, jCryptor->getCryptor());

    messenger_ = std::make_shared<Messenger>(JRuntime::ioService(), messageInStream, messageOutStream);
}

JMessenger::~JMessenger() {
//    messenger_.reset();
}

JMessenger::Pointer JMessenger::getJMessenger(JNIEnv *env, jobject jmessenger) {
    return (JMessenger::Pointer)env->GetLongField(jmessenger, JMessenger::handleId);;
}

void JMessenger::stop() {
    messenger_->stop();
}

IMessenger::Pointer JMessenger::getMessenger() {
    return messenger_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_messenger_Messenger_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/messenger/Messenger");
    JMessenger::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_messenger_Messenger_nativeSetupTCP(JNIEnv *env, jobject thiz, jobject jtcptransport, jobject jcryptor) {
    JTCPTransport::Pointer jTCPtransport = JTCPTransport::getJTCPTransport(env, jtcptransport);
    auto jMessenger = new JMessenger(env, thiz, jTCPtransport->getTransport(), jcryptor);
    return (jlong)((size_t)jMessenger);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_messenger_Messenger_nativeSetupUSB(JNIEnv *env, jobject thiz, jobject jusbtransport, jobject jcryptor) {
    JUSBTransport::Pointer jUSBtransport = JUSBTransport::getJUSBTransport(env, jusbtransport);
    auto jMessenger = new JMessenger(env, thiz, jUSBtransport->getTransport(), jcryptor);
    return (jlong)((size_t)jMessenger);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_messenger_Messenger_nativeStop(JNIEnv *env, jobject thiz) {
    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, thiz);
    jMessenger->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_messenger_Messenger_nativeDelete(JNIEnv *env, jobject thiz) {
    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, thiz);
    delete jMessenger;
}
