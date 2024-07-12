#pragma once

#include <JNIBase.h>
#include "SLESAudioCodec.h"

class JSLESAudioCodec : JNIBase {
public:
    typedef JSLESAudioCodec* Pointer;

    JSLESAudioCodec(JNIEnv *env, jobject jaudiooutput, int sampleRate, int channelConfig, int sampleSize);

    static jfieldID handleId;
    static JSLESAudioCodec::Pointer getJSLESAudioCodec(JNIEnv* env, jobject jaudiooutput);

    void queueBuffer(common::DataConstBuffer& b, int64_t timestamp);
    void stop();

    ~JSLESAudioCodec();

protected:
    void initJavaMethods() override;

private:
    jmethodID startMethodId_;
    jmethodID stopMethodId_;
    jmethodID suspendMethodId_;
    jmethodID openMethodId_;
    jmethodID writeMethodId_;
    jmethodID getSampleSizeMethodId_;
    jmethodID getChannelCountMethodId_;
    jmethodID getSampleRateMethodId_;

    SLESAudioCodec::Pointer slesAudioCodec_;
};
