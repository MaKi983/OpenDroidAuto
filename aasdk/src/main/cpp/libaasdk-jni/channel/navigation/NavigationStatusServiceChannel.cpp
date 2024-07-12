#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/NavigationChannelMessageIdsEnum.pb.h>
#include <proto/NavigationStatusMessage.pb.h>
#include <proto/NavigationTurnEventMessage.pb.h>
#include <proto/NavigationDistanceEventMessage.pb.h>
#include <channel/navigation/INavigationStatusServiceChannelEventHandler.hpp>
#include <channel/navigation/NavigationStatusServiceChannel.hpp>
#include <Log.h>

namespace aasdk
{
namespace channel
{
namespace navigation
{

    NavigationStatusServiceChannel::NavigationStatusServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
            : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::NAVIGATION)
    {

    }

    void NavigationStatusServiceChannel::receive(INavigationStatusServiceChannelEventHandler::Pointer eventHandler)
    {
        auto receivePromise = messenger::ReceivePromise::defer(strand_);
        receivePromise->then(std::bind(&NavigationStatusServiceChannel::messageHandler, this->shared_from_this(), std::placeholders::_1, eventHandler),
                             std::bind(&INavigationStatusServiceChannelEventHandler::onChannelError, eventHandler,std::placeholders::_1));

        messenger_->enqueueReceive(channelId_, std::move(receivePromise));
    }

    messenger::ChannelId NavigationStatusServiceChannel::getId() const
    {
        return channelId_;
    }

    void NavigationStatusServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("sendChannelOpenResponse: %s", response.Utf8DebugString().c_str());

        auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
        message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
        message->insertPayload(response);

        this->send(std::move(message), std::move(promise));
    }


    void NavigationStatusServiceChannel::messageHandler(messenger::Message::Pointer message, INavigationStatusServiceChannelEventHandler::Pointer eventHandler)
    {
        messenger::MessageId messageId(message->getPayload());
        common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
        if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::NavigationChannelMessage::Enum_Name(messageId.getId()).c_str());

        switch(messageId.getId())
        {
            case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
                this->handleChannelOpenRequest(payload, std::move(eventHandler));
                break;
            case proto::ids::NavigationChannelMessage::STATUS:
                this->handleStatusUpdate(payload, std::move(eventHandler));
                break;
            case proto::ids::NavigationChannelMessage::TURN_EVENT:
                this->handleTurnEvent(payload, std::move(eventHandler));
                break;
            case proto::ids::NavigationChannelMessage::DISTANCE_EVENT:
                this->handleDistanceEvent(payload, std::move(eventHandler));
                break;
            default:
                Log_e("message not handled: %d - %s", messageId.getId(), dump(payload).c_str());
                this->receive(std::move(eventHandler));
                break;
        }
    }

    void NavigationStatusServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler)
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

    void NavigationStatusServiceChannel::handleStatusUpdate(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler)
    {
        proto::messages::NavigationStatus navStatus;
        if(navStatus.ParseFromArray(payload.cdata, payload.size))
        {
            if (Log::isVerbose() && Log::logProtocol()) Log_v("handleStatusUpdate: %s", navStatus.Utf8DebugString().c_str());
            eventHandler->onStatusUpdate(navStatus);
        }
        else
        {
            eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
        }

    }

    void NavigationStatusServiceChannel::handleTurnEvent(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler)
    {
        proto::messages::NavigationTurnEvent turnEvent;
        if(turnEvent.ParseFromArray(payload.cdata, payload.size))
        {
            if (Log::isVerbose() && Log::logProtocol()) Log_v("handleTurnEvent: %s", turnEvent.Utf8DebugString().c_str());
            eventHandler->onTurnEvent(turnEvent);
        }
        else
        {
            eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
        }

    }

    void NavigationStatusServiceChannel::handleDistanceEvent(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler)
    {
        proto::messages::NavigationDistanceEvent distanceEvent;
        if(distanceEvent.ParseFromArray(payload.cdata, payload.size))
        {
            if (Log::isVerbose() && Log::logProtocol()) Log_v("handleDistanceEvent: %s", distanceEvent.Utf8DebugString().c_str());
            eventHandler->onDistanceEvent(distanceEvent);
        }
        else
        {
            eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
        }

    }

}
}
}
