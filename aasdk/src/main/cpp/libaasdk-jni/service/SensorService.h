#pragma once

#include "channel/sensor/SensorServiceChannel.hpp"
#include "IService.hpp"

namespace service
{

class SensorService: public aasdk::channel::sensor::ISensorServiceChannelEventHandler, public IService
{
public:
    typedef SensorService* Pointer;

    SensorService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, bool nightMode=false);
    ~SensorService();

    void start() override;
    void stop() override;
    void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
    void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
    void onSensorStartRequest(const aasdk::proto::messages::SensorStartRequestMessage& request) override;
    void onChannelError(const aasdk::error::Error& e) override;
    void setNightMode(bool nightMode);

private:
    void sendDrivingStatusUnrestricted();
    void sendNightData();

    boost::asio::io_service::strand strand_;
    aasdk::channel::sensor::SensorServiceChannel::Pointer channel_;
    bool nightMode_;
};

}
