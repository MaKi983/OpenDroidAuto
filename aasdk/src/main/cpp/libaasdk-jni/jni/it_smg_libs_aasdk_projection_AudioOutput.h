#pragma once

#include <JNIBase.h>
#include <projection/IAudioOutput.h>

namespace projection {

class JAudioOutput : JNIBase, public IAudioOutput {
public:
    typedef JAudioOutput* Pointer;

    JAudioOutput(JNIEnv *env, jobject jaudiooutput);

    static jfieldID handleId;
    static JAudioOutput::Pointer getJAudioOutput(JNIEnv* env, jobject jaudiooutput);

    void initJavaMethods() override;

    bool open() override;
    void write(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer& buffer) override;
    void start() override;
    void stop() override;
    void suspend() override;
    uint32_t getSampleSize() override;
    uint32_t getChannelCount() override;
    uint32_t getSampleRate() override;

private:
    jmethodID startMethodId_;
    jmethodID stopMethodId_;
    jmethodID suspendMethodId_;
    jmethodID openMethodId_;
    jmethodID writeMethodId_;
    jmethodID getSampleSizeMethodId_;
    jmethodID getChannelCountMethodId_;
    jmethodID getSampleRateMethodId_;
};

}
