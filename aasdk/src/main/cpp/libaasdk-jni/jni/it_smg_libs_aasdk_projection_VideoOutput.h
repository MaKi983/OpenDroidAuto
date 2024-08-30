#pragma once

#include <JNIBase.h>
#include <projection/IVideoOutput.h>

namespace projection {

class JVideoOutput : JNIBase, public IVideoOutput {
public:
    typedef JVideoOutput* Pointer;

    JVideoOutput(JNIEnv *env, jobject jvideooutput);

    static jfieldID handleId;
    static JVideoOutput::Pointer getJVideoOutput(JNIEnv* env, jobject jvideooutput);

    void initJavaMethods() override;

    bool open() override;
    bool init() override;
    void write(uint64_t timestamp, const aasdk::common::DataConstBuffer& buffer) override;
    void stop() override;
    aasdk::proto::enums::VideoFPS::Enum getVideoFPS() override;
    aasdk::proto::enums::VideoResolution::Enum getVideoResolution() override;
    size_t getScreenDPI() override;
    Rect getVideoMargins() override;

    ~JVideoOutput();
private:
    jmethodID openMethodId_;
    jmethodID initMethodId_;
    jmethodID writeMethodId_;
    jmethodID stopMethodId_;
    jmethodID getVideoFPSMethodId_;
    jmethodID getVideoResolutionMethodId_;
    jmethodID getScreenDPIMethodId_;
    jmethodID getVideoMarginsMethodId_;

    jclass byteBufferCls_;
    jmethodID bbOrderID_;
    jobject bbNativeByteOrderObj_;
};

}
