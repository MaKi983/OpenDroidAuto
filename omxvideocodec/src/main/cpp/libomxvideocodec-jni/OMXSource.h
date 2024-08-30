#pragma once

#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <utils/RefBase.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <queue>
#include <thread>
#include "Packet.h"
#include <atomic_queue/atomic_queue.h>

using namespace android;

class OMXSource : public MediaSource {
public:
    typedef OMXSource* Pointer;
    static constexpr common::Data::size_type cChunkSize = 38406 * 4;

    OMXSource(int width, int height, int fps, std::mutex& mutex);

    virtual status_t read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) ;
    virtual status_t start(MetaData *params = NULL);
    virtual status_t stop();
    virtual sp<MetaData> getFormat();
    void queueBuffer(Packet::Pointer packet);

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
    atomic_queue::AtomicQueueB<Packet::Pointer> pbuffers_;

    bool quitFlag_;

    Packet::Pointer nextBuffer();
};
