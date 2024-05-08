#pragma once

#include <memory>
#include "io/Promise.hpp"
#include "common/Data.hpp"

namespace projection
{

class IAudioInput
{
public:
    typedef aasdk::io::Promise<void, void> StartPromise;
    typedef aasdk::io::Promise<aasdk::common::Data, void> ReadPromise;
    typedef IAudioInput* Pointer;

    virtual ~IAudioInput() = default;

    virtual bool open() = 0;
    virtual bool isActive() = 0;
    virtual void read(projection::IAudioInput::ReadPromise::Pointer promise) = 0;
    virtual void start(projection::IAudioInput::StartPromise::Pointer promise) = 0;
    virtual void stop() = 0;
    virtual uint32_t getSampleSize() = 0;
    virtual uint32_t getChannelCount() = 0;
    virtual uint32_t getSampleRate() = 0;
    virtual uint32_t getBufferSize() = 0;
};

}
