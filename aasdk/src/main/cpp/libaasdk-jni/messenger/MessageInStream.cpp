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
//    , isStopping_(false)
{

}

void MessageInStream::startReceive(ReceivePromise::Pointer promise)
{
    strand_.dispatch([this, self = this->shared_from_this(), promise = std::move(promise)]() mutable {
        if (Log::isDebug()) Log_d("startReceive");
        if(promise_ == nullptr)
        {
            promise_ = std::move(promise);

            auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
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

//void MessageInStream::startReceive(ReceivePromise::Pointer promise)
//{
//    if (isStopping_){
//        if (Log::isInfo()) Log_i("stopping, return");
//        return;
//    }
//
//    strand_.dispatch([this, self = this->shared_from_this(), promise = std::move(promise)]() mutable {
//        if (Log::isDebug()) Log_d("startReceive");
//
//        if (isStopping_){
//            if (Log::isInfo()) Log_i("stopping, return");
//            return;
//        }
//
//        auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_, "MessageInStream-transportPromise");
//        transportPromise->then(
//            [this, self = this->shared_from_this(), promise = std::move(promise)](common::Data data) mutable {
//                this->receiveFrameHeaderHandler(common::DataConstBuffer(data), std::move(promise));
//            },
//            [this, self = this->shared_from_this(), promise = std::move(promise)](const error::Error& e) mutable {
//                Log_e("transportPromise error %s", e.what());
//                Log_e("promise null? %s", (promise.get() == nullptr ? "si" : "no"));
//                Log_e("transportPromise reject");
//                promise->reject(e);
//                Log_e("transportPromise rejected");
////                promise.reset();
//            });
//
//        transport_->receive(FrameHeader::getSizeOf(), std::move(transportPromise));
//    });
//}

void MessageInStream::receiveFrameHeaderHandler(const common::DataConstBuffer& buffer)
{
    FrameHeader frameHeader(buffer);
    if (Log::isVerbose()) Log_v("%s / receiveFrameHeaderHandler: %s", channelIdToString(frameHeader.getChannelId()).c_str(), frameHeader.toString().c_str());

    if(message_ != nullptr && message_->getChannelId() != frameHeader.getChannelId())
    {
        messageBuffer_[message_->getChannelId()] = message_;
        message_ = nullptr;
    }

    auto bufferedMessage = messageBuffer_.find(frameHeader.getChannelId());

    if(bufferedMessage != messageBuffer_.end())
    {
        if (Log::isVerbose()) Log_v("%s / found bufferedMessage", channelIdToString(frameHeader.getChannelId()).c_str());
        if(frameHeader.getType() != FrameType::FIRST && frameHeader.getType() != FrameType::BULK)
        {
            if (Log::isVerbose()) Log_v("%s / frameType is not FIRST and BULK", channelIdToString(frameHeader.getChannelId()).c_str());
            message_ = bufferedMessage->second;
        }
        else
        {
            if (Log::isVerbose()) Log_v("%s / frameType is FIRST or BULK, remove previous message in buffer and put new one", channelIdToString(frameHeader.getChannelId()).c_str());
//            messageBuffer_.erase(bufferedMessage);
            message_ = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
//            messageBuffer_[message_->getChannelId()] = message_;
        }
        messageBuffer_.erase(bufferedMessage);
    }
    else if(message_ == nullptr)
    {
        message_ = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
    }

    recentFrameType_ = frameHeader.getType();
    const size_t frameSize = FrameSize::getSizeOf(frameHeader.getType() == FrameType::FIRST ? FrameSizeType::EXTENDED : FrameSizeType::SHORT);

    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
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

//void MessageInStream::receiveFrameHeaderHandler(const common::DataConstBuffer& buffer, ReceivePromise::Pointer promise)
//{
//    FrameHeader frameHeader(buffer);
//    if (Log::isVerbose()) Log_v("%s / receiveFrameHeaderHandler: %s", channelIdToString(frameHeader.getChannelId()).c_str(), frameHeader.toString().c_str());
//
//    Message::Pointer message = nullptr;
//    auto bufferedMessage = messageBuffer_.find(frameHeader.getChannelId());
//
//    if(bufferedMessage != messageBuffer_.end())
//    {
//        if (Log::isVerbose()) Log_v("%s / found bufferedMessage", channelIdToString(frameHeader.getChannelId()).c_str());
//        if(frameHeader.getType() != FrameType::FIRST && frameHeader.getType() != FrameType::BULK)
//        {
//            if (Log::isVerbose()) Log_v("%s / frameType is not FIRST and BULK", channelIdToString(frameHeader.getChannelId()).c_str());
//            message = bufferedMessage->second;
//        }
//        else
//        {
//            if (Log::isVerbose()) Log_v("%s / frameType is FIRST or BULK, remove previous message in buffer and put new one", channelIdToString(frameHeader.getChannelId()).c_str());
//            messageBuffer_.erase(bufferedMessage);
//            message = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
//            messageBuffer_[message->getChannelId()] = message;
//        }
//    }
//    else if(message == nullptr)
//    {
//        if (Log::isVerbose()) Log_v("%s / add new message in bufferedMessage", channelIdToString(frameHeader.getChannelId()).c_str());
//        message = std::make_shared<Message>(frameHeader.getChannelId(), frameHeader.getEncryptionType(), frameHeader.getMessageType());
//        messageBuffer_[message->getChannelId()] = message;
//    }
//
//    const size_t frameSize = FrameSize::getSizeOf(frameHeader.getType() == FrameType::FIRST ? FrameSizeType::EXTENDED : FrameSizeType::SHORT);
//
//    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
//    transportPromise->then(
//        [this, self = this->shared_from_this(), promise = std::move(promise), message = std::move(message), frameHeader = std::move(frameHeader)](common::Data data) mutable {
//            this->receiveFrameSizeHandler(common::DataConstBuffer(data), std::move(message), frameHeader, std::move(promise));
//        },
//        [this, self = this->shared_from_this(), promise = std::move(promise), message = std::move(message)](const error::Error& e) mutable {
//            Log_e("transportPromise2 reject %s", e.what());
//            message.reset();
//            promise->reject(e);
//			promise.reset();
//        });
//
//    transport_->receive(frameSize, std::move(transportPromise));
//}

void MessageInStream::receiveFrameSizeHandler(const common::DataConstBuffer& buffer)
{
    FrameHeader frameHeader(buffer);
    if (Log::isVerbose()) Log_v("%s / receiveFrameSizeHandler", channelIdToString(frameHeader.getChannelId()).c_str());
    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
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
    if (Log::isVerbose()) Log_v("%s / frameSize %s", channelIdToString(frameHeader.getChannelId()).c_str(), frameSize.toString().c_str());
    transport_->receive(frameSize.getSize(), std::move(transportPromise));
}

//void MessageInStream::receiveFrameSizeHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise)
//{
//    if (Log::isVerbose()) Log_v("%s / receiveFrameSizeHandler", channelIdToString(frameHeader.getChannelId()).c_str());
//    auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
//    transportPromise->then(
//        [this, self = this->shared_from_this(), promise = std::move(promise), message = std::move(message), frameHeader = std::move(frameHeader)](common::Data data) mutable {
//            this->receiveFramePayloadHandler(common::DataConstBuffer(data), std::move(message), frameHeader, std::move(promise));
//        },
//        [this, self = this->shared_from_this(), promise = std::move(promise), message = std::move(message)](const error::Error& e) mutable {
//            Log_e("transportPromise3 reject %s", e.what());
//            message.reset();
//            promise->reject(e);
//            promise.reset();
//        });
//
//    FrameSize frameSize(buffer);
//    if (Log::isVerbose()) Log_v("%s / frameSize %s", channelIdToString(frameHeader.getChannelId()).c_str(), frameSize.toString().c_str());
//    if (Log::isVerbose()) Log_v("%s / transport_ is null %s", channelIdToString(frameHeader.getChannelId()).c_str(), (transport_ == nullptr ? "true" : "false"));
//    transport_->receive(frameSize.getSize(), std::move(transportPromise));
//}

void MessageInStream::receiveFramePayloadHandler(const common::DataConstBuffer& buffer) {
    FrameHeader frameHeader(buffer);
    if (Log::isVerbose()) Log_v("%s / receiveFramePayloadHandler", channelIdToString(frameHeader.getChannelId()).c_str());

    if (message_->getEncryptionType() == EncryptionType::ENCRYPTED) {
        try {
            if (Log::isDebug()) Log_d("%s / frame encrypted", channelIdToString(frameHeader.getChannelId()).c_str());
            cryptor_->decrypt(message_->getPayload(), buffer);
        }
        catch (const error::Error &e) {
            message_.reset();
            promise_->reject(e);
            promise_.reset();
            return;
        }
    } else {
        message_->insertPayload(buffer);
    }

    if (recentFrameType_ == FrameType::BULK || recentFrameType_ == FrameType::LAST) {
        if (Log::isDebug()) Log_d("%s / message BULK or LAST, resolve: %s", channelIdToString(frameHeader.getChannelId()).c_str(), message_->toString().c_str());
        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / plain payload: %s", channelIdToString(frameHeader.getChannelId()).c_str(), common::dump(message_->getPayload()).c_str());

        promise_->resolve(std::move(message_));
        promise_.reset();
    } else {
        if (Log::isDebug()) Log_d("%s / message FIRST or MIDDLE, wait to receive remaining data", channelIdToString(frameHeader.getChannelId()).c_str());
        auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
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

//void MessageInStream::receiveFramePayloadHandler(const common::DataConstBuffer& buffer, Message::Pointer message, FrameHeader& frameHeader, ReceivePromise::Pointer promise)
//{
//    if (Log::isVerbose()) Log_v("%s / receiveFramePayloadHandler", channelIdToString(frameHeader.getChannelId()).c_str());
//
//    if(message->getEncryptionType() == EncryptionType::ENCRYPTED)
//    {
//        try
//        {
//            if (Log::isDebug()) Log_d("%s / frame encrypted", channelIdToString(frameHeader.getChannelId()).c_str());
//            cryptor_->decrypt(message->getPayload(), buffer);
//        }
//        catch(const error::Error& e)
//        {
//            Log_e("receiveFramePayload error %s", e.what());
//            message.reset();
//            promise->reject(e);
//			promise.reset();
//            return;
//        }
//    }
//    else
//    {
//        message->insertPayload(buffer);
//    }
//
//    if(frameHeader.getType() == FrameType::BULK || frameHeader.getType() == FrameType::LAST)
//    {
//        if (Log::isDebug()) Log_d("%s / message BULK or LAST, resolve: %s", channelIdToString(frameHeader.getChannelId()).c_str(), message->toString().c_str());
//        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s / plain payload: %s", channelIdToString(frameHeader.getChannelId()).c_str(), common::dump(message->getPayload()).c_str());
//        messageBuffer_.erase(message->getChannelId());
//        promise->resolve(std::move(message));
//        promise.reset();
//    }
//    else
//    {
//        if (Log::isDebug()) Log_d("%s / message FIRST or MIDDLE, wait to receive remaining data", channelIdToString(frameHeader.getChannelId()).c_str());
//        auto transportPromise = transport::ITransport::ReceivePromise::defer(strand_);
//        transportPromise->then(
//            [this, self = this->shared_from_this(), promise = std::move(promise)](common::Data data) mutable {
//                this->receiveFrameHeaderHandler(common::DataConstBuffer(data), std::move(promise));
//            },
//            [this, self = this->shared_from_this(), promise = std::move(promise), message = std::move(message)](const error::Error& e) mutable {
//                Log_e("transportPromise4 reject %s", e.what());
//                message.reset();
//                promise->reject(e);
//				promise.reset();
//            });
//
//        transport_->receive(FrameHeader::getSizeOf(), std::move(transportPromise));
//    }
//}

//void MessageInStream::stop() {
//    if(Log::isVerbose()) Log_v("set stop flag");
//}

}
}
