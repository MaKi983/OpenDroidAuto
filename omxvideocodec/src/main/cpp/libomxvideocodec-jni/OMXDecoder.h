#pragma once

#include <utils/Errors.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/MediaSource.h>
#include <OMXSource.h>

using namespace android;

class OMXDecoder {
public:
    typedef OMXDecoder* Pointer;

    OMXDecoder(std::mutex& mutex);

    virtual status_t read();
    void setNativeWindow(sp<ANativeWindow> &nativeWindow);
    void setSource(OMXSource *source);
    status_t  init();
   int32_t frameRate();
    void stop();
    ~OMXDecoder();
    status_t getStatus();
protected:


private:
    OMXClient* client_;
    sp<MediaSource> decoder_;
    sp<ANativeWindow> nativeWindow_;
    OMXSource::Pointer source_;

    std::mutex& mutex_;
    int32_t frameRate_;

    status_t status_;
};
