#pragma once

#include <proto/BluetoothPairingRequestMessage.pb.h>
#include <proto/ChannelOpenRequestMessage.pb.h>
#include <error/Error.hpp>


namespace aasdk
{
namespace channel
{
namespace bluetooth
{

class IBluetoothServiceChannelEventHandler
{
public:
    typedef IBluetoothServiceChannelEventHandler* Pointer;

    IBluetoothServiceChannelEventHandler() = default;
    virtual ~IBluetoothServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onBluetoothPairingRequest(const proto::messages::BluetoothPairingRequest& request) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
};

}
}
}
