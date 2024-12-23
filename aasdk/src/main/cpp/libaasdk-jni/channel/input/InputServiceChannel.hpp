#pragma once

#include <channel/ServiceChannel.hpp>
#include <channel/input/IInputServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace input
{

class InputServiceChannel: public IInputServiceChannel, public ServiceChannel, public std::enable_shared_from_this<InputServiceChannel>
{
 public:
    InputServiceChannel(boost::optional<boost::asio::io_service::strand>& strand, messenger::IMessenger::Pointer messenger);
    ~InputServiceChannel();

    void receive(IInputServiceChannelEventHandler::Pointer eventHandler) override;
    void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;
    void sendInputEventIndication(const proto::messages::InputEventIndication& indication, SendPromise::Pointer promise) override;
    void sendBindingResponse(const proto::messages::BindingResponse& response, SendPromise::Pointer promise) override;
    messenger::ChannelId getId() const override;

private:
    using std::enable_shared_from_this<InputServiceChannel>::shared_from_this;
    void messageHandler(messenger::Message::Pointer message, IInputServiceChannelEventHandler::Pointer eventHandler);
    void handleBindingRequest(const common::DataConstBuffer& payload, IInputServiceChannelEventHandler::Pointer eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, IInputServiceChannelEventHandler::Pointer eventHandler);
};

}
}
}
