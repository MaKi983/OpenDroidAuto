#include <messenger/MessageInStream.hpp>
#include <error/Error.hpp>
#include <Log.h>

namespace aasdk
{
namespace messenger
{

MessageInStream::MessageInStream(boost::asio::io_service& ioService, transport::ITransport::Pointer transport, ICryptor::Pointer cryptor)
    : strand_(ioService)
    , transport_(std::move(transport))
    , cryptor_(std::move(cryptor))
{

}

void MessageInStream::startReceive(ReceivePromise::Pointer promise)
{
    strand_.dispatch([this, promise = std::move(promise)]() mutable {
        if (Log::isDebug()) Log_d("startReceive");

        auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
        transportPromise->then(
            [this, promise = std::move(promise)](common::Data data) mutable {
                this->receiveFrameHeaderHandler(common::DataConstBuffer(data), promise);
            },
            [this, promise = std::move(promise)](const error::Error& e) mutable {
                promise->reject(e);
            });

        transport_->receive(FrameHeader::getSizeOf(), std::move(transportPromise));
    });
}

void MessageInStream::receiveFrameHeaderHandler(const common::DataConstBuffer& buffer, ReceivePromise::Pointer promise)
{
    FrameHeader frameHeader(buffer);
    if (Log::isVerbose()) Log_v("%s / receiveFrameHeaderHandler: %s", channelIdToString(frameHeader.getChannelId()).c_str(), frameHeader.toString().c_str());

    Message::Pointer message = nullptr;
    auto bufferedMessage = messageBuffer_.find(frameHeader.getChannelId());

    if(bufferedMessage != messageBuffer_.end())
    {
        if (Log::isVerbose()) Log_v("%s / found bufferedMessage", channelIdToString(frameHeader.getChannelId()).c_str());
        if(frameHeader.getType() != FrameType::FIRST && frameHeader.getType() != FrameType::BULK)
        {
            if (Log::isVerbose()) Log_v("%s / frameType is not FIRST and BULK", channelIdToString(frameHeader.getChannelId()).c_str());
            message = bufferedMessage->second;
        }
        else
        {
            if (Log::isVerbose()) Log_v("%s / frameType is FIRST or BULK, remove previous message in buffer and put new one", channelIdToString(frameHeader.getChannelId()).c_str());
            messageBuffer_.erase(bufferedMessage);
            message = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
            messageBuffer_[message->getChannelId()] = message;
        }
    }
    else if(message == nullptr)
    {
        if (Log::isVerbose()) Log_v("%s / add new message in bufferedMessage", channelIdToString(frameHeader.getChannelId()).c_str());
        message = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
        messageBuffer_[message->getChannelId()] = message;
    }

    const size_t frameSize = FrameSize::getSizeOf(frameHeader.getType() == FrameType::FIRST ? FrameSizeType::EXTENDED : FrameSizeType::SHORT);

    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
    transportPromise->then(
        [this, promise = std::move(promise), message = std::move(message), frameHeader = std::move(frameHeader)](common::Data data) mutable {
            this->receiveFrameSizeHandler(common::DataConstBuffer(data), message, frameHeader, promise);
        },
        [this, promise = std::move(promise), message = std::move(message)](const error::Error& e) mutable {
            message.reset();
            promise->reject(e);
        });

    transport_->receive(frameSize, std::move(transportPromise));
}

void MessageInStream::receiveFrameSizeHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise)
{
    if (Log::isVerbose()) Log_v("%s / receiveFrameSizeHandler", channelIdToString(frameHeader.getChannelId()).c_str());
    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
    transportPromise->then(
        [this, promise = std::move(promise), message = std::move(message), frameHeader = std::move(frameHeader)](common::Data data) mutable {
            this->receiveFramePayloadHandler(common::DataConstBuffer(data), std::move(message), frameHeader, promise);
        },
        [this, promise = std::move(promise), message = std::move(message)](const error::Error& e) mutable {

            message.reset();
            promise->reject(e);
        });

    FrameSize frameSize(buffer);
    if (Log::isVerbose()) Log_v("%s / frameSize %s", channelIdToString(frameHeader.getChannelId()).c_str(), frameSize.toString().c_str());

    transport_->receive(frameSize.getSize(), std::move(transportPromise));
}

void MessageInStream::receiveFramePayloadHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise)
{
    if (Log::isVerbose()) Log_v("%s / receiveFramePayloadHandler", channelIdToString(frameHeader.getChannelId()).c_str());

    if(message->getEncryptionType() == EncryptionType::ENCRYPTED)
    {
        try
        {
            if (Log::isDebug()) Log_d("%s / frame encrypted", channelIdToString(frameHeader.getChannelId()).c_str());
            cryptor_->decrypt(message->getPayload(), buffer);
        }
        catch(const error::Error& e)
        {
            message.reset();
            promise->reject(e);
            return;
        }
    }
    else
    {
        message->insertPayload(buffer);
    }

    if(frameHeader.getType() == FrameType::BULK || frameHeader.getType() == FrameType::LAST)
    {
        if (Log::isDebug()) Log_d("%s / message BULK or LAST, resolve: %s", channelIdToString(frameHeader.getChannelId()).c_str(), message->toString().c_str());
        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / plain payload: %s", channelIdToString(frameHeader.getChannelId()).c_str(), common::dump(message->getPayload()).c_str());
        messageBuffer_.erase(message->getChannelId());
        promise->resolve(std::move(message));
    }
    else
    {
        if (Log::isDebug()) Log_d("%s / message FIRST or MIDDLE, wait to receive remaining data", channelIdToString(frameHeader.getChannelId()).c_str());
        auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
        transportPromise->then(
            [this, promise = std::move(promise)](common::Data data) mutable {
                this->receiveFrameHeaderHandler(common::DataConstBuffer(data), promise);
            },
            [this, promise = std::move(promise), message = std::move(message)](const error::Error& e) mutable {
                message.reset();
                promise->reject(e);
            });

        transport_->receive(FrameHeader::getSizeOf(), std::move(transportPromise));
    }
}

}
}
