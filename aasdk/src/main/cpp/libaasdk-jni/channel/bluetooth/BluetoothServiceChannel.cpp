#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/BluetoothChannelMessageIdsEnum.pb.h>
#include <proto/BluetoothPairingRequestMessage.pb.h>
#include <channel/bluetooth/IBluetoothServiceChannelEventHandler.hpp>
#include <channel/bluetooth/BluetoothServiceChannel.hpp>
#include <Log.h>


namespace aasdk
{
namespace channel
{
namespace bluetooth
{

BluetoothServiceChannel::BluetoothServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::BLUETOOTH)
{

}

void BluetoothServiceChannel::receive(IBluetoothServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_);
    receivePromise->then(std::bind(&BluetoothServiceChannel::messageHandler, this, std::placeholders::_1, eventHandler),
                        std::bind(&IBluetoothServiceChannelEventHandler::onChannelError, eventHandler,std::placeholders::_1));

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

messenger::ChannelId BluetoothServiceChannel::getId() const
{
    return channelId_;
}

void BluetoothServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendChannelOpenResponse: %s", response.Utf8DebugString().c_str());

    auto message = std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL);
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void BluetoothServiceChannel::sendBluetoothPairingResponse(const proto::messages::BluetoothPairingResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendBluetoothPairingResponse: %s", response.Utf8DebugString().c_str());

    auto message = std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC);
    message->insertPayload(messenger::MessageId(proto::ids::BluetoothChannelMessage::PAIRING_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void BluetoothServiceChannel::messageHandler(messenger::Message::Pointer message, IBluetoothServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
    if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::BluetoothChannelMessage::Enum_Name(messageId.getId()).c_str());

    switch(messageId.getId())
    {
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::BluetoothChannelMessage::PAIRING_REQUEST:
        this->handleBluetoothPairingRequest(payload, std::move(eventHandler));
        break;
    default:
        Log_e("message not handled: %d", messageId.getId());
        this->receive(std::move(eventHandler));
        break;
    }
}

void BluetoothServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, IBluetoothServiceChannelEventHandler::Pointer eventHandler)
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

void BluetoothServiceChannel::handleBluetoothPairingRequest(const common::DataConstBuffer& payload, IBluetoothServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::BluetoothPairingRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleBluetoothPairingRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onBluetoothPairingRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

}
}
}
