#include "SLESAudioCodec.h"
#include "Log.h"

#include <unistd.h>
#include <inttypes.h>
#include <sys/resource.h>

#define NUM_BUFFERS 5

SLESAudioCodec::SLESAudioCodec(int sampleRate, int channelConfig, int sampleSize):
    slesStream_(nullptr), decoderThreadRunning_(false), decoderThreadInitialized_(false), decoderThreadQuitFlag_(false),
    sampleRate_(sampleRate), sampleSize_(sampleSize), channelConfig_(channelConfig), idx_(0), curIdx_(0), curTs_(1), curSize_(0), playerReady_(true),
    isRunning_(false), curPacket_(nullptr)
{
    init();
}

SLESAudioCodec::~SLESAudioCodec() {
    Log_i("destructor");
}

void SLESAudioCodec::init(){
    if (Log::isInfo()) Log_i("start decoderThread");

    feedBufferThread_ = std::thread(&SLESAudioCodec::init_feedBufferThread, this);
    decoderThread_ = std::thread(&SLESAudioCodec::init_decoderThread, this);

    std::unique_lock<std::mutex> lock(dmutex_);
    while (!decoderThreadInitialized_){
        dcond_.wait(lock);
    }
    lock.unlock();
    Log_i("decoderThread ready");
}

// open the android audio device for input and/or output
void SLESAudioCodec::openAudioDevice() {
//    OPENSL_STREAM *p;
//    p = (OPENSL_STREAM *) calloc(sizeof(OPENSL_STREAM),1);
//
//    p->inchannels = inchannels;
//    p->outchannels = outchannels;
//    p->sr = sr;
//    p->inlock = createThreadLock();
//    p->outlock = createThreadLock();

    slesStream_ = new SLESStream();

//    if((p->outBufSamples = bufferframes*outchannels) != 0) {
//        if((p->outputBuffer[0] = (short *) calloc(p->outBufSamples, sizeof(short))) == NULL ||
//           (p->outputBuffer[1] = (short *) calloc(p->outBufSamples, sizeof(short))) == NULL) {
//            closeAudioDevice();
//            return;
//        }
//    }
//
//    if((p->inBufSamples = bufferframes*inchannels) != 0){
//        if((p->inputBuffer[0] = (short *) calloc(p->inBufSamples, sizeof(short))) == NULL ||
//           (p->inputBuffer[1] = (short *) calloc(p->inBufSamples, sizeof(short))) == NULL){
//            closeAudioDevice();
//            return;
//        }
//    }

//    p->currentInputIndex = 0;
//    p->currentOutputBuffer  = 0;
//    p->currentInputIndex = p->inBufSamples;
//    p->currentInputBuffer = 0;

    if(createEngine() != SL_RESULT_SUCCESS) {
        closeAudioDevice();
        return;
    }

//    if(openSLRecOpen(p) != SL_RESULT_SUCCESS) {
//        closeAudioDevice();
//        return;
//    }

    if(createAudioPlayer() != SL_RESULT_SUCCESS) {
        closeAudioDevice();
        return;
    }

//    notifyThreadLock(p->outlock);
//    notifyThreadLock(p->inlock);

//    p->time = 0.;
}

// close the android audio device
void SLESAudioCodec::closeAudioDevice() {
    isRunning_ = false;

    if (slesStream_ == nullptr)
        return;

    destroyEngine();

//    if (p->inlock != NULL) {
//        notifyThreadLock(p->inlock);
//        destroyThreadLock(p->inlock);
//        p->inlock = NULL;
//    }
//
//    if (p->outlock != NULL) {
//        notifyThreadLock(p->outlock);
//        destroyThreadLock(p->outlock);
//        p->inlock = NULL;
//    }
//
//    if (p->outputBuffer[0] != NULL) {
//        free(p->outputBuffer[0]);
//        p->outputBuffer[0] = NULL;
//    }
//
//    if (p->outputBuffer[1] != NULL) {
//        free(p->outputBuffer[1]);
//        p->outputBuffer[1] = NULL;
//    }
//
//    if (p->inputBuffer[0] != NULL) {
//        free(p->inputBuffer[0]);
//        p->inputBuffer[0] = NULL;
//    }
//
//    if (p->inputBuffer[1] != NULL) {
//        free(p->inputBuffer[1]);
//        p->inputBuffer[1] = NULL;
//    }

    delete slesStream_;
//    free(p);
}

// creates the OpenSL ES audio engine
SLresult SLESAudioCodec::createEngine() {
    SLresult result;
    // create engine
    result = slCreateEngine(&(slesStream_->engineObject), 0, NULL, 0, NULL, NULL);
    if(result != SL_RESULT_SUCCESS) {
        Log_e("create engine object error %d", result);
        return result;
    }

    // realize the engine
    result = (*slesStream_->engineObject)->Realize(slesStream_->engineObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS) {
        Log_e("create realize engine object error %d", result);
        return result;
    }

    // get the engine interface, which is needed in order to create other objects
    result = (*slesStream_->engineObject)->GetInterface(slesStream_->engineObject, SL_IID_ENGINE, &(slesStream_->engineEngine));
    if(result != SL_RESULT_SUCCESS) {
        Log_e("create get interface engine object error %d", result);
        return result;
    }

    return result;
}

// close the OpenSL IO and destroy the audio engine
void SLESAudioCodec::destroyEngine()
{
    // destroy buffer queue audio player object, and invalidate all associated interfaces
    if (slesStream_->bqPlayerObject != nullptr) {
        (*slesStream_->bqPlayerObject)->Destroy(slesStream_->bqPlayerObject);
        slesStream_->bqPlayerObject = nullptr;
        slesStream_->bqPlayerPlay = nullptr;
        (*slesStream_->bqPlayerBufferQueue)->Clear(slesStream_->bqPlayerBufferQueue);
        slesStream_->bqPlayerBufferQueue = nullptr;
        slesStream_->bqPlayerEffectSend = nullptr;
    }

//    // destroy audio recorder object, and invalidate all associated interfaces
//    if (p->recorderObject != NULL) {
//        (*p->recorderObject)->Destroy(p->recorderObject);
//        p->recorderObject = NULL;
//        p->recorderRecord = NULL;
//        p->recorderBufferQueue = NULL;
//    }

    // destroy output mix object, and invalidate all associated interfaces
    if (slesStream_->outputMixObject != nullptr) {
        (*slesStream_->outputMixObject)->Destroy(slesStream_->outputMixObject);
        slesStream_->outputMixObject = nullptr;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (slesStream_->engineObject != nullptr) {
        (*slesStream_->engineObject)->Destroy(slesStream_->engineObject);
        slesStream_->engineObject = nullptr;
        slesStream_->engineEngine = nullptr;
    }
}

// opens the OpenSL ES device for output
SLresult SLESAudioCodec::createAudioPlayer(){
    SLresult result = SL_RESULT_SUCCESS;
    SLuint32 sr = sampleRate_;
    SLuint32 channels = channelConfig_;

    if(channels) {
        // configure audio source
        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, NUM_BUFFERS};

        switch(sr){
            case 8000:
                sr = SL_SAMPLINGRATE_8;
                break;
            case 11025:
                sr = SL_SAMPLINGRATE_11_025;
                break;
            case 16000:
                sr = SL_SAMPLINGRATE_16;
                break;
            case 22050:
                sr = SL_SAMPLINGRATE_22_05;
                break;
            case 24000:
                sr = SL_SAMPLINGRATE_24;
                break;
            case 32000:
                sr = SL_SAMPLINGRATE_32;
                break;
            case 44100:
                sr = SL_SAMPLINGRATE_44_1;
                break;
            case 48000:
                sr = SL_SAMPLINGRATE_48;
                break;
            case 64000:
                sr = SL_SAMPLINGRATE_64;
                break;
            case 88200:
                sr = SL_SAMPLINGRATE_88_2;
                break;
            case 96000:
                sr = SL_SAMPLINGRATE_96;
                break;
            case 192000:
                sr = SL_SAMPLINGRATE_192;
                break;
            default:
                break;

                return -1;
        }

        const SLInterfaceID ids[] = {SL_IID_VOLUME};
        const SLboolean req[] = {SL_BOOLEAN_FALSE};
        result = (*slesStream_->engineEngine)->CreateOutputMix(slesStream_->engineEngine, &(slesStream_->outputMixObject), 1, ids, req);
        if(result != SL_RESULT_SUCCESS) {
            Log_e("create get interface engine object error %d", result);
            return result;
        }

        // realize the output mix
        result = (*slesStream_->outputMixObject)->Realize(slesStream_->outputMixObject, SL_BOOLEAN_FALSE);

        SLuint32 speakers;
        if(channels > 1) {
            speakers = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
        } else {
            speakers = SL_SPEAKER_FRONT_CENTER;
        }

        SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, channels, sr,
                                       SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                       speakers, SL_BYTEORDER_LITTLEENDIAN};

        SLDataSource audioSrc = {&loc_bufq, &format_pcm};

        // configure audio sink
        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, slesStream_->outputMixObject};
        SLDataSink audioSnk = {&loc_outmix, NULL};

        // create audio player
        const SLInterfaceID ids1[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};
        const SLboolean req1[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
        result = (*slesStream_->engineEngine)->CreateAudioPlayer(slesStream_->engineEngine, &(slesStream_->bqPlayerObject), &audioSrc, &audioSnk,
                                                       2, ids1, req1);
        if(result != SL_RESULT_SUCCESS) {
            Log_e("create audio player error %d", result);
            return result;
        }

        SLAndroidConfigurationItf playerConfig;
        result = (*slesStream_->bqPlayerObject)->GetInterface(slesStream_->bqPlayerObject, SL_IID_ANDROIDCONFIGURATION, &playerConfig);
        if(result != SL_RESULT_SUCCESS) {
            Log_e("get config error %d", result);
            return result;
        }

        SLint32 streamType = SL_ANDROID_STREAM_MEDIA;
        result = (*playerConfig)->SetConfiguration(playerConfig, SL_ANDROID_KEY_STREAM_TYPE, &streamType, sizeof(SLint32));
        if(result != SL_RESULT_SUCCESS) {
            Log_e("set config stream type error %d", result);
            return result;
        }

        // realize the player
        result = (*slesStream_->bqPlayerObject)->Realize(slesStream_->bqPlayerObject, SL_BOOLEAN_FALSE);
        if(result != SL_RESULT_SUCCESS) {
            Log_e("realize audio player error %d", result);
            return result;
        }

        // get the play interface
        result = (*slesStream_->bqPlayerObject)->GetInterface(slesStream_->bqPlayerObject, SL_IID_PLAY, &(slesStream_->bqPlayerPlay));
        if(result != SL_RESULT_SUCCESS) {
            Log_e("audio player get interface error %d", result);
            return result;
        }

        // get the buffer queue interface
        result = (*slesStream_->bqPlayerObject)->GetInterface(slesStream_->bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                                    &(slesStream_->bqPlayerBufferQueue));
        if(result != SL_RESULT_SUCCESS) {
            Log_e("android simple buffer queue get interface error %d", result);
            return result;
        }

        // register callback on the buffer queue
        result = (*slesStream_->bqPlayerBufferQueue)->RegisterCallback(slesStream_->bqPlayerBufferQueue, playerCallback, this);
        if(result != SL_RESULT_SUCCESS) {
            Log_e("audio player register callback error %d", result);
            return result;
        }

        // set the player's state to playing
        result = (*slesStream_->bqPlayerPlay)->SetPlayState(slesStream_->bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    }

    return result;
}

// puts a buffer of size samples to the device
bool SLESAudioCodec::streamAudio() {
    if (curPacket_){
        Log_v("delete previous packet");
        delete curPacket_;
    }

    waitForBuffer();
    Log_v("buffer available");

//    waitForPlayer();
//    Log_v("player ready");

//    playerReady_ = false;
//    Log_v("set player busy");

    curPacket_ = nextBuffer();
//    Log_v("next packet");
//    common::DataConstBuffer b(curPacket_->buffer);
//    Log_v("%s", common::dump(b).c_str());
//    const int8_t* buffer = new int8_t[b.size];
//    memcpy(&buffer, b.cdata, b.size);
//    common::DataConstBuffer b1(buffer, b.size);
//    Log_v("%s", common::dump(b1).c_str());
    SLresult result = (*slesStream_->bqPlayerBufferQueue)->Enqueue(slesStream_->bqPlayerBufferQueue, curPacket_->cbuffer.cdata, curPacket_->cbuffer.size);


    if (result != SL_RESULT_SUCCESS){
        Log_e("enqueue buffer error %d", result);
        closeAudioDevice();
        return false;
    }

    return true;

////
////    short *outBuffer;
////    int i, bufsamps = slesStream_->outBufSamples, index = slesStream_->currentOutputIndex;
////    if(slesStream_ == nullptr  || bufsamps ==  0)  return 0;
////    outBuffer = slesStream_->outputBuffer[slesStream_->currentOutputBuffer];
////
////    for(i=0; i < size; i++){
////        outBuffer[index++] = (short)(buffer[i]);
////        if (index >= slesStream_->outBufSamples) {
////            waitThreadLock(slesStream_->outlock);
////            (*slesStream_->bqPlayerBufferQueue)->Enqueue(slesStream_->bqPlayerBufferQueue,
////                                               outBuffer,bufsamps*sizeof(short));
////            slesStream_->currentOutputBuffer = (slesStream_->currentOutputBuffer ?  0 : 1);
////            index = 0;
////            outBuffer = slesStream_->outputBuffer[slesStream_->currentOutputBuffer];
////        }
////    }
////    slesStream_->currentOutputIndex = index;
////    slesStream_->time += (double) size/(slesStream_->sr*slesStream_->outchannels);
//    return i;
    return result == SL_RESULT_SUCCESS;
}

void SLESAudioCodec::waitForBuffer(){
    std::unique_lock<std::mutex> codeclock(dmutex_);
    while (pbuffers_.empty() && !decoderThreadQuitFlag_) {
        Log_v("buffers empty");
        auto t1 = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        dcond_.wait_until(codeclock, t1);
    }
}

void SLESAudioCodec::waitForPlayer(){
    std::unique_lock<std::mutex> lock(pmutex_);
    Log_v("player %s", playerReady_ ? "true" : "false");
    while (!playerReady_ && !decoderThreadQuitFlag_) {
        Log_v("player not ready");
        auto t1 = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        pcond_.wait_until(lock, t1);
    }
}

Packet::Pointer SLESAudioCodec::nextBuffer(){
    std::unique_lock<std::mutex> codeclock(dmutex_);
    if (Log::isVerbose()) Log_v("queue size2: %d", pbuffers_.size());
    Packet::Pointer buffer = pbuffers_.front();
    pbuffers_.pop();
    if (Log::isVerbose()) Log_v("queue size3: %d", pbuffers_.size());
    return buffer;
}

void SLESAudioCodec::feedBuffer(Packet::Pointer packet) {
    if (Log::isVerbose()) Log_v("feedBuffer");
    std::unique_lock<std::mutex> lock(dmutex_);
//    if (!quitFlag_) {

    pbuffers_.push(packet);
    if (Log::isVerbose()) Log_v("feedBuffer queue size1: %d", pbuffers_.size());
    lock.unlock();
    dcond_.notify_one();
//    }
}

void SLESAudioCodec::shutdown(){
    if (decoderThreadInitialized_) {
        if (Log::isInfo()) Log_i("Waiting for app thread to finish...");

        isRunning_ = false;

        ALooper_removeFd(bufferThreadLooper_, messagePipe_[0]);
        close(messagePipe_[0]);
        close(messagePipe_[1]);
        ALooper_release(bufferThreadLooper_);

        if (Log::isDebug()) Log_d("Stopping feedBufferThread");
        if (feedBufferThread_.joinable()){
            if (Log::isDebug()) Log_d("joining feedBuffe thread");
            feedBufferThread_.join();
        }

        decoderThreadQuitFlag_ = true;
//        if (Log::isDebug()) Log_d("Stop omxSource");
//        omxSource_->stop();

        if (decoderThreadRunning_) {
            if (Log::isDebug()) Log_d("Stopping decoderThread");

            if (decoderThread_.joinable()){
                if (Log::isDebug()) Log_d("joining decoder thread");
                decoderThread_.join();
            }
        }

        usleep(200);
//        ALooper_removeFd(bufferThreadLooper_, messagePipe_[0]);
//        close(messagePipe_[0]);
//        close(messagePipe_[1]);
//        ALooper_release(bufferThreadLooper_);

        if (Log::isDebug()) Log_d("close audio device");
        closeAudioDevice();

//        if (Log::isDebug()) Log_d("delete decoder");
//        delete omxDecoder_;
    }

    if (Log::isInfo()) Log_i("all done");
}

void SLESAudioCodec::init_decoderThread() {
    if (Log::isInfo()) Log_i("decoderThread begins");

    JNIEnv* env;
    JNIBase::javaAttachThread("SLESAudioCodec thread", &env);

    setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_AUDIO);
//    if (priority >= ANDROID_PRIORITY_BACKGROUND) {
//        set_sched_policy(gettid()(), SP_BACKGROUND);
//    } else {
//        set_sched_policy(gettid(), SP_FOREGROUND);
//    }

    openAudioDevice();
//    omxSource_ = new OMXSource(screenSize_.width, screenSize_.height, fps_, codecMutex_);

//    if (Log::isDebug()) Log_d("Created OMXSource %p", omxSource_);

    std::unique_lock<std::mutex> lock(dmutex_);
    decoderThreadRunning_ = true;
    decoderThreadInitialized_ = true;
    isRunning_ = true;
    lock.unlock();
    dcond_.notify_one();

    streamAudio();
//    while (!decoderThreadQuitFlag_ && streamAudio()) {
////        streamAudio();
//        Log_v("play streaming");
//    }

//    closeAudioDevice();

//    if (Log::isDebug()) Log_d("Stop omxDecoder");
//    omxDecoder_->stop();

    if (Log::isInfo()) Log_i("decoderThread ends");
    decoderThreadRunning_ = false;

    JNIBase::javaDetachThread();
}

void SLESAudioCodec::playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    if (Log::isVerbose()) Log_v("playerCallback");
    SLESAudioCodec::Pointer self = (SLESAudioCodec::Pointer) context;
    self->streamAudio();
//    std::unique_lock<std::mutex> lock(self->pmutex_);
//    self->playerReady_ = true;
//    if (Log::isVerbose()) Log_v("playerCallback self->playerReady_ = %s", self->playerReady_?"true":"false");
//    lock.unlock();
//    self->pcond_.notify_one();
}

int SLESAudioCodec::looperCallback(int fd, int events, void *data) {
    SLESAudioCodec::Pointer self = (SLESAudioCodec::Pointer)data;

    if (!self->isRunning_){
        if (Log::isWarn()) Log_v("looperCB: codec not running");
        return 0;
    }

    common::Data bufferData(SLESAudioCodec::cChunkSize);
    int size = read(fd, &bufferData[0], SLESAudioCodec::cChunkSize);
    if (Log::isVerbose()) Log_v("looperCB: readed= %d", size);

    int idx = 0;
    while (size > 0){
        Packet::Pointer packet = Packet::fromData(bufferData, idx);
        int packetSize = packet->packetSize();
        if (Log::isVerbose()) Log_v("looperCB: packetSize= %d", packetSize);
//        if (Log::isVerbose()) Log_v("looperCB: size= %d, %s", packet->buffer.size(), common::dump(common::DataConstBuffer(packet->buffer)).c_str());
        size -= packetSize;
        idx += packetSize;
        self->feedBuffer(packet);
        if (Log::isVerbose()) Log_v("looperCB: remaining size= %d", size);
    }
    return 1; // continue listening for events
}

void SLESAudioCodec::queueBuffer(common::DataConstBuffer& buffer, int64_t timestamp) {
    if (!isRunning_){
        if (Log::isWarn()) Log_v("codec not running");
        return;
    }

    if (Log::isVerbose()) Log_v("buffer size %d", buffer.size);
//    Log_v("write %s", common::dump(buffer).c_str());
    common::Data data = Packet::toData(buffer, timestamp);
    common::DataConstBuffer b(data);

    write(messagePipe_[1], &b.cdata[0], b.size);
}

void SLESAudioCodec::init_feedBufferThread() {
    Log_v("init_feedBufferThread");
    setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_AUDIO);

    bufferThreadLooper_ = ALooper_prepare(0);
    ALooper_acquire(bufferThreadLooper_);
    pipe(messagePipe_);
    ALooper_addFd(bufferThreadLooper_, messagePipe_[0], ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, &SLESAudioCodec::looperCallback, this);

    Log_v("start alooper polling");
    while (ALooper_pollOnce(0,nullptr, nullptr, nullptr) && !decoderThreadQuitFlag_) {
//        Log_v("pollOnce");
    }

    Log_v("init_feedBufferThread finished");
}


