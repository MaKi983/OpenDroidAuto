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

    OMXSource(int width, int height, int fps/*, std::mutex& mutex*/);

    status_t read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) override;
    status_t start(MetaData *params) override;
    status_t stop() override;
    sp<MetaData> getFormat() override;
    void queueBuffer(Packet::Pointer packet);

    status_t pause() override {
        return ERROR_UNSUPPORTED;
    }

    status_t setBuffers(const Vector<MediaBuffer *> &buffers) override {
        return ERROR_UNSUPPORTED;
    }

    ~OMXSource() override;
protected:


private:
    sp<MetaData> format_;
    MediaBufferGroup group_;
    atomic_queue::AtomicQueueB<Packet::Pointer> pbuffers_;

    bool quitFlag_;

    Packet::Pointer nextBuffer();
};
