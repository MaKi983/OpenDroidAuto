#include "SLESAudioCodec.h"
#include "Log.h"

#include <unistd.h>
#include <inttypes.h>
#include <sys/resource.h>

#define NUM_BUFFERS 5

SLESAudioCodec::SLESAudioCodec(int sampleRate, int channelConfig, int sampleSize):
    slesStream_(nullptr), sampleRate_(sampleRate), sampleSize_(sampleSize), channelConfig_(channelConfig),
    curPacket_(nullptr), pbuffers_(20)
{
    openAudioDevice();
}

SLESAudioCodec::~SLESAudioCodec() {
    Log_i("destructor");
}

//void SLESAudioCodec::init(){
//    if (Log::isInfo()) Log_i("start decoderThread");
//
//    decoderThread_ = std::thread(&SLESAudioCodec::init_decoderThread, this);
//
//    std::unique_lock<std::mutex> lock(dmutex_);
//    while (!decoderThreadInitialized_){
//        dcond_.wait(lock);
//    }
//    lock.unlock();
//    Log_i("decoderThread ready");
//}

// open the android audio device for input and/or output
void SLESAudioCodec::openAudioDevice() {
    slesStream_ = new SLESStream();

    if(createEngine() != SL_RESULT_SUCCESS) {
        closeAudioDevice();
        return;
    }

    if(createAudioPlayer() != SL_RESULT_SUCCESS) {
        closeAudioDevice();
        return;
    }
}

// close the android audio device
void SLESAudioCodec::closeAudioDevice() {
   if (slesStream_ == nullptr)
        return;

    destroyEngine();

    delete slesStream_;
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

    if (Log::isInfo()) Log_i("Engine created");

    return result;
}

// close the OpenSL IO and destroy the audio engine
void SLESAudioCodec::destroyEngine()
{
    SLresult result;
//    // destroy buffer queue audio player object, and invalidate all associated interfaces
//    if (slesStream_->bqPlayerBufferQueue != nullptr) {
//        if (Log::isInfo()) Log_i("clear bqPlayerBufferQueue");
//        (*slesStream_->bqPlayerBufferQueue)->Clear(slesStream_->bqPlayerBufferQueue);
//        slesStream_->bqPlayerBufferQueue = nullptr;
//    }

    if (slesStream_->bqPlayerObject != nullptr) {
        if (Log::isInfo()) Log_i("destroy bqPlayerObject");
        (*slesStream_->bqPlayerObject)->Destroy(slesStream_->bqPlayerObject);
        slesStream_->bqPlayerObject = nullptr;
        slesStream_->bqPlayerPlay = nullptr;
    }

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
        if(result != SL_RESULT_SUCCESS) {
            Log_e("realize outputmix error %d", result);
            return result;
        }

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
//        const SLInterfaceID ids1[] = {SL_IID_BUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};
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
//
//        // set the player's state to playing
//        result = (*slesStream_->bqPlayerPlay)->SetPlayState(slesStream_->bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    }

    if (Log::isInfo()) Log_i("Audio Player created");

    return result;
}

// puts a buffer of size samples to the device
bool SLESAudioCodec::streamAudio() {
    Log_v("streamAudio");
    if (curPacket_){
        Log_v("delete previous packet");
        delete curPacket_;
    }

    curPacket_ = nextBuffer();
    SLresult result = (*slesStream_->bqPlayerBufferQueue)->Enqueue(slesStream_->bqPlayerBufferQueue, curPacket_->cbuffer.cdata, curPacket_->cbuffer.size);

    if (result != SL_RESULT_SUCCESS){
        Log_e("enqueue buffer error %d", result);
        closeAudioDevice();
        return false;
    }

    return true;
}

Packet::Pointer SLESAudioCodec::nextBuffer(){
    Packet::Pointer buffer = pbuffers_.pop();
    if (Log::isVerbose()) Log_v("queue size3: %d", pbuffers_.was_size());
    return buffer;
}

void SLESAudioCodec::feedBuffer(Packet::Pointer packet) {
    if (Log::isVerbose()) Log_v("feedBuffer");
    pbuffers_.push(packet);
    if (Log::isVerbose()) Log_v("feedBuffer queue size1: %d", pbuffers_.was_size());
}

void SLESAudioCodec::start(){
    // set the player's state to playing
    if (Log::isInfo()) Log_i("Set state PLAYING");
    SLresult result = (*slesStream_->bqPlayerPlay)->SetPlayState(slesStream_->bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    if (result != SL_RESULT_SUCCESS){
        Log_e("set PLAYING state error %d", result);
    }
    streamAudio();
}

void SLESAudioCodec::stop(){
    // set the player's state to stop
    if (Log::isInfo()) Log_i("Set state STOPPED");
    SLresult result = (*slesStream_->bqPlayerPlay)->SetPlayState(slesStream_->bqPlayerPlay, SL_PLAYSTATE_STOPPED);
    if (result != SL_RESULT_SUCCESS){
        Log_e("set stopped state error %d", result);
    }

    // destroy buffer queue audio player object
    if (slesStream_->bqPlayerBufferQueue != nullptr) {
        if (Log::isInfo()) Log_i("clear bqPlayerBufferQueue");
        result = (*slesStream_->bqPlayerBufferQueue)->Clear(slesStream_->bqPlayerBufferQueue);
        slesStream_->bqPlayerBufferQueue = nullptr;
        if (result != SL_RESULT_SUCCESS){
            Log_e("clear bqPlayerBufferQueue error %d", result);
        }
    }

    if (Log::isInfo()) Log_i("clear pbuffers %d", pbuffers_.was_size());
    while (!pbuffers_.was_empty()){
        if (Log::isInfo()) Log_i("delete packet");
        Packet::Pointer p = pbuffers_.pop();
        delete p;
    }
}

void SLESAudioCodec::shutdown(){
    if (Log::isDebug()) Log_d("close audio device");
    closeAudioDevice();

    if (Log::isInfo()) Log_i("all done");
}

//void SLESAudioCodec::init_decoderThread() {
//    if (Log::isInfo()) Log_i("decoderThread begins");
//
//    JNIEnv* env;
//    JNIBase::javaAttachThread("SLESAudioCodec thread", &env);
//
//    setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_AUDIO);
//
//    std::unique_lock<std::mutex> lock(dmutex_);
//    decoderThreadRunning_ = true;
//    decoderThreadInitialized_ = true;
//    isRunning_ = true;
//    lock.unlock();
//    dcond_.notify_one();
//
//    streamAudio();
////    while (!decoderThreadQuitFlag_ && streamAudio()) {
//////        streamAudio();
////        Log_v("play streaming");
////    }
//
////    closeAudioDevice();
//
////    if (Log::isDebug()) Log_d("Stop omxDecoder");
////    omxDecoder_->stop();
//
//    if (Log::isInfo()) Log_i("decoderThread ends");
//    decoderThreadRunning_ = false;
//
//    JNIBase::javaDetachThread();
//}

void SLESAudioCodec::playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    if (Log::isVerbose()) Log_v("playerCallback");
    SLESAudioCodec::Pointer self = (SLESAudioCodec::Pointer) context;
    self->streamAudio();
}

void SLESAudioCodec::queueBuffer(common::DataConstBuffer& buffer, int64_t timestamp) {
    if (Log::isVerbose()) Log_v("buffer size %d", buffer.size);
    common::Data d;
    common::copy(d, buffer);

    Packet::Pointer p = new Packet(d, timestamp, d.size());
    feedBuffer(p);
}


