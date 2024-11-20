#pragma once

#include <boost/asio.hpp>
#include <list>
#include <messenger/IMessenger.hpp>
#include <messenger/IMessageInStream.hpp>
#include <messenger/IMessageOutStream.hpp>

namespace aasdk
{
namespace messenger
{

class Messenger: public IMessenger, public std::enable_shared_from_this<Messenger>, boost::noncopyable
{
public:
    Messenger(boost::asio::io_service& ioService, IMessageInStream::Pointer messageInStream, IMessageOutStream::Pointer messageOutStream);
    ~Messenger();
    void enqueueReceive(ChannelId channelId, ReceivePromise::Pointer promise) override;
    void enqueueSend(Message::Pointer message, SendPromise::Pointer promise) override;
    void stop() override;
    void startReceive() override;

private:
    using std::enable_shared_from_this<Messenger>::shared_from_this;
    typedef std::unordered_map<ChannelId, ReceivePromise::Pointer> ChannelReceivePromiseQueue;
	typedef std::list<std::pair<Message::Pointer, SendPromise::Pointer>> ChannelSendQueue;
	
	void doSend();
    void doReceive();
    void inStreamMessageHandler(Message::Pointer message);
    void outStreamMessageHandler(ChannelSendQueue::iterator queueElement);
    void rejectReceivePromiseQueue(const error::Error& e);
    void rejectSendPromiseQueue(const error::Error& e);

    boost::asio::io_service::strand receiveStrand_;
    boost::asio::io_service::strand sendStrand_;
    IMessageInStream::Pointer messageInStream_;
    IMessageOutStream::Pointer messageOutStream_;

    ChannelReceivePromiseQueue channelReceivePromiseQueue_;
	ChannelSendQueue channelSendPromiseQueue_;

    bool isStopping_;
};

}
}
