#include <messenger/Timestamp.hpp>
#include "it_smg_libs_aasdk_projection_AudioInput.h"

using namespace projection;

jfieldID JAudioInput::handleId = nullptr;

JAudioInput::JAudioInput(JNIEnv *env, jobject jaudioinput) :
        JNIBase(env, jaudioinput, "JAudioInput"){

    initJavaMethods();
}

JAudioInput::Pointer JAudioInput::getJAudioInput(JNIEnv *env, jobject jaudioinput) {
    return (JAudioInput::Pointer)env->GetLongField(jaudioinput, JAudioInput::handleId);;
}

void JAudioInput::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    stopMethodId_ = env->GetMethodID(cls, "stop", "()V");
    readMethodId_ = env->GetMethodID(cls, "read", "(Ljava/nio/ByteBuffer;)I");
    openMethodId_ = env->GetMethodID(cls, "open", "()Z");
    isActiveMethodId_ = env->GetMethodID(cls, "isActive", "()Z" );
    startMethodId_ = env->GetMethodID(cls, "start", "()Z");
    getSampleSizeMethodId_ = env->GetMethodID(cls, "getSampleSize", "()I");
    getChannelCountMethodId_ = env->GetMethodID(cls, "getChannelCount", "()I");
    getSampleRateMethodId_ = env->GetMethodID(cls, "getSampleRate", "()I");
    getBufferSizeMethodId_ = env->GetMethodID(cls, "getBufferSize", "()I");

    env->DeleteLocalRef(cls);
    cls = nullptr;
}

bool JAudioInput::open() {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, openMethodId_);
    if (ret == JNI_TRUE){
        bufferSize_ = getBufferSize();
        return true;
    }
    return false;
}

void JAudioInput::start(projection::IAudioInput::StartPromise::Pointer promise) {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, startMethodId_);
    if (ret == JNI_TRUE) {
        promise->resolve();
    } else {
        promise->reject();
    }
}

void JAudioInput::stop() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, stopMethodId_);
}

bool JAudioInput::isActive() {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, isActiveMethodId_);
    return ret == JNI_TRUE;
}

uint32_t JAudioInput::getSampleSize() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getSampleSizeMethodId_);
    return ret;
}

uint32_t JAudioInput::getChannelCount() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getChannelCountMethodId_);
    return ret;
}

uint32_t JAudioInput::getSampleRate() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getSampleRateMethodId_);
    return ret;
}

uint32_t JAudioInput::getBufferSize() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getBufferSizeMethodId_);
    return ret;
}

void JAudioInput::read(projection::IAudioInput::ReadPromise::Pointer promise) {
    JNIEnv* env = getJniEnv();

    jclass byteBufferClass = env->FindClass("java/nio/ByteBuffer");
    jmethodID orderID = env->GetMethodID(byteBufferClass, "order", "(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;");

    jclass byteOrderClass = env->FindClass("java/nio/ByteOrder");
    jmethodID nativeOrderID = env->GetStaticMethodID(byteOrderClass, "nativeOrder", "()Ljava/nio/ByteOrder;");

    jobject nativeByteOrderObj = env->CallStaticObjectMethod(byteOrderClass, nativeOrderID);

    aasdk::common::Data data(bufferSize_);
    aasdk::common::DataBuffer buffer(data);

    jobject byteBuffer = env->NewDirectByteBuffer(buffer.data, buffer.size);

    jobject me = env->CallObjectMethod(byteBuffer, orderID, nativeByteOrderObj);
    env->DeleteLocalRef(me);
    me = nullptr;

    jint read = env->CallIntMethod(androidClass_, readMethodId_, byteBuffer);

    env->DeleteLocalRef(byteBuffer);
    byteBuffer = nullptr;

    env->DeleteLocalRef(nativeByteOrderObj);
    nativeByteOrderObj = nullptr;

    env->DeleteLocalRef(byteBufferClass);
    byteBufferClass = nullptr;

    env->DeleteLocalRef(byteOrderClass);
    byteOrderClass = nullptr;

    if (read >= 0){
        promise->resolve(data);
    } else {
        promise->reject();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_AudioInput_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/projection/AudioInput");
    JAudioInput::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_projection_AudioInput_nativeSetup(JNIEnv *env, jobject thiz) {
    JAudioInput::Pointer jAudioInput = new JAudioInput(env, thiz);
    return (jlong)((size_t)jAudioInput);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_AudioInput_nativeDelete(JNIEnv *env, jobject thiz) {
    JAudioInput::Pointer jAudioInput = JAudioInput::getJAudioInput(env, thiz);
    delete jAudioInput;
}