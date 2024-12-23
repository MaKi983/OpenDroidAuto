#include <boost/endian/conversion.hpp>
#include <io/PromiseLink.hpp>
#include <messenger/MessageOutStream.hpp>
#include <Log.h>

namespace aasdk
{
namespace messenger
{

MessageOutStream::MessageOutStream(aasdk::io::ioService& ioService, transport::ITransport::Pointer transport, ICryptor::Pointer cryptor)
    : strand_(ioService)
    , transport_(std::move(transport))
    , cryptor_(std::move(cryptor))
    , isStopping_(false)
{
}

MessageOutStream::~MessageOutStream(){
    if (Log::isVerbose()) Log_v("destructor");
}

void MessageOutStream::stream(Message::Pointer message, SendPromise::Pointer promise)
{
    if (isStopping_) {
        if (Log::isInfo()) Log_i("MessageOutStream stopped");
        return;
    }

    strand_->dispatch([this, self = this->shared_from_this(), message = std::move(message), promise = std::move(promise)]() mutable {
        if (Log::isDebug()) Log_d("%s / sending message %s", channelIdToString(message->getChannelId()).c_str(), message->toString().c_str());

        if(message->getPayload().size() >= cMaxFramePayloadSize)
        {
            if (Log::isDebug()) Log_d("%s / split message", channelIdToString(message->getChannelId()).c_str());
            this->streamSplittedMessage(std::move(message), 0, message->getPayload().size(), std::move(promise));
        }
        else
        {
            try
            {
                if (Log::isDebug()) Log_d("%s / send full BULK message", channelIdToString(message->getChannelId()).c_str());
                auto data(this->compoundFrame(message, FrameType::BULK, common::DataConstBuffer(message->getPayload())));

                auto transportPromise = transport::ITransport::SendPromise::defer(strand_, "MessageOutStream_sendTransport");
                io::PromiseLink<>::forward(*transportPromise, std::move(promise));

                if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / sending data: %s", channelIdToString(message->getChannelId()).c_str(), common::dump(data).c_str());
                transport_->send(std::move(data), std::move(transportPromise));
            }
            catch(const error::Error& e)
            {
                promise->reject(e);
                promise.reset();
            }

            this->reset();
        }
    });
}

void MessageOutStream::streamSplittedMessage(Message::Pointer message, int offset, int remainingSize, SendPromise::Pointer promise) {
    if(Log::isDebug()) Log_d("%s / message too big, split it: offset= %d, remainingSize= %d", channelIdToString(message->getChannelId()).c_str(), offset, remainingSize);
    try
    {
        const auto& payload = message->getPayload();
        auto ptr = &payload[offset];
        auto size = remainingSize < cMaxFramePayloadSize ? remainingSize : cMaxFramePayloadSize;

        FrameType frameType = offset == 0 ? FrameType::FIRST : (remainingSize - size > 0 ? FrameType::MIDDLE : FrameType::LAST);
        if (Log::isDebug()) Log_d("%s / frameType: %s", channelIdToString(message->getChannelId()).c_str(), frameTypeToString(frameType).c_str());
        auto data(this->compoundFrame(message, frameType, common::DataConstBuffer(ptr, size)));

        auto transportPromise = transport::ITransport::SendPromise::defer(strand_, "MessageOutStream_sendTransport1");

        if(frameType == FrameType::LAST)
        {
            this->reset();
            io::PromiseLink<>::forward(*transportPromise, std::move(promise));
        }
        else
        {
            transportPromise->then([this, self = this->shared_from_this(), offset = offset, remainingSize = remainingSize, size, message = std::move(message), promise = std::move(promise)]() mutable {
               this->streamSplittedMessage(message, offset+size, remainingSize-size, promise);
           },
           [this, self = this->shared_from_this(), promise = std::move(promise)](const error::Error& e) mutable {
               this->reset();
               promise->reject(e);
               promise.reset();
           });
        }
        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / sending data: %s", channelIdToString(message->getChannelId()).c_str(), common::dump(data).c_str());

        transport_->send(std::move(data), std::move(transportPromise));
    }
    catch(const error::Error& e)
    {
        this->reset();
        promise->reject(e);
        promise.reset();
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
//    offset_ = 0;
//    remainingSize_ = 0;
//    message_.reset();
}

void MessageOutStream::stop() {
    if(Log::isVerbose()) Log_v("stop");
    isStopping_ = true;
    strand_ = boost::none;
}


}
}
