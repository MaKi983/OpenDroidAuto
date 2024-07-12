#pragma once

#include <channel/ServiceChannel.hpp>
#include <channel/navigation/INavigationStatusServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace navigation
{

class NavigationStatusServiceChannel: public INavigationStatusServiceChannel, public ServiceChannel, public std::enable_shared_from_this<NavigationStatusServiceChannel>
{
public:
    NavigationStatusServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger);

    void receive(INavigationStatusServiceChannelEventHandler::Pointer eventHandler) override;
    messenger::ChannelId getId() const override;
    void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;

private:
    using std::enable_shared_from_this<NavigationStatusServiceChannel>::shared_from_this;
    void messageHandler(messenger::Message::Pointer message, INavigationStatusServiceChannelEventHandler::Pointer eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler);
    void handleStatusUpdate(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler);
    void handleTurnEvent(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler);
    void handleDistanceEvent(const common::DataConstBuffer& payload, INavigationStatusServiceChannelEventHandler::Pointer eventHandler);
};

}
}
}
