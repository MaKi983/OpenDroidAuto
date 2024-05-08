#pragma once

#include <memory>
#include "messenger/Timestamp.hpp"
#include "common/Data.hpp"

namespace projection
{

class IAudioOutput
{
public:
    typedef IAudioOutput* Pointer;

    IAudioOutput() = default;
    virtual ~IAudioOutput() = default;

    virtual bool open() = 0;
    virtual void write(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer& buffer) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void suspend() = 0;
    virtual uint32_t getSampleSize() = 0;
    virtual uint32_t getChannelCount() = 0;
    virtual uint32_t getSampleRate() = 0;
};

}