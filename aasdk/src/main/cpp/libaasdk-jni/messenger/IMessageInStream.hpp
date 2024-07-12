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
    typedef std::shared_ptr<IMessageInStream> Pointer;

    IMessageInStream() = default;
    virtual ~IMessageInStream() = default;

    virtual void startReceive(ReceivePromise::Pointer promise) = 0;
//    virtual void stop() = 0;
};

}
}
