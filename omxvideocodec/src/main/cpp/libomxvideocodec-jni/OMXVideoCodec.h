#pragma once

#include <jni.h>
#include <sys/types.h>
#include "OMXSource.h"
#include "OMXDecoder.h"
#include <JNIBase.h>
#include <thread>
#include <android/looper.h>

struct ScreenSize {
    int height = 0;
    int width = 0;
};

class OMXVideoCodec : JNIBase {
public:
    typedef OMXVideoCodec* Pointer;

    OMXVideoCodec(JNIEnv *env, jobject androidApp);

    status_t init(int fps);
    void shutdown();
    void setNativeWindow(ANativeWindow* nativeWindow, int height, int width);

    OMXSource::Pointer source();
    OMXDecoder::Pointer decoder();

    void setSps(unsigned char* buf, int len);

    ~OMXVideoCodec();

protected:
    void initJavaMethods() { };

private:
    void init_omxDecoderThread();

    OMXSource::Pointer omxSource_;
    OMXDecoder::Pointer omxDecoder_;

    std::thread omxDecoderThread_;
    std::mutex codecMutex_;
    std::condition_variable omxDecoderCond_;

    bool omxDecoderThreadRunning_;
    bool decoderThreadInitialized_;
    bool decoderThreadQuitFlag_;
    sp<ANativeWindow> nativeWindow_;
    ScreenSize screenSize_;
    int fps_;
};
