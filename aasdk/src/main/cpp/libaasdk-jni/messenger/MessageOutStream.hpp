#pragma once

#include <common/Data.hpp>
#include <transport/ITransport.hpp>
#include <messenger/ICryptor.hpp>
#include <messenger/IMessageOutStream.hpp>
#include <messenger/FrameHeader.hpp>
#include <messenger/FrameSize.hpp>


namespace aasdk
{
namespace messenger
{

class MessageOutStream: public IMessageOutStream, public std::enable_shared_from_this<MessageOutStream>, boost::noncopyable
{
public:
    MessageOutStream(boost::asio::io_service& ioService, transport::ITransport::Pointer transport, ICryptor::Pointer cryptor);

    void stream(Message::Pointer message, SendPromise::Pointer promise) override;
//    void stop() override;
private:
    using std::enable_shared_from_this<MessageOutStream>::shared_from_this;

//    void streamSplittedMessage(int offset, int remainingSize);
//    common::Data compoundFrame(FrameType frameType, const common::DataConstBuffer& payloadBuffer);
    void streamSplittedMessage(Message::Pointer message, int offset, int remainingSize, SendPromise::Pointer promise);
    common::Data compoundFrame(Message::Pointer message, FrameType frameType, const common::DataConstBuffer& payloadBuffer);
//    void streamEncryptedFrame(FrameType frameType, const common::DataConstBuffer& payloadBuffer);
//    void streamPlainFrame(FrameType frameType, const common::DataConstBuffer& payloadBuffer);
    void setFrameSize(common::Data& data, FrameType frameType, size_t payloadSize, size_t totalSize);
    void reset();

    boost::asio::io_service::strand strand_;
    transport::ITransport::Pointer transport_;
    ICryptor::Pointer cryptor_;
//    Message::Pointer message_;
//    size_t offset_;
//    size_t remainingSize_;
//    SendPromise::Pointer promise_;
//    bool isStopping_;

    static constexpr size_t cMaxFramePayloadSize = 0x4000;
};

}
}
