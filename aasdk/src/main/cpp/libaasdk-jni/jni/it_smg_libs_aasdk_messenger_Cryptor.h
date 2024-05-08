#pragma once

#include <JNIBase.h>

using namespace aasdk::messenger;

class JCryptor : JNIBase {
public:
    typedef JCryptor* Pointer;

    JCryptor(JNIEnv *env, jobject jcryptor);
    ~JCryptor();

    static jfieldID handleId;
    static JCryptor::Pointer getJCryptor(JNIEnv* env, jobject jcryptor);

    ICryptor::Pointer getCryptor();
private:
    ICryptor::Pointer cryptor_;

};

