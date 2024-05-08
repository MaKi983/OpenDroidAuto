#include <boost/endian/conversion.hpp>
#include <io/PromiseLink.hpp>
#include <messenger/MessageOutStream.hpp>
#include <Log.h>

namespace aasdk
{
namespace messenger
{

MessageOutStream::MessageOutStream(boost::asio::io_service& ioService, transport::ITransport::Pointer transport, ICryptor::Pointer cryptor)
    : strand_(ioService)
    , transport_(std::move(transport))
    , cryptor_(std::move(cryptor))
{

}

void MessageOutStream::stream(Message::Pointer message, SendPromise::Pointer promise)
{
    strand_.dispatch([this, message = std::move(message), promise = std::move(promise)]() mutable {
        if (Log::isDebug()) Log_d("%s / sending message %s", channelIdToString(message->getChannelId()).c_str(), message->toString().c_str());

        if(message->getPayload().size() >= cMaxFramePayloadSize)
        {
            int offset = 0;
            int remainingSize = message->getPayload().size();
            this->streamSplittedMessage(std::move(message), offset, remainingSize, std::move(promise));
        }
        else
        {
            try
            {
                if (Log::isDebug()) Log_d("%s / send full BULK message", channelIdToString(message->getChannelId()).c_str());
                auto data(this->compoundFrame(message, FrameType::BULK, common::DataConstBuffer(message->getPayload())));

                auto transportPromise = transport::ITransport::SendPromise::defer(strand_);
                io::PromiseLink<>::forward(*transportPromise, std::move(promise));

                if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / sending data: %s", channelIdToString(message->getChannelId()).c_str(), common::dump(data).c_str());
                transport_->send(std::move(data), std::move(transportPromise));
            }
            catch(const error::Error& e)
            {
                promise->reject(e);
            }

            message.reset();
        }
    });
}

void MessageOutStream::streamSplittedMessage(Message::Pointer message, int offset, int remainingSize, SendPromise::Pointer promise)
{
    if(Log::isDebug()) Log_d("%s / message to big, split it: offset= %d, remainingSize= %d", channelIdToString(message->getChannelId()).c_str(), offset, remainingSize);
    try
    {
        const auto& payload = message->getPayload();
        auto ptr = &payload[offset];
        auto size = remainingSize < cMaxFramePayloadSize ? remainingSize : cMaxFramePayloadSize;

        FrameType frameType = offset == 0 ? FrameType::FIRST : (remainingSize - size > 0 ? FrameType::MIDDLE : FrameType::LAST);
        if (Log::isDebug()) Log_d("%s / frameType: %s", channelIdToString(message->getChannelId()).c_str(), frameTypeToString(frameType).c_str());

        auto data(this->compoundFrame(std::move(message), frameType, common::DataConstBuffer(ptr, size)));

        auto transportPromise = transport::ITransport::SendPromise::defer(strand_);

        if(frameType == FrameType::LAST)
        {
            io::PromiseLink<>::forward(*transportPromise, std::move(promise));
        }
        else
        {
            transportPromise->then([this, message = std::move(message), size, offset, remainingSize, promise = std::move(promise)]() mutable {
                    this->streamSplittedMessage(std::move(message), offset+size, remainingSize-size, promise);
                },
                [this, promise = std::move(promise)](const error::Error& e) mutable {
                    this->reset();
                    promise->reject(e);
                });
        }

        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / sending data: %s", channelIdToString(message->getChannelId()).c_str(), common::dump(data).c_str());

        transport_->send(std::move(data), std::move(transportPromise));
    }
    catch(const error::Error& e)
    {
        message.reset();
        promise->reject(e);
    }
}

common::Data MessageOutStream::compoundFrame(Message::Pointer message, FrameType frameType, const common::DataConstBuffer& payloadBuffer)
{
    const FrameHeader frameHeader(message->getChannelId(), frameType, message->getEncryptionType(), message->getType());
    common::Data data(frameHeader.getData());
    data.resize(data.size() + FrameSize::getSizeOf(frameType == FrameType::FIRST ? FrameSizeType::EXTENDED : FrameSizeType::SHORT));
    size_t payloadSize = 0;

    if(message->getEncryptionType() == EncryptionType::ENCRYPTED)
    {
        payloadSize = cryptor_->encrypt(data, payloadBuffer);
    }
    else
    {
        data.insert(data.end(), payloadBuffer.cdata, payloadBuffer.cdata + payloadBuffer.size);
        payloadSize = payloadBuffer.size;
    }

    this->setFrameSize(data, frameType, payloadSize, message->getPayload().size());
    return data;
}

void MessageOutStream::setFrameSize(common::Data& data, FrameType frameType, size_t payloadSize, size_t totalSize)
{
    const auto& frameSize = frameType == FrameType::FIRST ? FrameSize(payloadSize, totalSize) : FrameSize(payloadSize);
    const auto& frameSizeData = frameSize.getData();
    memcpy(&data[FrameHeader::getSizeOf()], &frameSizeData[0], frameSizeData.size());
}

void MessageOutStream::reset()
{
}

}
}
