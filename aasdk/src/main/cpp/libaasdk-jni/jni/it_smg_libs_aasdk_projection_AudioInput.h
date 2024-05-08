#pragma once

#include <JNIBase.h>
#include <projection/IAudioInput.h>

namespace projection {

    class JAudioInput : JNIBase, public IAudioInput {
    public:
        typedef JAudioInput* Pointer;

        JAudioInput(JNIEnv *env, jobject jaudioinput);

        static jfieldID handleId;
        static JAudioInput::Pointer getJAudioInput(JNIEnv* env, jobject jaudioinput);

        void initJavaMethods() override;

        bool open() override;
        bool isActive() override;
        void start(projection::IAudioInput::StartPromise::Pointer promise) override;
        void stop() override;
        void read(projection::IAudioInput::ReadPromise::Pointer promise) override;
        uint32_t getSampleSize() override;
        uint32_t getChannelCount() override;
        uint32_t getSampleRate() override;
        uint32_t getBufferSize() override;

        aasdk::common::DataBuffer getBuffer(int size);
        void commitData(int bytesRead);
    private:
        jmethodID startMethodId_;
        jmethodID stopMethodId_;
        jmethodID readMethodId_;
        jmethodID openMethodId_;
        jmethodID isActiveMethodId_;
        jmethodID getSampleSizeMethodId_;
        jmethodID getChannelCountMethodId_;
        jmethodID getSampleRateMethodId_;
        jmethodID getBufferSizeMethodId_;

        int bufferSize_;
    };

}
