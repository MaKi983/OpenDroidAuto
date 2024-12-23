#include <boost/endian/conversion.hpp>
#include <proto/ControlMessageIdsEnum.pb.h>
#include <Version.hpp>
#include <io/PromiseLink.hpp>
#include <channel/control/ControlServiceChannel.hpp>
#include <channel/control/IControlServiceChannelEventHandler.hpp>
#include <Log.h>


namespace aasdk
{
namespace channel
{
namespace control
{

ControlServiceChannel::ControlServiceChannel(io::strand& strand, messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::CONTROL)
{

}

void ControlServiceChannel::sendVersionRequest(SendPromise::Pointer promise)
{
    if (Log::isDebug()) Log_d("sendVersionRequest");

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::PLAIN, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::VERSION_REQUEST).getData());

    common::Data versionBuffer(4, 0);
    reinterpret_cast<uint16_t&>(versionBuffer[0]) = boost::endian::native_to_big(AASDK_MAJOR);
    reinterpret_cast<uint16_t&>(versionBuffer[2]) = boost::endian::native_to_big(AASDK_MINOR);
    message->insertPayload(versionBuffer);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendHandshake(common::Data handshakeBuffer, SendPromise::Pointer promise)
{
    if (Log::isDebug()) Log_d("sendHandshake");

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::PLAIN, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::SSL_HANDSHAKE).getData());
    message->insertPayload(handshakeBuffer);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendAuthComplete(const proto::messages::AuthCompleteIndication& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendAuthComplete: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::PLAIN, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::AUTH_COMPLETE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendServiceDiscoveryResponse(const proto::messages::ServiceDiscoveryResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendServiceDiscoveryResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::SERVICE_DISCOVERY_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendAudioFocusResponse(const proto::messages::AudioFocusResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendAudioFocusResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::AUDIO_FOCUS_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendShutdownRequest(const proto::messages::ShutdownRequest& request, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendShutdownRequest: %s", request.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::SHUTDOWN_REQUEST).getData());
    message->insertPayload(request);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendShutdownResponse(const proto::messages::ShutdownResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendShutdownResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::SHUTDOWN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendNavigationFocusResponse(const proto::messages::NavigationFocusResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendNavigationFocusResponse: %s", response.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::NAVIGATION_FOCUS_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendPingResponse(const proto::messages::PingResponse& request, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendPingResponse: %s", request.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::PLAIN, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::PING_RESPONSE).getData());
    message->insertPayload(request);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::sendPingRequest(const proto::messages::PingRequest& request, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendPingRequest: %s", request.Utf8DebugString().c_str());

    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::PLAIN, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::PING_REQUEST).getData());
    message->insertPayload(request);

    this->send(std::move(message), std::move(promise));
}

void ControlServiceChannel::receive(IControlServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise  = messenger::ReceivePromise::defer(strand_, "ControlServiceChannel_messageHandler");
    receivePromise->then(std::bind(&ControlServiceChannel::messageHandler, this->shared_from_this(), std::placeholders::_1, eventHandler),
                        std::bind(&IControlServiceChannelEventHandler::onChannelError, eventHandler, std::placeholders::_1));

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
    messenger_->startReceive();
}

void ControlServiceChannel::messageHandler(messenger::Message::Pointer message, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
    if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::ControlMessage::Enum_Name(messageId.getId()).c_str());

    switch(messageId.getId())
    {
    case proto::ids::ControlMessage::VERSION_RESPONSE:
        this->handleVersionResponse(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::SSL_HANDSHAKE:
        eventHandler->onHandshake(payload);
        break;
    case proto::ids::ControlMessage::SERVICE_DISCOVERY_REQUEST:
        this->handleServiceDiscoveryRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::AUDIO_FOCUS_REQUEST:
        this->handleAudioFocusRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::SHUTDOWN_REQUEST:
        this->handleShutdownRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::SHUTDOWN_RESPONSE:
        this->handleShutdownResponse(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::NAVIGATION_FOCUS_REQUEST:
        this->handleNavigationFocusRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::PING_REQUEST:
        this->handlePingRequest(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::PING_RESPONSE:
        this->handlePingResponse(payload, std::move(eventHandler));
        break;
    case proto::ids::ControlMessage::VOICE_SESSION_REQUEST:
        this->handleVoiceSessionRequest(payload, std::move(eventHandler));
        break;
    default:
        Log_e("message not handled %d", messageId.getId());
//        this->receive(std::move(eventHandler));
        break;
    }
}

void ControlServiceChannel::handleVersionResponse(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    const size_t elements = payload.size / sizeof(uint16_t);
    const uint16_t* versionResponse = reinterpret_cast<const uint16_t*>(payload.cdata);

    uint16_t majorCode = elements > 0 ? boost::endian::big_to_native(versionResponse[0]) : 0;
    uint16_t minorCode = elements > 1 ? boost::endian::big_to_native(versionResponse[1]) : 0;
    proto::enums::VersionResponseStatus::Enum status = elements > 2 ? static_cast<proto::enums::VersionResponseStatus::Enum>(versionResponse[2]) : proto::enums::VersionResponseStatus::MISMATCH;

    eventHandler->onVersionResponse(majorCode, minorCode, status);
}

void ControlServiceChannel::handleServiceDiscoveryRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::ServiceDiscoveryRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleVersionResponse: %s", request.Utf8DebugString().c_str());
        eventHandler->onServiceDiscoveryRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void ControlServiceChannel::handleAudioFocusRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::AudioFocusRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleAudioFocusRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onAudioFocusRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void ControlServiceChannel::handleVoiceSessionRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::VoiceSessionRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleVoiceSessionRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onVoiceSessionRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void ControlServiceChannel::handleShutdownRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::ShutdownRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleShutdownRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onShutdownRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void ControlServiceChannel::handleShutdownResponse(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::ShutdownResponse response;
    if(response.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleShutdownResponse: %s", response.Utf8DebugString().c_str());
        eventHandler->onShutdownResponse(response);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void ControlServiceChannel::handleNavigationFocusRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::NavigationFocusRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handleNavigationFocusRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onNavigationFocusRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}
void ControlServiceChannel::handlePingRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::PingRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handlePingRequest: %s", request.Utf8DebugString().c_str());
        eventHandler->onPingRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}
void ControlServiceChannel::handlePingResponse(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::PingResponse response;
    if(response.ParseFromArray(payload.cdata, payload.size))
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("handlePingResponse: %s", response.Utf8DebugString().c_str());
        eventHandler->onPingResponse(response);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

}
}
}
