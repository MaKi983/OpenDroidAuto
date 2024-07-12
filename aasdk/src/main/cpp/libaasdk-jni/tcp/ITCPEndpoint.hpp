#pragma once

#include <memory>
#include <common/Data.hpp>
#include <io/Promise.hpp>

namespace aasdk
{
namespace tcp
{

class ITCPEndpoint
{
public:
    typedef std::shared_ptr<ITCPEndpoint> Pointer;
    typedef io::Promise<size_t> Promise;
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> SocketPointer;

    virtual ~ITCPEndpoint() = default;

    virtual void send(common::DataConstBuffer buffer, Promise::Pointer promise) = 0;
    virtual void receive(common::DataBuffer buffer, Promise::Pointer promise) = 0;
    virtual void stop() = 0;
};

}
}