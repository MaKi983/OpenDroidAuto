#pragma once

#include <list>
#include <queue>
#include <boost/asio.hpp>
#include <transport/ITransport.hpp>
#include <transport/DataSink.hpp>

namespace aasdk
{
namespace transport
{

class Transport: public ITransport, public std::enable_shared_from_this<Transport>, boost::noncopyable
{
public:
    Transport(boost::asio::io_service& ioService);

    void receive(size_t size, ReceivePromise::Pointer promise) override;
    void send(common::Data data, SendPromise::Pointer promise) override;

protected:
    typedef std::list<std::pair<size_t, ReceivePromise::Pointer>> ReceiveQueue;
    typedef std::list<std::pair<common::Data, SendPromise::Pointer>> SendQueue;

    using std::enable_shared_from_this<Transport>::shared_from_this;
    void receiveHandler(size_t bytesTransferred);
    void distributeReceivedData();
    void rejectReceivePromises(const error::Error& e);

    virtual void enqueueReceive(common::DataBuffer buffer) = 0;
    virtual void enqueueSend(SendQueue::iterator queueElement) = 0;

    DataSink receivedDataSink_;

    boost::asio::io_service::strand receiveStrand_;
    ReceiveQueue receiveQueue_;

    boost::asio::io_service::strand sendStrand_;
    SendQueue sendQueue_;
};

}
}
