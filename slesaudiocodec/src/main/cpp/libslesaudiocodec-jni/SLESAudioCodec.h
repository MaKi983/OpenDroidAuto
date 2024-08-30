#pragma once

#include <sys/types.h>
#include <JNIBase.h>
#include <thread>
#include <android/looper.h>
#include "SLESStream.h"
#include <Data.hpp>
#include <queue>
#include <atomic_queue/atomic_queue.h>

class SLESAudioCodec {
public:
    enum {
        // From utils/threads.h

        /* all normal audio threads */
        ANDROID_PRIORITY_AUDIO          = -16,
    };


    typedef SLESAudioCodec* Pointer;
    static constexpr common::Data::size_type cChunkSize = 38406;

    SLESAudioCodec(int sampleRate, int channelConfig, int sampleSize);

    void openAudioDevice();
    void closeAudioDevice();
    void queueBuffer(common::DataConstBuffer& b, int64_t timestamp);
    void shutdown();
    void start();
    void stop();

//
//    void setSps(unsigned char* buf, int len);

    ~SLESAudioCodec();

private:
    void init_decoderThread();
//    void init_feedBufferThread();
//    static int looperCallback(int fd, int events, void* data);
    static void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

    void init();
    SLresult createEngine();
    void destroyEngine();
    SLresult createAudioPlayer();
    void feedBuffer(Packet::Pointer packet);
    Packet::Pointer nextBuffer();
    bool streamAudio();

//    std::thread decoderThread_;
//    // decoder lock
//    std::mutex dmutex_;
//    std::condition_variable dcond_;
//    // player lock
//    std::mutex pmutex_;
//    std::condition_variable pcond_;
//
//    bool decoderThreadRunning_;
//    bool decoderThreadInitialized_;
//    bool decoderThreadQuitFlag_;
//    bool playerReady_;
//    bool isRunning_;

    atomic_queue::AtomicQueueB<Packet::Pointer> pbuffers_;
    Packet::Pointer curPacket_;

//    int idx_;
//    int curIdx_;
//    int64_t curTs_;
//    int curSize_;
    SLESStream* slesStream_;
    int sampleRate_;
    int channelConfig_;
    int sampleSize_;
};
