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

class MessageInStream: public IMessageInStream, boost::noncopyable
{
public:
    MessageInStream(boost::asio::io_service& ioService, transport::ITransport::Pointer transport, ICryptor::Pointer cryptor);

    void startReceive(ReceivePromise::Pointer promise) override;

private:

    void receiveFrameHeaderHandler(const common::DataConstBuffer& buffer, ReceivePromise::Pointer promise);
    void receiveFrameSizeHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise);
    void receiveFramePayloadHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise);

    boost::asio::io_service::strand strand_;
    transport::ITransport::Pointer transport_;
    ICryptor::Pointer cryptor_;
    std::map<messenger::ChannelId, Message::Pointer> messageBuffer_;
};

}
}
