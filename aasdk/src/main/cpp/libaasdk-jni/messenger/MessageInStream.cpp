#include <messenger/MessageInStream.hpp>
#include <error/Error.hpp>
#include <Log.h>

namespace aasdk
{
namespace messenger
{

MessageInStream::MessageInStream(aasdk::io::ioService& ioService, transport::ITransport::Pointer transport, ICryptor::Pointer cryptor)
    : strand_(ioService)
    , transport_(std::move(transport))
    , cryptor_(std::move(cryptor))
    , isStopping_(false)
{
}

MessageInStream::~MessageInStream(){
    if (Log::isVerbose()) Log_v("destructor");
}

void MessageInStream::startReceive(ReceivePromise::Pointer promise)
{
    if (isStopping_) {
        if (Log::isInfo()) Log_i("Messenger stopped");
        return;
    }

    strand_->dispatch([this, self = this->shared_from_this(), promise = std::move(promise)]() mutable {
        if (Log::isDebug()) Log_d("startReceive");
        if(promise_ == nullptr)
        {
            promise_ = std::move(promise);

            auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_, "MessageInStream_receive");
            transportPromise->then(
                    [this, self = this->shared_from_this()](common::Data data) mutable {
                        this->receiveFrameHeaderHandler(common::DataConstBuffer(data));
                    },
                    [this, self = this->shared_from_this()](const error::Error& e) mutable {
                        promise_->reject(e);
                        promise_.reset();
                    });

            transport_->receive(FrameHeader::getSizeOf(), std::move(transportPromise));
        }
        else
        {
            promise->reject(error::Error(error::ErrorCode::OPERATION_IN_PROGRESS));
        }
    });
}

void MessageInStream::receiveFrameHeaderHandler(const common::DataConstBuffer& buffer) {
    FrameHeader frameHeader(buffer);
    if (Log::isDebug()) Log_d("%s / receiveFrameHeaderHandler: %s", channelIdToString(frameHeader.getChannelId()).c_str(), frameHeader.toString().c_str());
    if (Log::isVerbose() && Log::logProtocol()) Log_v("%s/receiveFrameHeaderHandler %s", channelIdToString(frameHeader.getChannelId()).c_str(), common::dump(buffer).c_str());

    if(message_ != nullptr && message_->getChannelId() != frameHeader.getChannelId())
    {
        if (Log::isDebug()) Log_d("%s / message already present, add it to buffer", channelIdToString(message_->getChannelId()).c_str());
        messageBuffer_[message_->getChannelId()] = message_;
        message_ = nullptr;
    }

    auto bufferedMessage = messageBuffer_.find(frameHeader.getChannelId());

    if(bufferedMessage != messageBuffer_.end())
    {
        if(frameHeader.getType() != FrameType::FIRST && frameHeader.getType() != FrameType::BULK)
        {
            if (Log::isDebug()) Log_d("%s / frameType is not FIRST and BULK", channelIdToString(frameHeader.getChannelId()).c_str());
            message_ = bufferedMessage->second;
        }
        else
        {
            if (Log::isDebug()) Log_d("%s / frameType is FIRST or BULK, remove previous message in buffer and put new one", channelIdToString(frameHeader.getChannelId()).c_str());
//            messageBuffer_.erase(bufferedMessage);
            message_ = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
//            messageBuffer_[frameHeader.getChannelId()] = message_;
        }
        messageBuffer_.erase(bufferedMessage);
    }
    else if(message_ == nullptr)
    {
        if (Log::isDebug()) Log_d("%s / add new message in bufferedMessage", channelIdToString(frameHeader.getChannelId()).c_str());
        message_ = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
//        messageBuffer_[frameHeader.getChannelId()] = message_;
    }

    recentFrameType_ = frameHeader.getType();
    const size_t frameSize = FrameSize::getSizeOf(frameHeader.getType() == FrameType::FIRST ? FrameSizeType::EXTENDED : FrameSizeType::SHORT);

    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_, "MessageInStream_frameSizeHandler");
    transportPromise->then(
        [this, self = this->shared_from_this()](common::Data data) mutable {
            this->receiveFrameSizeHandler(common::DataConstBuffer(data));
        },
        [this, self = this->shared_from_this()](const error::Error& e) mutable {
            message_.reset();
            promise_->reject(e);
            promise_.reset();
        });

    transport_->receive(frameSize, std::move(transportPromise));
}

void MessageInStream::receiveFrameSizeHandler(const common::DataConstBuffer& buffer)
{
    if (Log::isDebug()) Log_d("%s / receiveFrameSizeHandler", channelIdToString(message_->getChannelId()).c_str());
    if (Log::isVerbose() && Log::logProtocol()) Log_v("%s/receiveFrameSizeHandler %s", channelIdToString(message_->getChannelId()).c_str(), common::dump(buffer).c_str());
    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_, "MessageInStream_framePayloadHandler");
    transportPromise->then(
            [this, self = this->shared_from_this()](common::Data data) mutable {
                this->receiveFramePayloadHandler(common::DataConstBuffer(data));
            },
            [this, self = this->shared_from_this()](const error::Error& e) mutable {
                message_.reset();
                promise_->reject(e);
                promise_.reset();
            });

    FrameSize frameSize(buffer);
    if (Log::isVerbose()) Log_v("%s / frameSize %s", channelIdToString(message_->getChannelId()).c_str(), frameSize.toString().c_str());
    transport_->receive(frameSize.getSize(), std::move(transportPromise));
}

void MessageInStream::receiveFramePayloadHandler(const common::DataConstBuffer& buffer) {
    if (Log::isDebug()) Log_d("%s / receiveFramePayloadHandler", channelIdToString(message_->getChannelId()).c_str());
    if (Log::isVerbose() && Log::logProtocol()) Log_v("%s/receiveFramePayloadHandler %s", channelIdToString(message_->getChannelId()).c_str(), common::dump(buffer).c_str());

    if (message_->getEncryptionType() == EncryptionType::ENCRYPTED) {
        try {
            if (Log::isDebug()) Log_d("%s / frame encrypted", channelIdToString(message_->getChannelId()).c_str());
            cryptor_->decrypt(message_->getPayload(), buffer);
        }
        catch (const error::Error &e) {
            message_.reset();
            promise_->reject(e);
            promise_.reset();
            return;
        }
    } else {
        if (Log::isDebug()) Log_d("%s / frame not encrypted, add payload to message", channelIdToString(message_->getChannelId()).c_str());
        message_->insertPayload(buffer);
    }

    if (recentFrameType_ == FrameType::BULK || recentFrameType_ == FrameType::LAST) {
        if (Log::isDebug()) Log_d("%s / message BULK or LAST, resolve: %s", channelIdToString(message_->getChannelId()).c_str(), message_->toString().c_str());
        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / plain payload: %s", channelIdToString(message_->getChannelId()).c_str(), common::dump(message_->getPayload()).c_str());

        promise_->resolve(std::move(message_));
//        messageBuffer_[frameHeader.getChannelId()] = nullptr;
        promise_.reset();
    } else {
        if (Log::isDebug()) Log_d("%s / message FIRST or MIDDLE, wait to receive remaining data", channelIdToString(message_->getChannelId()).c_str());
//        messageBuffer_[frameHeader.getChannelId()] = message;
        auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_, "MessageInStream_frameHeaderHandler_continue");
        transportPromise->then(
                [this, self = this->shared_from_this()](common::Data data) mutable {
                    this->receiveFrameHeaderHandler(common::DataConstBuffer(data));
                },
                [this, self = this->shared_from_this()](const error::Error &e) mutable {
                    message_.reset();
                    promise_->reject(e);
                    promise_.reset();
                });

        transport_->receive(FrameHeader::getSizeOf(), std::move(transportPromise));
    }
}

void MessageInStream::stop() {
    if(Log::isVerbose()) Log_v("stop");
    isStopping_ = true;
    strand_ = boost::none;
}

}
}
