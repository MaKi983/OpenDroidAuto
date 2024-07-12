#pragma once

#include <channel/ServiceChannel.hpp>
#include <channel/av/IMediaStatusServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class MediaStatusServiceChannel: public IMediaStatusServiceChannel, public ServiceChannel, public std::enable_shared_from_this<MediaStatusServiceChannel>
{
public:
    MediaStatusServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger);

    void receive(IMediaStatusServiceChannelEventHandler::Pointer eventHandler) override;
    messenger::ChannelId getId() const override;
    void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;

private:
    using std::enable_shared_from_this<MediaStatusServiceChannel>::shared_from_this;
    void messageHandler(messenger::Message::Pointer message, IMediaStatusServiceChannelEventHandler::Pointer eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, IMediaStatusServiceChannelEventHandler::Pointer eventHandler);
    void handleMetadataUpdate(const common::DataConstBuffer& payload, IMediaStatusServiceChannelEventHandler::Pointer eventHandler);
    void handlePlaybackUpdate(const common::DataConstBuffer& payload, IMediaStatusServiceChannelEventHandler::Pointer eventHandler);

};

}
}
}
