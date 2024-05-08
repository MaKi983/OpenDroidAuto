#pragma once

#include "proto/ButtonCodeEnum.pb.h"
#include "channel/input/InputServiceChannel.hpp"
#include "IService.hpp"
#include "projection/IInputDevice.h"
#include "projection/IInputDeviceEventHandler.h"

namespace service
{

class InputService:
        public aasdk::channel::input::IInputServiceChannelEventHandler,
        public IService,
        public projection::IInputDeviceEventHandler
{
public:
    typedef InputService* Pointer;

    InputService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IInputDevice::Pointer inputDevice);
    ~InputService();

    void sendButtonPress(aasdk::proto::enums::ButtonCode::Enum buttonCode, projection::WheelDirection wheelDirection = projection::WheelDirection::NONE, projection::ButtonEventType buttonEventType = projection::ButtonEventType::NONE);
    void start() override;
    void stop() override;
    void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
    void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
    void onBindingRequest(const aasdk::proto::messages::BindingRequest& request) override;
    void onChannelError(const aasdk::error::Error& e) override;
    void onButtonEvent(const projection::ButtonEvent& event) override;
    void onTouchEvent(aasdk::proto::messages::InputEventIndication inputEventIndication) override;
    void onMouseEvent(const projection::TouchEvent& event) override;

private:
    boost::asio::io_service::strand strand_;
    aasdk::channel::input::InputServiceChannel::Pointer channel_;
    projection::IInputDevice::Pointer inputDevice_;
    bool serviceActive = false;
};

}
