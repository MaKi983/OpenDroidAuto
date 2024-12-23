#pragma once

#include <messenger/Message.hpp>
#include <messenger/MessageId.hpp>
#include <messenger/Promise.hpp>

namespace aasdk
{
namespace messenger
{

class IMessageOutStream
{
public:
    typedef std::shared_ptr<IMessageOutStream> Pointer;

    IMessageOutStream() = default;
    virtual ~IMessageOutStream() = default;

    virtual void stream(Message::Pointer message, SendPromise::Pointer promise) = 0;
    virtual void stop() = 0;
};

}
}
