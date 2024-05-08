#include <transport/TCPTransport.hpp>

namespace aasdk
{
namespace transport
{

TCPTransport::TCPTransport(boost::asio::io_service& ioService, tcp::ITCPEndpoint::Pointer tcpEndpoint)
    : Transport(ioService)
    , tcpEndpoint_(std::move(tcpEndpoint))
{

}

void TCPTransport::enqueueReceive(common::DataBuffer buffer)
{
    auto receivePromise = tcp::ITCPEndpoint::Promise::defer(receiveStrand_);
    receivePromise->then([this](auto bytesTransferred) {
            this->receiveHandler(bytesTransferred);
        },
        [this](auto e) {
            this->rejectReceivePromises(e);
        });

    tcpEndpoint_->receive(buffer, std::move(receivePromise));
}

void TCPTransport::enqueueSend(SendQueue::iterator queueElement)
{
    auto sendPromise = tcp::ITCPEndpoint::Promise::defer(sendStrand_);

    sendPromise->then([this, queueElement](auto) {
        this->sendHandler(queueElement, error::Error());
    },
    [this, queueElement](auto e) {
        this->sendHandler(queueElement, e);
    });

    tcpEndpoint_->send(common::DataConstBuffer(queueElement->first), std::move(sendPromise));
}

void TCPTransport::stop()
{
    tcpEndpoint_->stop();
}

void TCPTransport::sendHandler(SendQueue::iterator queueElement, const error::Error& e)
{
    if(!e)
    {
        queueElement->second->resolve();
    }
    else
    {
        queueElement->second->reject(e);
    }

    sendQueue_.erase(queueElement);

    if(!sendQueue_.empty())
    {
        this->enqueueSend(sendQueue_.begin());
    }
}

}
}
