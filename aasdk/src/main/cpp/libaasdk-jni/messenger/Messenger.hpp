#pragma once

#include <boost/asio.hpp>
#include <list>
#include <messenger/IMessenger.hpp>
#include <messenger/IMessageInStream.hpp>
#include <messenger/IMessageOutStream.hpp>
#include <messenger/ChannelReceiveMessageQueue.hpp>
#include <messenger/ChannelReceivePromiseQueue.hpp>

namespace aasdk
{
namespace messenger
{

class Messenger: public IMessenger, boost::noncopyable
{
public:
    Messenger(boost::asio::io_service& ioService, IMessageInStream::Pointer messageInStream, IMessageOutStream::Pointer messageOutStream);
    ~Messenger();
    void enqueueReceive(ChannelId channelId, ReceivePromise::Pointer promise) override;
    void enqueueSend(Message::Pointer message, SendPromise::Pointer promise) override;
    void stop() override;

private:
    typedef std::list<std::pair<Message::Pointer, SendPromise::Pointer>> ChannelSendQueue;
    void doSend();
    void inStreamMessageHandler(Message::Pointer message);
    void outStreamMessageHandler(ChannelSendQueue::iterator queueElement);
    void rejectReceivePromiseQueue(const error::Error& e);
    void rejectSendPromiseQueue(const error::Error& e);

    boost::asio::io_service::strand receiveStrand_;
    boost::asio::io_service::strand sendStrand_;
    IMessageInStream::Pointer messageInStream_;
    IMessageOutStream::Pointer messageOutStream_;

    ChannelReceivePromiseQueue channelReceivePromiseQueue_;
    ChannelReceiveMessageQueue channelReceiveMessageQueue_;
    ChannelSendQueue channelSendPromiseQueue_;

    bool isStopping_;
};

}
}
