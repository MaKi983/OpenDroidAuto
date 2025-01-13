#pragma once

#include <sys/types.h>
#include "OMXSource.h"
#include "OMXDecoder.h"
#include <thread>
#include <android/looper.h>

struct ScreenSize {
    int height = 0;
    int width = 0;
};

class OMXVideoCodec {
public:
    typedef OMXVideoCodec* Pointer;

    OMXVideoCodec();

    status_t init(int fps);
    void shutdown();
    void setNativeWindow(ANativeWindow* nativeWindow, int height, int width);

    OMXSource::Pointer source();
    OMXDecoder::Pointer decoder();
    void queueBuffer(common::DataConstBuffer& b, int64_t timestamp);

    void setSps(common::DataConstBuffer &b);

    ~OMXVideoCodec();
private:
    void init_omxDecoderThread();

    OMXSource::Pointer omxSource_;
    OMXDecoder::Pointer omxDecoder_;

    std::thread omxDecoderThread_;
    std::mutex codecMutex_;
    std::condition_variable omxDecoderCond_;

//    bool omxDecoderThreadRunning_;
    bool decoderThreadInitialized_;
    bool decoderThreadQuitFlag_;
    sp<ANativeWindow> nativeWindow_;
    ScreenSize screenSize_;
    int fps_;
};
