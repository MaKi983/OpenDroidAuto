#pragma once

#include <common/Data.hpp>
#include <messenger/FrameSizeType.hpp>

namespace aasdk
{
namespace messenger
{

class FrameSize
{
public:
    FrameSize(size_t frameSize, size_t totalSize);
    FrameSize(size_t frameSize);
    FrameSize(const common::DataConstBuffer& buffer);
    ~FrameSize();

    common::Data getData() const;
    size_t getSize() const;

    static size_t getSizeOf(FrameSizeType type);

    std::string toString();
private:
    FrameSizeType frameSizeType_;
    size_t frameSize_;
    size_t totalSize_;
};

}
}
