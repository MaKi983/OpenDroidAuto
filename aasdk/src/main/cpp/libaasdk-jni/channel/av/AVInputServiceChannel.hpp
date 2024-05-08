#pragma once

#include <messenger/MessageId.hpp>
#include <messenger/Timestamp.hpp>
#include <channel/ServiceChannel.hpp>
#include <channel/av/IAVInputServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class AVInputServiceChannel: public IAVInputServiceChannel, public ServiceChannel
{
public:
    AVInputServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger);

    void receive(IAVInputServiceChannelEventHandler::Pointer eventHandler) override;
    void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;
    void sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise) override;
    void sendAVInputOpenResponse(const proto::messages::AVInputOpenResponse& response, SendPromise::Pointer promise) override;
    void sendAVMediaWithTimestampIndication(messenger::Timestamp::ValueType, const common::Data& data, SendPromise::Pointer promise) override;
    messenger::ChannelId getId() const override;

private:
    void messageHandler(messenger::Message::Pointer message, IAVInputServiceChannelEventHandler::Pointer eventHandler);
    void handleAVChannelSetupRequest(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler);
    void handleAVInputOpenRequest(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler);
    void handleAVMediaAckIndication(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, IAVInputServiceChannelEventHandler::Pointer eventHandler);
};

}
}
}
