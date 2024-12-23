#pragma once

#include <channel/ServiceChannel.hpp>
#include <channel/av/IVideoServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class VideoServiceChannel: public IVideoServiceChannel, public ServiceChannel, public std::enable_shared_from_this<VideoServiceChannel>
{
public:
    VideoServiceChannel(io::strand& strand, messenger::IMessenger::Pointer messenger);

    void receive(IVideoServiceChannelEventHandler::Pointer eventHandler) override;
    void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;
    void sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise) override;
    void sendVideoFocusIndication(const proto::messages::VideoFocusIndication& indication, SendPromise::Pointer promise) override;
    void sendAVMediaAckIndication(const proto::messages::AVMediaAckIndication& indication, SendPromise::Pointer promise) override;
    messenger::ChannelId getId() const override;

private:
    using std::enable_shared_from_this<VideoServiceChannel>::shared_from_this;
    void messageHandler(messenger::Message::Pointer message, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleAVChannelSetupRequest(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleStartIndication(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleStopIndication(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleVideoFocusRequest(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleAVMediaWithTimestampIndication(const common::DataConstBuffer& payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleAVMediaWithTimestampIndication(const common::Data payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
    void handleAVMediaIndication(const common::Data payload, IVideoServiceChannelEventHandler::Pointer eventHandler);
};

}
}
}
