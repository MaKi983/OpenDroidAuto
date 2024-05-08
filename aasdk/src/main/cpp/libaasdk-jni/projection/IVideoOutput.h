#pragma once

#include <memory>
#include "proto/VideoFPSEnum.pb.h"
#include "proto/VideoResolutionEnum.pb.h"
#include "common/Data.hpp"

namespace projection
{

struct Rect {
    int width;
    int height;
};

class IVideoOutput
{
public:
    typedef IVideoOutput* Pointer;

    IVideoOutput() = default;
    virtual ~IVideoOutput() = default;

    virtual bool open() = 0;
    virtual bool init() = 0;
    virtual void write(uint64_t timestamp, const aasdk::common::DataConstBuffer& buffer) = 0;
    virtual void stop() = 0;
    virtual aasdk::proto::enums::VideoFPS::Enum getVideoFPS() = 0;
    virtual aasdk::proto::enums::VideoResolution::Enum getVideoResolution() = 0;
    virtual size_t getScreenDPI() = 0;
    virtual Rect getVideoMargins() = 0;
};

}