#pragma once

#include <boost/asio.hpp>
#include <messenger/IMessenger.hpp>
#include <channel/ServiceChannel.hpp>
#include <channel/control/IControlServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace control
{

class ControlServiceChannel: public IControlServiceChannel, public ServiceChannel, public std::enable_shared_from_this<ControlServiceChannel>
{
public:
    ControlServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger);

    void receive(IControlServiceChannelEventHandler::Pointer eventHandler) override;

    void sendVersionRequest(SendPromise::Pointer promise) override;
    void sendHandshake(common::Data handshakeBuffer, SendPromise::Pointer promise) override;
    void sendAuthComplete(const proto::messages::AuthCompleteIndication& response, SendPromise::Pointer promise) override;
    void sendServiceDiscoveryResponse(const proto::messages::ServiceDiscoveryResponse& response, SendPromise::Pointer promise) override;
    void sendAudioFocusResponse(const proto::messages::AudioFocusResponse& response, SendPromise::Pointer promise) override;
    void sendShutdownRequest(const proto::messages::ShutdownRequest& request, SendPromise::Pointer promise) override;
    void sendShutdownResponse(const proto::messages::ShutdownResponse& response, SendPromise::Pointer promise) override;
    void sendNavigationFocusResponse(const proto::messages::NavigationFocusResponse& respons, SendPromise::Pointer promisee) override;
    void sendPingRequest(const proto::messages::PingRequest& request, SendPromise::Pointer promise) override;
    void sendPingResponse(const proto::messages::PingResponse& response, SendPromise::Pointer promise) override;

private:
    using std::enable_shared_from_this<ControlServiceChannel>::shared_from_this;
    void messageHandler(messenger::Message::Pointer message, IControlServiceChannelEventHandler::Pointer eventHandler);

    void handleVersionResponse(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handleServiceDiscoveryRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handleAudioFocusRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handleShutdownRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handleShutdownResponse(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handleNavigationFocusRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handlePingRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handlePingResponse(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
    void handleVoiceSessionRequest(const common::DataConstBuffer& payload, IControlServiceChannelEventHandler::Pointer eventHandler);
};

}
}
}
