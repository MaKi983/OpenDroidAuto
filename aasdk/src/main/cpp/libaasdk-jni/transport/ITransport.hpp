#pragma once

#include <memory>
#include <common/Data.hpp>
#include <io/Promise.hpp>


namespace aasdk
{
namespace transport
{

class ITransport
{
public:
    typedef std::shared_ptr<ITransport> Pointer;
    typedef io::Promise<common::Data> ReceivePromise;
    typedef io::Promise<void> SendPromise;

    ITransport() = default;
    virtual ~ITransport() = default;

    virtual void receive(size_t size, ReceivePromise::Pointer promise) = 0;
    virtual void send(common::Data data, SendPromise::Pointer promise) = 0;
    virtual void stop() = 0;
};

}
}
