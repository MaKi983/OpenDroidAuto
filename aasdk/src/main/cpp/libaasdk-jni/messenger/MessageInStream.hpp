#pragma once

#include <map>
#include <transport/ITransport.hpp>
#include <messenger/IMessageInStream.hpp>
#include <messenger/ICryptor.hpp>
#include <messenger/FrameHeader.hpp>
#include <messenger/FrameSize.hpp>
#include <messenger/FrameType.hpp>

namespace aasdk
{
namespace messenger
{

class MessageInStream: public IMessageInStream, public std::enable_shared_from_this<MessageInStream>, boost::noncopyable
{
public:
    MessageInStream(boost::asio::io_service& ioService, transport::ITransport::Pointer transport, ICryptor::Pointer cryptor);

    void startReceive(ReceivePromise::Pointer promise) override;
//    void stop() override;
private:
    using std::enable_shared_from_this<MessageInStream>::shared_from_this;

//    void receiveFrameHeaderHandler(const common::DataConstBuffer& buffer, ReceivePromise::Pointer promise);
//    void receiveFrameSizeHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise);
//    void receiveFramePayloadHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise);
    void receiveFrameHeaderHandler(const common::DataConstBuffer& buffer);
    void receiveFrameSizeHandler(const common::DataConstBuffer& buffer);
    void receiveFramePayloadHandler(const common::DataConstBuffer& buffer);

    boost::asio::io_service::strand strand_;
    transport::ITransport::Pointer transport_;
    ICryptor::Pointer cryptor_;
    FrameType recentFrameType_;
    ReceivePromise::Pointer promise_;
    Message::Pointer message_;
    std::map<messenger::ChannelId, Message::Pointer> messageBuffer_;
//    bool isStopping_;
};

}
}
