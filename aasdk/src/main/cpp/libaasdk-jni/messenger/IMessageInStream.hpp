#pragma once

#include <memory>
#include <messenger/Promise.hpp>

namespace aasdk
{
namespace messenger
{

class IMessageInStream
{
public:
    typedef IMessageInStream* Pointer;

    IMessageInStream() = default;
    virtual ~IMessageInStream() = default;

    virtual void startReceive(ReceivePromise::Pointer promise) = 0;
};

}
}
