#pragma once

#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <utils/RefBase.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <queue>
#include <thread>
#include <atomic_queue/atomic_queue.h>
#include <Data.hpp>

using namespace android;

class OMXSource : public MediaSource, public MediaBufferObserver {
public:
    typedef OMXSource* Pointer;
    static constexpr common::Data::size_type cChunkSize = 38406 * 4;

    OMXSource(int width, int height, int fps, std::mutex& mutex);

    status_t read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) override;
    status_t start(MetaData *params) override;
    status_t stop() override;
    sp<MetaData> getFormat() override;
    void queueBuffer(MediaBuffer* buffer);

    status_t pause() override {
        return ERROR_UNSUPPORTED;
    }

    status_t setBuffers(const Vector<MediaBuffer *> &buffers) override {
        return ERROR_UNSUPPORTED;
    }

    void signalBufferReturned(MediaBuffer *buffer) override;

    ~OMXSource() override;
protected:


private:
    sp<MetaData> format_;
    std::queue<MediaBuffer*> pbuffers_;
    std::mutex& mutex_;
    std::condition_variable cond_;

    bool quitFlag_;

//    MediaBuffer* nextBuffer();
};
