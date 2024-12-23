#pragma once

#include "channel/sensor/SensorServiceChannel.hpp"
#include "IService.hpp"
#include "IServiceEventHandler.h"

namespace service
{

class SensorService: public aasdk::channel::sensor::ISensorServiceChannelEventHandler, public IService, public std::enable_shared_from_this<SensorService>
{
public:
    typedef std::shared_ptr<SensorService> Pointer;

    SensorService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, IServiceEventHandler::Pointer serviceEventHandler, bool nightMode=false);
    ~SensorService();

    void start() override;
    void stop() override;
    void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
    void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
    void onSensorStartRequest(const aasdk::proto::messages::SensorStartRequestMessage& request) override;
    void onChannelError(const aasdk::error::Error& e) override;
    void setNightMode(bool nightMode);

private:
    using std::enable_shared_from_this<SensorService>::shared_from_this;
    void sendDrivingStatusUnrestricted();
    void sendNightData();

    aasdk::io::strand strand_;
    aasdk::channel::sensor::SensorServiceChannel::Pointer channel_;
    bool nightMode_;
    IServiceEventHandler::Pointer serviceEventHandler_;
    bool isRunning_;
};

}
