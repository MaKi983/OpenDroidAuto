#pragma once

#include "channel/bluetooth/BluetoothServiceChannel.hpp"
#include "projection/IBluetoothDevice.h"
#include "IService.hpp"

namespace service
{

class BluetoothService: public aasdk::channel::bluetooth::IBluetoothServiceChannelEventHandler, public IService
{
public:
    typedef BluetoothService* Pointer;

    BluetoothService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IBluetoothDevice::Pointer bluetoothDevice);
    ~BluetoothService();
    void start() override;
    void stop() override;
    void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
    void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
    void onBluetoothPairingRequest(const aasdk::proto::messages::BluetoothPairingRequest& request) override;
    void onChannelError(const aasdk::error::Error& e) override;

private:
    boost::asio::io_service::strand strand_;
    aasdk::channel::bluetooth::BluetoothServiceChannel::Pointer channel_;
    projection::IBluetoothDevice::Pointer bluetoothDevice_;
};

}
