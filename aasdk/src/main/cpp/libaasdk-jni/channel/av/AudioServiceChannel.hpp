#pragma once

#include <messenger/MessageId.hpp>
#include <channel/ServiceChannel.hpp>
#include <channel/av/IAudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class AudioServiceChannel: public IAudioServiceChannel, public ServiceChannel, public std::enable_shared_from_this<AudioServiceChannel>
{
public:
    AudioServiceChannel(io::strand& strand, messenger::IMessenger::Pointer messenger,  messenger::ChannelId channelId);

    void receive(IAudioServiceChannelEventHandler::Pointer eventHandler) override;
    void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;
    void sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise) override;
    void sendAVMediaAckIndication(const proto::messages::AVMediaAckIndication& indication, SendPromise::Pointer promise) override;
    messenger::ChannelId getId() const override;

private:
    using std::enable_shared_from_this<AudioServiceChannel>::shared_from_this;
    void messageHandler(messenger::Message::Pointer message, IAudioServiceChannelEventHandler::Pointer eventHandler);
    void handleAVChannelSetupRequest(const common::DataConstBuffer& payload, IAudioServiceChannelEventHandler::Pointer eventHandler);
    void handleStartIndication(const common::DataConstBuffer& payload, IAudioServiceChannelEventHandler::Pointer eventHandler);
    void handleStopIndication(const common::DataConstBuffer& payload, IAudioServiceChannelEventHandler::Pointer eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, IAudioServiceChannelEventHandler::Pointer eventHandler);
    void handleAVMediaWithTimestampIndication(const common::DataConstBuffer& payload, IAudioServiceChannelEventHandler::Pointer eventHandler);
};

}
}
}
