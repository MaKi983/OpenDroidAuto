#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/SensorChannelMessageIdsEnum.pb.h>
#include <channel/sensor/ISensorServiceChannelEventHandler.hpp>
#include <channel/sensor/SensorServiceChannel.hpp>
#include <Log.h>


namespace aasdk
{
namespace channel
{
namespace sensor
{

SensorServiceChannel::SensorServiceChannel(boost::optional<boost::asio::io_service::strand>& strand,  messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::SENSOR)
{
}

SensorServiceChannel::~SensorServiceChannel(){
    if (Log::isVerbose()) Log_v("destructor");
}

void SensorServiceChannel::receive(ISensorServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_, "SensorServiceChannel_messageHandler");
    receivePromise->then(std::bind(&SensorServiceChannel::messageHandler, this->shared_from_this(), std::placeholders::_1, eventHandler),
                        std::bind(&ISensorServiceChannelEventHandler::onChannelError, eventHandler, std::placeholders::_1));

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

messenger::ChannelId SensorServiceChannel::getId() const
{
    return channelId_;
}

void SensorServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendChannelOpenResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void SensorServiceChannel::messageHandler(messenger::Message::Pointer message, ISensorServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
    if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::SensorChannelMessage::Enum_Name(messageId.getId()).c_str());

    switch(messageId.getId())
    {
    case proto::ids::SensorChannelMessage::SENSOR_START_REQUEST:
        this->handleSensorStartRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, std::move(eventHandler));
        break;
    default:
        Log_e("message not handled: %d", messageId.getId());
//        this->receive(std::move(eventHandler));
        break;
    }
}

void SensorServiceChannel::sendSensorEventIndication(const proto::messages::SensorEventIndication& indication, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendSensorEventIndication: %s", indication.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::SensorChannelMessage::SENSOR_EVENT_INDICATION).getData());
    message->insertPayload(indication);

    this->send(std::move(message), std::move(promise));
}

void SensorServiceChannel::sendSensorStartResponse(const proto::messages::SensorStartResponseMessage& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendSensorStartResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::SensorChannelMessage::SENSOR_START_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void SensorServiceChannel::handleSensorStartRequest(const common::DataConstBuffer& payload, ISensorServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::SensorStartRequestMessage request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleSensorStartRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onSensorStartRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void SensorServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, ISensorServiceChannelEventHandler::Pointer eventHandler)
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
