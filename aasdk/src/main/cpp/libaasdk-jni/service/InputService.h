#pragma once

#include "proto/ButtonCodeEnum.pb.h"
#include "channel/input/InputServiceChannel.hpp"
#include "IService.hpp"
#include "projection/IInputDevice.h"
#include "projection/IInputDeviceEventHandler.h"
#include "IServiceEventHandler.h"

namespace service
{

class InputService:
        public aasdk::channel::input::IInputServiceChannelEventHandler,
        public IService,
        public projection::IInputDeviceEventHandler,
        public std::enable_shared_from_this<InputService>
{
public:
    typedef std::shared_ptr<InputService> Pointer;

    InputService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IInputDevice::Pointer inputDevice, IServiceEventHandler::Pointer serviceEventHandler);
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
    using std::enable_shared_from_this<InputService>::shared_from_this;

    aasdk::io::strand strand_;
    aasdk::channel::input::InputServiceChannel::Pointer channel_;
    projection::IInputDevice::Pointer inputDevice_;
    bool serviceActive = false;
    IServiceEventHandler::Pointer serviceEventHandler_;
    bool isRunning_;
};

}
