#include <Log.h>
#include "it_smg_libs_aasdk_projection_VideoOutput.h"

using namespace projection;

jfieldID JVideoOutput::handleId = nullptr;

JVideoOutput::JVideoOutput(JNIEnv *env, jobject jvideooutput) :
        JNIBase(env, jvideooutput, "JVideoOutput"){

    initJavaMethods();
}

JVideoOutput::Pointer JVideoOutput::getJVideoOutput(JNIEnv *env, jobject jvideooutput) {
    return (JVideoOutput::Pointer)env->GetLongField(jvideooutput, JVideoOutput::handleId);;
}

void JVideoOutput::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    openMethodId_ = env->GetMethodID(cls, "open", "()Z");
    initMethodId_ = env->GetMethodID(cls, "init", "()Z");
    writeMethodId_ = env->GetMethodID(cls, "write", "(JLjava/nio/ByteBuffer;)V" );
    stopMethodId_ = env->GetMethodID(cls, "stop", "()V");
    getVideoFPSMethodId_ = env->GetMethodID(cls, "getVideoFPS", "()I");
    getVideoResolutionMethodId_ = env->GetMethodID(cls, "getVideoResolution", "()I");
    getScreenDPIMethodId_ = env->GetMethodID(cls, "getScreenDPI", "()I");
    getVideoMarginsMethodId_ = env->GetMethodID(cls, "getVideoMargins", "()Lit/smg/libs/aasdk/projection/IVideoOutput$Rect;");

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

bool JVideoOutput::open() {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, openMethodId_);
    return ret == JNI_TRUE;
}

bool JVideoOutput::init() {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, initMethodId_);
    return ret == JNI_TRUE;
}

void JVideoOutput::stop() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, stopMethodId_);
}

aasdk::proto::enums::VideoFPS::Enum JVideoOutput::getVideoFPS() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getVideoFPSMethodId_);
    //TODO: try to cast
    switch (ret) {
        case aasdk::proto::enums::VideoFPS_Enum__60:
            return aasdk::proto::enums::VideoFPS_Enum__60;
        default:
            return aasdk::proto::enums::VideoFPS_Enum__30;
    }
}

aasdk::proto::enums::VideoResolution::Enum JVideoOutput::getVideoResolution() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getVideoResolutionMethodId_);
    switch (ret) {
        case aasdk::proto::enums::VideoResolution_Enum__720p:
            return aasdk::proto::enums::VideoResolution_Enum__720p;
        case aasdk::proto::enums::VideoResolution_Enum__1080p:
            return aasdk::proto::enums::VideoResolution_Enum__1080p;
        case aasdk::proto::enums::VideoResolution_Enum__1440p:
            return aasdk::proto::enums::VideoResolution_Enum__1440p;
        case aasdk::proto::enums::VideoResolution_Enum__720p_p:
            return aasdk::proto::enums::VideoResolution_Enum__720p_p;
        case aasdk::proto::enums::VideoResolution_Enum__1080pp:
            return aasdk::proto::enums::VideoResolution_Enum__1080pp;
        case aasdk::proto::enums::VideoResolution_Enum__108s0p_p:
            return aasdk::proto::enums::VideoResolution_Enum__108s0p_p;
        default:
            return aasdk::proto::enums::VideoResolution_Enum__480p;
    }
}

size_t JVideoOutput::getScreenDPI() {
    JNIEnv* env = getJniEnv();
    jint ret = env->CallIntMethod(androidClass_, getScreenDPIMethodId_);
    return ret;
}

Rect JVideoOutput::getVideoMargins() {
    JNIEnv* env = getJniEnv();
    jobject ret = env->CallObjectMethod(androidClass_, getVideoMarginsMethodId_);

    jclass rectClass = env->GetObjectClass(ret);
    jfieldID widthFieldId = env->GetFieldID(rectClass,"width","I");
    jfieldID heightFieldId = env->GetFieldID(rectClass,"height","I");

    jint width = env->GetIntField(ret, widthFieldId);
    jint height = env->GetIntField(ret, heightFieldId);

    Rect geom;
    geom.width = width;
    geom.height = height;

    env->DeleteLocalRef(ret);
    ret = nullptr;

    env->DeleteLocalRef(rectClass);
    rectClass = nullptr;

    return geom;
}

void JVideoOutput::write(uint64_t timestamp, const aasdk::common::DataConstBuffer& buffer) {
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

}

JVideoOutput::~JVideoOutput() {
    JNIEnv* env = getJniEnv();

    env->DeleteGlobalRef(byteBufferCls_);
    env->DeleteGlobalRef(bbNativeByteOrderObj_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_VideoOutput_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/projection/VideoOutput");
    JVideoOutput::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_projection_VideoOutput_nativeSetup(JNIEnv *env, jobject thiz) {
    auto jVideoOutput = new JVideoOutput(env, thiz);
    return (jlong)((size_t)jVideoOutput);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_VideoOutput_nativeDelete(JNIEnv *env, jobject thiz) {
    JVideoOutput::Pointer jVideoOutput = JVideoOutput::getJVideoOutput(env, thiz);
    delete jVideoOutput;
}