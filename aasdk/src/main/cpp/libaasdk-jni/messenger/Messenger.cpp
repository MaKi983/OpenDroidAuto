#include <boost/endian/conversion.hpp>
#include <error/Error.hpp>
#include <messenger/Messenger.hpp>
#include <Log.h>

namespace aasdk
{
namespace messenger
{

Messenger::Messenger(boost::asio::io_service& ioService, IMessageInStream::Pointer messageInStream, IMessageOutStream::Pointer messageOutStream)
    : receiveStrand_(ioService)
    , sendStrand_(ioService)
    , messageInStream_(std::move(messageInStream))
    , messageOutStream_(std::move(messageOutStream))
    , isStopping_(false)
{

}

Messenger::~Messenger(){
    channelReceivePromiseQueue_.clear();
    channelReceiveMessageQueue_.clear();
    channelSendPromiseQueue_.clear();
//    delete messageInStream_;
//    delete messageOutStream_;
//	messageInStream_.reset();
//	messageOutStream_.reset();
}

void Messenger::enqueueReceive(ChannelId channelId, ReceivePromise::Pointer promise)
{
    if (isStopping_) {
        return;
    }

    receiveStrand_.dispatch([this, self = this->shared_from_this(), channelId, promise = std::move(promise)]() mutable {
        if(Log::isDebug()) Log_d("%s / enqueueReceive", channelIdToString(channelId).c_str());
        if(!channelReceiveMessageQueue_.empty(channelId))
        {
            promise->resolve(std::move(channelReceiveMessageQueue_.pop(channelId)));
        }
        else
        {
            channelReceivePromiseQueue_.push(channelId, std::move(promise));

            if(channelReceivePromiseQueue_.size() == 1)
            {

                auto inStreamPromise = ReceivePromise::defer(receiveStrand_);
                inStreamPromise->then(std::bind(&Messenger::inStreamMessageHandler, this->shared_from_this(), std::placeholders::_1),
                                     std::bind(&Messenger::rejectReceivePromiseQueue, this->shared_from_this(), std::placeholders::_1));
                messageInStream_->startReceive(std::move(inStreamPromise));
            }
        }
    });
}

void Messenger::enqueueSend(Message::Pointer message, SendPromise::Pointer promise)
{
    if (isStopping_) {
        return;
    }

    sendStrand_.dispatch([this, self = this->shared_from_this(), message = std::move(message), promise = std::move(promise)]() mutable {
        channelSendPromiseQueue_.emplace_back(std::make_pair(std::move(message), std::move(promise)));

        if(channelSendPromiseQueue_.size() == 1)
        {
            this->doSend();
        }
    });
}

void Messenger::inStreamMessageHandler(Message::Pointer message)
{
    auto channelId = message->getChannelId();

    if(channelReceivePromiseQueue_.isPending(channelId))
    {
        channelReceivePromiseQueue_.pop(channelId)->resolve(std::move(message));
    }
    else
    {
        channelReceiveMessageQueue_.push(std::move(message));
    }

    if(!channelReceivePromiseQueue_.empty())
    {
        auto inStreamPromise = ReceivePromise::defer(receiveStrand_);
        inStreamPromise->then(std::bind(&Messenger::inStreamMessageHandler, this->shared_from_this(), std::placeholders::_1),
                             std::bind(&Messenger::rejectReceivePromiseQueue, this->shared_from_this(), std::placeholders::_1));
        messageInStream_->startReceive(std::move(inStreamPromise));
    }
}

void Messenger::doSend()
{
    auto queueElementIter = channelSendPromiseQueue_.begin();
    auto outStreamPromise = SendPromise::defer(sendStrand_);
    outStreamPromise->then(std::bind(&Messenger::outStreamMessageHandler, this->shared_from_this(), queueElementIter),
                           std::bind(&Messenger::rejectSendPromiseQueue, this->shared_from_this(), std::placeholders::_1));

    messageOutStream_->stream(std::move(queueElementIter->first), std::move(outStreamPromise));
}

void Messenger::outStreamMessageHandler(ChannelSendQueue::iterator queueElement)
{
    queueElement->second->resolve();
    channelSendPromiseQueue_.erase(queueElement);
    if(!channelSendPromiseQueue_.empty())
    {
        this->doSend();
    }
}

void Messenger::rejectReceivePromiseQueue(const error::Error& e)
{
    while(!channelReceivePromiseQueue_.empty())
    {
        channelReceivePromiseQueue_.pop()->reject(e);
    }
}

void Messenger::rejectSendPromiseQueue(const error::Error& e)
{
    while(!channelSendPromiseQueue_.empty())
    {
        auto queueElement(std::move(channelSendPromiseQueue_.front()));
        channelSendPromiseQueue_.pop_front();
        queueElement.second->reject(e);
    }
}

void Messenger::stop()
{
    if (Log::isInfo()) Log_i("Stop messenger");

//    isStopping_ = true;
//    messageOutStream_->stop();
//    messageInStream_->stop();
//    receiveStrand_.dispatch([this, self = this->shared_from_this()]() {
    channelReceiveMessageQueue_.clear();
//    channelSendPromiseQueue_.clear();
//    channelReceivePromiseQueue_.clear();
//    });
}

}
}
