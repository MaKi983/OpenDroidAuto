#pragma once

#include <JNIBase.h>
#include <transport/ITransport.hpp>

using namespace aasdk::messenger;

class JMessenger : JNIBase {
public:
    typedef JMessenger* Pointer;

    JMessenger(JNIEnv *env, jobject jmessenger, aasdk::transport::ITransport::Pointer transport, jobject jcryptor);
    ~JMessenger();

    static jfieldID handleId;
    static JMessenger::Pointer getJMessenger(JNIEnv* env, jobject jmessenger);

    void stop();
    IMessenger::Pointer getMessenger();
private:
    IMessenger::Pointer messenger_;

};
