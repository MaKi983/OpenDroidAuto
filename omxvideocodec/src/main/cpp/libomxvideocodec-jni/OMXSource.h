#pragma once

#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <utils/RefBase.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <queue>
#include <thread>

using namespace android;

struct Packet {
    typedef Packet* Pointer;

    Packet(int bufferSize){
        buf = new unsigned char[bufferSize];
    }
    unsigned char* buf;
    size_t len;
    uint64_t timestamp;

    void clear(){
        len = 0;
        timestamp = 0;
    }

//    ~Packet(){ delete[] buf; }
    ~Packet(){ free(buf); }
};

class OMXSource : public MediaSource {
public:
    typedef OMXSource* Pointer;

    OMXSource(int width, int height, int fps, std::mutex& mutex);

    virtual status_t read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) ;
    virtual status_t start(MetaData *params = NULL);
    virtual status_t stop();
    virtual sp<MetaData> getFormat();
    int queueReadBuffer(Packet::Pointer buffer);
//    status_t queueMediaBuffer(MediaBuffer* buffer);
    Packet::Pointer  getWriteBuffer();
//    status_t getMediaBuffer(MediaBuffer** buffer);
    void waitForReadBuffer();
    void waitForWriteBuffer();
//    void waitForMediaBuffer();

    virtual status_t pause() {
        return ERROR_UNSUPPORTED;
    }

    virtual status_t setBuffers(const Vector<MediaBuffer *> &buffers) {
        return ERROR_UNSUPPORTED;
    }

    ~OMXSource();
protected:


private:
    sp<MetaData> format_;
    MediaBufferGroup group_;
    std::queue<Packet*> readBuffers_;
    std::queue<Packet*> writeBuffers_;
    std::queue<MediaBuffer*> mediaBuffers_;

    std::mutex& mutex_;
    std::condition_variable cond_;
    size_t bufferSize_;
    bool quitFlag_;

//    MediaBuffer* nextMediaBuffer();
    Packet::Pointer nextReadBuffer();
    Packet::Pointer nextWriteBuffer();
};
