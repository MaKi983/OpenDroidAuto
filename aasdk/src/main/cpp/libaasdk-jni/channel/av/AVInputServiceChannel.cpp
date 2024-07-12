#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/AVChannelMessageIdsEnum.pb.h>
#include <messenger/Timestamp.hpp>
#include <channel/av/IAVInputServiceChannelEventHandler.hpp>
#include <channel/av/AVInputServiceChannel.hpp>
#include <Log.h>


namespace aasdk
{
namespace channel
{
namespace av
{

AVInputServiceChannel::AVInputServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::AV_INPUT)
{

}

void AVInputServiceChannel::receive(IAVInputServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_);
    receivePromise->then(std::bind(&AVInputServiceChannel::messageHandler, this->shared_from_this(), std::placeholders::_1, eventHandler),
                        std::bind(&IAVInputServiceChannelEventHandler::onChannelError, eventHandler, std::placeholders::_1));

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

void AVInputServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendChannelOpenResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void AVInputServiceChannel::sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendAVChannelSetupResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::SETUP_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

messenger::ChannelId AVInputServiceChannel::getId() const
{
    return channelId_;
}

void AVInputServiceChannel::messageHandler(messenger::Message::Pointer message, IAVInputServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
    if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::AVChannelMessage::Enum_Name(messageId.getId()).c_str());

    switch(messageId.getId())
    {
    case proto::ids::AVChannelMessage::SETUP_REQUEST:
        this->handleAVChannelSetupRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::AVChannelMessage::AV_INPUT_OPEN_REQUEST:
        this->handleAVInputOpenRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::AVChannelMessage::AV_MEDIA_ACK_INDICATION:
        this->handleAVMediaAckIndication(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, std::move(eventHandler));
        break;
    default:
        Log_e("message not handled: %d", messageId.getId());
        this->receive(std::move(eventHandler));
        break;
    }
}

void AVInputServiceChannel::sendAVInputOpenResponse(const proto::messages::AVInputOpenResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendAVInputOpenResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::AV_INPUT_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void AVInputServiceChannel::sendAVMediaWithTimestampIndication(messenger::Timestamp::ValueType timestamp, const common::Data& data, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendAVMediaWithTimestampIndication");

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::AV_MEDIA_WITH_TIMESTAMP_INDICATION).getData());

    auto timestampData = messenger::Timestamp(timestamp).getData();
    message->insertPayload(std::move(timestampData));
    message->insertPayload(data);

    this->send(std::move(message), std::move(promise));
}

void AVInputServiceChannel::handleAVChannelSetupRequest(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::AVChannelSetupRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleAVChannelSetupRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onAVChannelSetupRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void AVInputServiceChannel::handleAVInputOpenRequest(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::AVInputOpenRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleAVInputOpenRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onAVInputOpenRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void AVInputServiceChannel::handleAVMediaAckIndication(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::AVMediaAckIndication indication;
    if(indication.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleAVMediaAckIndication: %s", indication.Utf8DebugString().c_str());
        eventHandler->onAVMediaAckIndication(indication);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void AVInputServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::ChannelOpenRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleChannelOpenRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onChannelOpenRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

}
}
}
