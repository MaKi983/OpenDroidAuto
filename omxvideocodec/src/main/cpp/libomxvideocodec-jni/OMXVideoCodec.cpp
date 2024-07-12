#include "OMXVideoCodec.h"
#include "Log.h"

#include <unistd.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <inttypes.h>
#include <chrono>
#include <sstream>
#include <iomanip>

OMXVideoCodec::OMXVideoCodec()
    : fps_(0), omxDecoderThreadRunning_(false), decoderThreadInitialized_(false), decoderThreadQuitFlag_(false), tmpPacket_(nullptr){
}

//OMXVideoCodec::~OMXVideoCodec() {
//    Log_i("destructor");
//}

status_t OMXVideoCodec::init(int fps){
    fps_ = fps;
    if (Log::isInfo()) Log_i("FPS %d", fps_);
    // Create decoder thread and wait until ready
    if (Log::isInfo()) Log_i("start omxDecoderThread");

    feedBufferThread_ = std::thread(&OMXVideoCodec::init_feedBufferThread, this);
    omxDecoderThread_ = std::thread(&OMXVideoCodec::init_omxDecoderThread, this);

    std::unique_lock<std::mutex> codeclock(codecMutex_);
    while (!decoderThreadInitialized_){
        omxDecoderCond_.wait(codeclock);
    }
    codeclock.unlock();
    if (Log::isInfo()) Log_i("omxDecoderThread ready");

    return omxDecoder_->getStatus();
}

void OMXVideoCodec::shutdown(){
    if (decoderThreadInitialized_) {
        if (Log::isInfo()) Log_i("Waiting for app thread to finish...");

        decoderThreadQuitFlag_ = true;

        if (Log::isDebug()) Log_d("Stop omxSource");
        omxSource_->stop();

        ALooper_removeFd(bufferThreadLooper_, messagePipe_[0]);
        close(messagePipe_[0]);
        close(messagePipe_[1]);
        ALooper_release(bufferThreadLooper_);
        if (Log::isDebug()) Log_d("Stopping feedBufferThread");

        if (feedBufferThread_.joinable()){
            if (Log::isDebug()) Log_d("joining feedBuffer thread");
            feedBufferThread_.join();
        }

        if (omxDecoderThreadRunning_) {
            if (Log::isDebug()) Log_d("Stopping omxDecoderThread");

            if (omxDecoderThread_.joinable()){
                if (Log::isDebug()) Log_d("joining decoder thread");
                omxDecoderThread_.join();
            }
        }

        if (Log::isDebug()) Log_d("release native window");
        ANativeWindow_release(nativeWindow_.get());

        if (Log::isDebug()) Log_d("delete decoder");
        delete omxDecoder_;
    }

    if (Log::isInfo()) Log_i("all done");
}

void OMXVideoCodec::init_omxDecoderThread() {
    if (Log::isInfo()) Log_i("omxDecoderThread begins");

//    JNIEnv* env;
//    JNIBase::javaAttachThread("OMXCodec thread", &env);

    omxSource_ = new OMXSource(screenSize_.width, screenSize_.height, fps_, codecMutex_);

    if (Log::isDebug()) Log_d("Created OMXSource %p", omxSource_);

    omxDecoder_ = new OMXDecoder(codecMutex_);
    omxDecoder_->setNativeWindow(nativeWindow_);
    omxDecoder_->setSource(omxSource_);
    omxDecoder_->init();

    std::unique_lock<std::mutex> codeclock(codecMutex_);
    omxDecoderThreadRunning_ = true;
    decoderThreadInitialized_ = true;
    codeclock.unlock();
    omxDecoderCond_.notify_one();

    while (!decoderThreadQuitFlag_) {
        omxDecoder_->read();
    }

    if (Log::isDebug()) Log_d("Stop omxDecoder");
    omxDecoder_->stop();

    if (Log::isInfo()) Log_i("omxDecoderThread ends");
    omxDecoderThreadRunning_ = false;

//    JNIBase::javaDetachThread();
}

void OMXVideoCodec::setNativeWindow(ANativeWindow* nativeWindow, int height, int width){
    if (Log::isDebug()) Log_d("set native window size w: %d, h: %d", width, height);
    nativeWindow_ = nativeWindow;
    screenSize_.width = width;
    screenSize_.height = height;
}

void OMXVideoCodec::setSps(common::DataConstBuffer &b){
    if (Log::isDebug()) Log_d("sps packet len %d", b.size);
    omxSource_->getFormat()->setData(kKeyAVCC, kTypeAVCC, b.cdata, b.size);
}

OMXSource::Pointer OMXVideoCodec::source() {
    return omxSource_;
}

OMXDecoder::Pointer OMXVideoCodec::decoder() {
    return omxDecoder_;
}

int OMXVideoCodec::looperCallback(int fd, int events, void *data) {
    OMXVideoCodec::Pointer self = (OMXVideoCodec::Pointer)data;

    if (self->decoderThreadQuitFlag_){
        if(Log::isVerbose()) Log_v("looperCB: decoder is stopping, return");
        return 0;
    }

    common::Data bufferData(OMXSource::cChunkSize);
    int size = read(fd, &bufferData[0], OMXSource::cChunkSize);
    if (Log::isVerbose()) Log_v("looperCB: readed= %d", size);

    int idx = 0;
    while (size > 0){
        Packet::Pointer packet;
        if (self->tmpPacket_ != nullptr){
            if (Log::isVerbose()) Log_v("looperCB: found prev packet, complete it");
            packet = self->tmpPacket_;
            self->tmpPacket_ = nullptr;
            size_t remainingSize = packet->size - packet->buffer.size();
            if (Log::isVerbose()) Log_v("looperCB: prev packet remaining size %d", remainingSize);
            size_t currentSize = remainingSize;
            if (size < remainingSize){
                currentSize = size;
            }
            common::DataConstBuffer b(&bufferData[0], currentSize);
            common::copy(packet->buffer, b);
            size -= remainingSize;
            if (Log::isVerbose()) Log_v("looperCB: remaining size= %d", size);
            idx += remainingSize;
        } else {
            packet = Packet::fromData(bufferData, idx);
            size_t packetSize = packet->packetSize();
            if (Log::isVerbose()) Log_v("looperCB: packetSize= %d", packetSize);
            size -= packetSize;
            if (Log::isVerbose()) Log_v("looperCB: remaining size= %d", size);
            idx += packetSize;
        }

        if (size < 0){
            if (Log::isVerbose()) Log_v("looperCB: packet not complete, keep it");
            self->tmpPacket_ = packet;
        } else {
            if (self->decoderThreadQuitFlag_){
                if(Log::isVerbose()) Log_v("looperCB: decoder is stopping, do not send buffer to queue");
                return 0;
            }

            if (Log::isVerbose()) Log_v("looperCB: send buffer to queue");
            self->source()->queueBuffer(packet);
        }
    }

    return 1; // continue listening for events
}

void OMXVideoCodec::queueBuffer(common::DataConstBuffer& buffer, int64_t timestamp) {
    if (decoderThreadQuitFlag_){
        if(Log::isVerbose()) Log_v("decoder is stopping, return");
        return;
    }

    const auto p1 = std::chrono::system_clock::now();
    int64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
    if (timestamp == 0){
        ts = 0;
    }

    common::Data data = Packet::toData(buffer, ts);
    common::DataConstBuffer b(data);

    write(messagePipe_[1], &b.cdata[0], b.size);
}

void OMXVideoCodec::init_feedBufferThread() {
    Log_v("init_feedBufferThread");
    bufferThreadLooper_ = ALooper_prepare(0);
    ALooper_acquire(bufferThreadLooper_);
    pipe(messagePipe_);
    ALooper_addFd(bufferThreadLooper_, messagePipe_[0], ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, &OMXVideoCodec::looperCallback, this);

    Log_v("start alooper polling");
    while (ALooper_pollOnce(0,nullptr, nullptr, nullptr) && !decoderThreadQuitFlag_) {
//        Log_v("pollOnce");
    }

    Log_v("init_feedBufferThread finished");
}
