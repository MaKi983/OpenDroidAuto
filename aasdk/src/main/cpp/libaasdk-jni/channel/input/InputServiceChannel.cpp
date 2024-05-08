#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/ButtonCodeEnum.pb.h>
#include <proto/InputChannelMessageIdsEnum.pb.h>
#include <channel/input/InputServiceChannel.hpp>
#include <channel/input/IInputServiceChannelEventHandler.hpp>
#include <Log.h>

namespace aasdk
{
namespace channel
{
namespace input
{

InputServiceChannel::InputServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::INPUT)
{

}

void InputServiceChannel::receive(IInputServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_);
    receivePromise->then(std::bind(&InputServiceChannel::messageHandler, this, std::placeholders::_1, eventHandler),
                        std::bind(&IInputServiceChannelEventHandler::onChannelError, eventHandler, std::placeholders::_1));

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

messenger::ChannelId InputServiceChannel::getId() const
{
    return channelId_;
}

void InputServiceChannel::sendInputEventIndication(const proto::messages::InputEventIndication& indication, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendInputEventIndication: %s", indication.Utf8DebugString().c_str());

    auto message = std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC);
    message->insertPayload(messenger::MessageId(proto::ids::InputChannelMessage::INPUT_EVENT_INDICATION).getData());
    message->insertPayload(indication);

    this->send(std::move(message), std::move(promise));
}

void InputServiceChannel::sendBindingResponse(const proto::messages::BindingResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendBindingResponse: %s", response.Utf8DebugString().c_str());

    auto message = std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC);
    message->insertPayload(messenger::MessageId(proto::ids::InputChannelMessage::BINDING_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void InputServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendChannelOpenResponse: %s", response.Utf8DebugString().c_str());

    auto message = std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL);
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void InputServiceChannel::messageHandler(messenger::Message::Pointer message, IInputServiceChannelEventHandler::Pointer eventHandler)
{
    if (Log::isDebug()) Log_d("messageHandler: received message");
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
    if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::InputChannelMessage::Enum_Name(messageId.getId()).c_str());

    switch(messageId.getId())
    {
    case proto::ids::InputChannelMessage::BINDING_REQUEST:
        this->handleBindingRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, std::move(eventHandler));
        break;
    default:
        Log_e("message not handled %d", messageId.getId());
        this->receive(std::move(eventHandler));
        break;
    }
}

void InputServiceChannel::handleBindingRequest(const common::DataConstBuffer& payload, IInputServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::BindingRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleBindingRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onBindingRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void InputServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, IInputServiceChannelEventHandler::Pointer eventHandler)
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
