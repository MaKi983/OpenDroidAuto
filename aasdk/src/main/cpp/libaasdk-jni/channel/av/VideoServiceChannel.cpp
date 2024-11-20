#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/AVChannelMessageIdsEnum.pb.h>
#include <messenger/Timestamp.hpp>
#include <channel/av/IVideoServiceChannelEventHandler.hpp>
#include <channel/av/VideoServiceChannel.hpp>
#include <Log.h>

namespace aasdk
{
namespace channel
{
namespace av
{

VideoServiceChannel::VideoServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::VIDEO)
{

}

void VideoServiceChannel::receive(IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_, "VideoServiceChannel_messageHandler");
    receivePromise->then(std::bind(&VideoServiceChannel::messageHandler, this->shared_from_this(), std::placeholders::_1, eventHandler),
                        std::bind(&IVideoServiceChannelEventHandler::onChannelError, eventHandler, std::placeholders::_1));

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

messenger::ChannelId VideoServiceChannel::getId() const
{
    return channelId_;
}

void VideoServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendChannelOpenResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendAVChannelSetupResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::SETUP_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::sendVideoFocusIndication(const proto::messages::VideoFocusIndication& indication, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendVideoFocusIndication: %s", indication.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::VIDEO_FOCUS_INDICATION).getData());
    message->insertPayload(indication);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::sendAVMediaAckIndication(const proto::messages::AVMediaAckIndication& indication, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendAVMediaAckIndication: %s", indication.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::AV_MEDIA_ACK_INDICATION).getData());
    message->insertPayload(indication);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::messageHandler(messenger::Message::Pointer message, IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
    if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::AVChannelMessage::Enum_Name(messageId.getId()).c_str());

    switch(messageId.getId())
    {
    case proto::ids::AVChannelMessage::SETUP_REQUEST:
        this->handleAVChannelSetupRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::AVChannelMessage::START_INDICATION:
        this->handleStartIndication(payload, std::move(eventHandler));
        break;
    case proto::ids::AVChannelMessage::STOP_INDICATION:
        this->handleStopIndication(payload, std::move(eventHandler));
        break;
    case proto::ids::AVChannelMessage::AV_MEDIA_WITH_TIMESTAMP_INDICATION:
        this->handleAVMediaWithTimestampIndication(payload, std::move(eventHandler));
        break;
    case proto::ids::AVChannelMessage::AV_MEDIA_INDICATION:
        eventHandler->onAVMediaIndication(payload);
        break;
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::AVChannelMessage::VIDEO_FOCUS_REQUEST:
        this->handleVideoFocusRequest(payload, std::move(eventHandler));
        break;
    default:
        Log_e("message not handled %d", messageId.getId());
//        this->receive(std::move(eventHandler));
        break;
    }
}

void VideoServiceChannel::handleAVChannelSetupRequest(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler)
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

void VideoServiceChannel::handleStartIndication(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::AVChannelStartIndication indication;
    if(indication.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleStartIndication: %s", indication.Utf8DebugString().c_str());
        eventHandler->onAVChannelStartIndication(indication);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void VideoServiceChannel::handleStopIndication(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::AVChannelStopIndication indication;
    if(indication.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleStopIndication: %s", indication.Utf8DebugString().c_str());
        eventHandler->onAVChannelStopIndication(indication);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void VideoServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler)
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

void VideoServiceChannel::handleVideoFocusRequest(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::VideoFocusRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleVideoFocusRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onVideoFocusRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void VideoServiceChannel::handleAVMediaWithTimestampIndication(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    if(payload.size >= sizeof(messenger::Timestamp::ValueType))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleAVMediaWithTimestampIndication");
        messenger::Timestamp timestamp(payload);
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleAVMediaWithTimestampIndication timestamp %lld", timestamp.getValue());
        eventHandler->onAVMediaWithTimestampIndication(timestamp.getValue(), common::DataConstBuffer(payload.cdata, payload.size, sizeof(messenger::Timestamp::ValueType)));
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

}
}
}
