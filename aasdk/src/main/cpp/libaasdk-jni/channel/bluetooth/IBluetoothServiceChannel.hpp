
#pragma once

#include <memory>
#include <proto/ChannelOpenResponseMessage.pb.h>
#include <proto/BluetoothPairingResponseMessage.pb.h>
#include <messenger/ChannelId.hpp>
#include <channel/Promise.hpp>
#include <channel/bluetooth/IBluetoothServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace bluetooth
{

class IBluetoothServiceChannel
{
public:
    typedef std::shared_ptr<IBluetoothServiceChannel> Pointer;

    IBluetoothServiceChannel() = default;
    virtual ~IBluetoothServiceChannel() = default;

    virtual void receive(IBluetoothServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendBluetoothPairingResponse(const proto::messages::BluetoothPairingResponse& response, SendPromise::Pointer promise) = 0;
    virtual messenger::ChannelId getId() const = 0;
};

}
}
}
