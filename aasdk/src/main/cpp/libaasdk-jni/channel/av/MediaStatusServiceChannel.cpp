#include <proto/ControlMessageIdsEnum.pb.h>
#include <proto/MediaInfoChannelMessageIdsEnum.pb.h>
#include <proto/MediaInfoChannelMetadataData.pb.h>
#include <proto/MediaInfoChannelPlaybackData.pb.h>
#include <proto/BluetoothChannelMessageIdsEnum.pb.h>
#include <proto/BluetoothPairingRequestMessage.pb.h>
#include <channel/av/IMediaStatusServiceChannelEventHandler.hpp>
#include <channel/av/MediaStatusServiceChannel.hpp>
#include <Log.h>


namespace aasdk
{
namespace channel
{
namespace av
{

MediaStatusServiceChannel::MediaStatusServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::MEDIA_STATUS)
{

}

void MediaStatusServiceChannel::receive(IMediaStatusServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_, "MediaStatusServiceChannel_messageHandler");
    receivePromise->then(std::bind(&MediaStatusServiceChannel::messageHandler, this->shared_from_this(), std::placeholders::_1, eventHandler),
                        std::bind(&IMediaStatusServiceChannelEventHandler::onChannelError, eventHandler,std::placeholders::_1));

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

messenger::ChannelId MediaStatusServiceChannel::getId() const
{
    return channelId_;
}

void MediaStatusServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    if (Log::isVerbose() && Log::logProtocol()) Log_v("sendChannelOpenResponse: %s", response.Utf8DebugString().c_str());
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}


void MediaStatusServiceChannel::messageHandler(messenger::Message::Pointer message, IMediaStatusServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());
    if (Log::isDebug()) Log_d("messageHandler: received message, %d (%s)", messageId.getId(), proto::ids::ControlMessage::Enum_Name(messageId.getId()).c_str());

    switch(messageId.getId())
    {
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, std::move(eventHandler));
        break;

    case proto::ids::MediaInfoChannelMessage::METADATA:
        this->handleMetadataUpdate(payload, std::move(eventHandler));
        break;

    case proto::ids::MediaInfoChannelMessage::PLAYBACK:
        this->handlePlaybackUpdate(payload, std::move(eventHandler));
        break;

    default:
        Log_e("message not handled %d - %s", messageId.getId(), dump(payload).c_str());
//        this->receive(std::move(eventHandler));
        break;
    }
}

void MediaStatusServiceChannel::handleMetadataUpdate(const common::DataConstBuffer& payload, IMediaStatusServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::MediaInfoChannelMetadataData metadata;
    if(metadata.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onMetadataUpdate(metadata);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
        Log_e("encountered error with message: %s", dump(payload).c_str());
    }

}


void MediaStatusServiceChannel::handlePlaybackUpdate(const common::DataConstBuffer& payload, IMediaStatusServiceChannelEventHandler::Pointer eventHandler)
{
    proto::messages::MediaInfoChannelPlaybackData playback;
    if(playback.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onPlaybackUpdate(playback);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
        Log_e("encountered error with message: %s", dump(payload).c_str());
    }

}

void MediaStatusServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, IMediaStatusServiceChannelEventHandler::Pointer eventHandler)
{
    if (Log::isVerbose()) Log_v("channel open request");

    proto::messages::ChannelOpenRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
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
