#include <transport/TCPTransport.hpp>
#include <Log.h>

namespace aasdk
{
namespace transport
{

TCPTransport::TCPTransport(aasdk::io::ioService& ioService, tcp::ITCPEndpoint::Pointer tcpEndpoint)
    : Transport(ioService)
    , tcpEndpoint_(std::move(tcpEndpoint))
{

}

void TCPTransport::enqueueReceive(common::DataBuffer buffer)
{
    if (Log::isVerbose()) Log_v("enqueueReceive");
    auto receivePromise = tcp::ITCPEndpoint::Promise::defer(receiveStrand_, "TCPTransport_enqueueReceive");
    receivePromise->then([this, self = this->shared_from_this()](auto bytesTransferred) {
            this->receiveHandler(bytesTransferred);
        },
        [this, self = this->shared_from_this()](auto e) {
            this->rejectReceivePromises(e);
        });

    tcpEndpoint_->receive(buffer, std::move(receivePromise));
}

void TCPTransport::enqueueSend(SendQueue::iterator queueElement)
{
    auto sendPromise = tcp::ITCPEndpoint::Promise::defer(sendStrand_, "TCPTransport_enqueueSend");

    sendPromise->then([this, self = this->shared_from_this(), queueElement](auto) {
        this->sendHandler(queueElement, error::Error());
    },
    [this, self = this->shared_from_this(), queueElement](auto e) {
        this->sendHandler(queueElement, e);
    });

	if (Log::isDebug()) Log_d("send to tcpendpoint");
    tcpEndpoint_->send(common::DataConstBuffer(queueElement->first), std::move(sendPromise));
}

void TCPTransport::stop()
{
    if (Log::isDebug()) Log_d("stop tcp endpoint");
    isStopping_ = true;
    sendQueue_.clear();
    receiveStrand_ = boost::none;
    sendStrand_ = boost::none;
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
