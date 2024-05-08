#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <tcp/ITCPEndpoint.hpp>
#include <tcp/ITCPWrapper.hpp>

namespace aasdk
{
namespace tcp
{

class TCPEndpoint: public ITCPEndpoint
{
public:
    TCPEndpoint(ITCPWrapper& tcpWrapper, SocketPointer socket);
    ~TCPEndpoint();

    void send(common::DataConstBuffer buffer, Promise::Pointer promise) override;
    void receive(common::DataBuffer buffer, Promise::Pointer promise) override;
    void stop() override;

private:
    void asyncOperationHandler(const boost::system::error_code& ec, size_t bytesTransferred, Promise::Pointer promise);

    ITCPWrapper& tcpWrapper_;
    SocketPointer socket_;
};

}
}