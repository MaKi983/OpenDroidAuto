#pragma once

#include <channel/ServiceChannel.hpp>
#include <channel/bluetooth/IBluetoothServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace bluetooth
{

class BluetoothServiceChannel: public IBluetoothServiceChannel, public ServiceChannel, public std::enable_shared_from_this<BluetoothServiceChannel>
{
public:
    BluetoothServiceChannel(io::strand& strand, messenger::IMessenger::Pointer messenger);
    ~BluetoothServiceChannel();
    void receive(IBluetoothServiceChannelEventHandler::Pointer eventHandler) override;
    messenger::ChannelId getId() const override;
    void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;
    void sendBluetoothPairingResponse(const proto::messages::BluetoothPairingResponse& response, SendPromise::Pointer promise) override;

private:
    using std::enable_shared_from_this<BluetoothServiceChannel>::shared_from_this;
    void messageHandler(messenger::Message::Pointer message, IBluetoothServiceChannelEventHandler::Pointer eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, IBluetoothServiceChannelEventHandler::Pointer eventHandler);
    void handleBluetoothPairingRequest(const common::DataConstBuffer& payload, IBluetoothServiceChannelEventHandler::Pointer eventHandler);
};

}
}
}
