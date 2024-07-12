#include <transport/ISSLWrapper.hpp>
#include <messenger/Cryptor.hpp>
#include <transport/SSLWrapper.hpp>
#include "it_smg_libs_aasdk_messenger_Cryptor.h"
#include "it_smg_libs_aasdk_transport_Transport.h"
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JCryptor::handleId = nullptr;

JCryptor::JCryptor(JNIEnv *env, jobject jcryptor)
        : JNIBase(env, jcryptor, "JCryptor")  {

    aasdk::transport::ISSLWrapper::Pointer sslWrapper = std::make_unique<aasdk::transport::SSLWrapper>();
    cryptor_ = std::make_unique<Cryptor>(sslWrapper);
    cryptor_->init();
}

JCryptor::~JCryptor() {
//    cryptor_.reset();
}

JCryptor::Pointer JCryptor::getJCryptor(JNIEnv *env, jobject jcryptor) {
    return (JCryptor::Pointer)env->GetLongField(jcryptor, JCryptor::handleId);;
}

ICryptor::Pointer JCryptor::getCryptor() {
    return cryptor_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_messenger_Cryptor_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/messenger/Cryptor");
    JCryptor::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_messenger_Cryptor_nativeSetup(JNIEnv *env, jobject thiz) {
    JCryptor::Pointer jCryptor = new JCryptor(env, thiz);
    return (jlong)((size_t)jCryptor);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_messenger_Cryptor_nativeDelete(JNIEnv *env, jobject thiz) {
    JCryptor::Pointer jCryptor = JCryptor::getJCryptor(env, thiz);
    delete jCryptor;
}
