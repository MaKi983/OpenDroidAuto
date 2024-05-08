#pragma once

#include <tcp/ITCPEndpoint.hpp>
#include <transport/Transport.hpp>


namespace aasdk
{
namespace transport
{

class TCPTransport: public Transport
{
public:
    TCPTransport(boost::asio::io_service& ioService, tcp::ITCPEndpoint::Pointer tcpEndpoint);

    void stop() override;

private:
    void enqueueReceive(common::DataBuffer buffer) override;
    void enqueueSend(SendQueue::iterator queueElement) override;
    void sendHandler(SendQueue::iterator queueElement, const error::Error& e);

    tcp::ITCPEndpoint::Pointer tcpEndpoint_;
};

}
}
