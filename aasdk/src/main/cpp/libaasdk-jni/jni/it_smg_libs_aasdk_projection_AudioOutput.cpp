#include <Log.h>
#include "it_smg_libs_aasdk_projection_AudioOutput.h"

using namespace projection;

jfieldID JAudioOutput::handleId = nullptr;

JAudioOutput::JAudioOutput(JNIEnv *env, jobject jaudiooutput) :
    JNIBase(env, jaudiooutput, "JAudioOutput"){

    initJavaMethods();
}

JAudioOutput::Pointer JAudioOutput::getJAudioOutput(JNIEnv *env, jobject jaudiooutput) {
    return (JAudioOutput::Pointer)env->GetLongField(jaudiooutput, JAudioOutput::handleId);;
}

void JAudioOutput::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    stopMethodId_ = env->GetMethodID(cls, "stop", "()V");
    suspendMethodId_ = env->GetMethodID(cls, "suspend", "()V");
    openMethodId_ = env->GetMethodID(cls, "open", "()Z");
    writeMethodId_ = env->GetMethodID(cls, "write", "(JLjava/nio/ByteBuffer;)V" );
//    writeMethodId_ = env->GetMethodID(cls, "write", "(J[B)V" );
    startMethodId_ = env->GetMethodID(cls, "start", "()V");
    getSampleSizeMethodId_ = env->GetMethodID(cls, "getSampleSize", "()I");
    getChannelCountMethodId_ = env->GetMethodID(cls, "getChannelCount", "()I");
    getSampleRateMethodId_ = env->GetMethodID(cls, "getSampleRate", "()I");

    env->DeleteLocalRef(cls);

    jclass byteBufferClass = env->FindClass("java/nio/ByteBuffer");
    byteBufferCls_ = reinterpret_cast<jclass>(env->NewGlobalRef(byteBufferClass));
    bbOrderID_ = env->GetMethodID(byteBufferCls_, "order", "(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;");
    jclass byteOrderClass = env->FindClass("java/nio/ByteOrder");
    jmethodID boNativeOrderID_ = env->GetStaticMethodID(byteOrderClass, "nativeOrder", "()Ljava/nio/ByteOrder;");
    jobject nativeByteOrderObj = env->CallStaticObjectMethod(byteOrderClass, boNativeOrderID_);
    bbNativeByteOrderObj_ = env->NewGlobalRef(nativeByteOrderObj);

    env->DeleteLocalRef(byteBufferClass);
    env->DeleteLocalRef(byteOrderClass);
}

bool JAudioOutput::open() {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, openMethodId_);
    return ret == JNI_TRUE;
}

void JAudioOutput::start() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, startMethodId_);
}

void JAudioOutput::stop() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, stopMethodId_);
}

void JAudioOutput::suspend() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, suspendMethodId_);
}

uint32_t JAudioOutput::getSampleSize() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getSampleSizeMethodId_);
    return ret;
}

uint32_t JAudioOutput::getChannelCount() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getChannelCountMethodId_);
    return ret;
}

uint32_t JAudioOutput::getSampleRate() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getSampleRateMethodId_);
    return ret;
}

void JAudioOutput::write(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer &buffer) {
    JNIEnv* env = getJniEnv();

    jobject byteBuffer = env->NewDirectByteBuffer((uint8_t *)buffer.cdata, buffer.size);

    if (byteBuffer) {
        jobject me = env->CallObjectMethod(byteBuffer, bbOrderID_, bbNativeByteOrderObj_);
        env->DeleteLocalRef(me);
        me = nullptr;

        env->CallVoidMethod(androidClass_, writeMethodId_, timestamp, byteBuffer);

        env->DeleteLocalRef(byteBuffer);
        byteBuffer = nullptr;
    }

//    jbyteArray jbuf = env->NewByteArray(buffer.size);
//    env->SetByteArrayRegion(jbuf, 0, buffer.size, (const jbyte*)reinterpret_cast<const char*>(buffer.cdata));
//
//    env->CallVoidMethod(androidClass_, writeMethodId_, timestamp, jbuf, buffer.size);
//    env->DeleteLocalRef(jbuf);

}

JAudioOutput::~JAudioOutput() {
    JNIEnv* env = getJniEnv();

    env->DeleteGlobalRef(byteBufferCls_);
    env->DeleteGlobalRef(bbNativeByteOrderObj_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_AudioOutput_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/projection/AudioOutput");
    JAudioOutput::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
        Java_it_smg_libs_aasdk_projection_AudioOutput_nativeSetup(JNIEnv *env, jobject thiz) {
    auto jAudioOutput = new JAudioOutput(env, thiz);
    return (jlong)((size_t)jAudioOutput);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_AudioOutput_nativeDelete(JNIEnv *env, jobject thiz) {
    JAudioOutput::Pointer jAudioOutput = JAudioOutput::getJAudioOutput(env, thiz);
    delete jAudioOutput;
}