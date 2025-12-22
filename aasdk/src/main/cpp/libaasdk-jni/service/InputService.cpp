#include <Log.h>
#include "proto/InputEventIndicationMessage.pb.h"
#include "service/InputService.h"

namespace service
{

InputService::InputService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IInputDevice::Pointer inputDevice, IServiceEventHandler::Pointer serviceEventHandler)
        : strand_(ioService)
        , channel_(std::make_shared<aasdk::channel::input::InputServiceChannel>(strand_, std::move(messenger)))
        , serviceEventHandler_(serviceEventHandler)
        , inputDevice_(inputDevice)
        , isRunning_(false)
{
}

InputService::~InputService(){
    if (Log::isVerbose()) Log_v("destructor");
    channel_.reset();
    serviceEventHandler_ = nullptr;
    inputDevice_ = nullptr;
}

void InputService::start()
{
    isRunning_ = true;
//    strand_->dispatch([this, self = this->shared_from_this()]() {
//    strand_->post([this, self = this->shared_from_this()]() {
        if(Log::isInfo()) Log_i("start");
        channel_->receive(this->shared_from_this());
//    });
//    serviceActive = true;
}

void InputService::stop()
{
    if(Log::isInfo()) Log_i("stop");
    isRunning_ = false;
    inputDevice_->stop();

//    serviceActive = false;
}

void InputService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
{
    if(Log::isInfo()) Log_i("fill features");

    auto* channelDescriptor = response.add_channels();
    channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));

    auto* inputChannel = channelDescriptor->mutable_input_channel();

    const auto& supportedButtonCodes = inputDevice_->getSupportedButtonCodes();

    for(const auto& buttonCode : supportedButtonCodes)
    {
        inputChannel->add_supported_keycodes(buttonCode);
    }

    if(inputDevice_->hasTouchscreen())
    {
        const auto& touchscreenSurface = inputDevice_->getTouchscreenGeometry();
        auto touchscreenConfig = inputChannel->mutable_touch_screen_config();

        touchscreenConfig->set_width(touchscreenSurface.width);
        touchscreenConfig->set_height(touchscreenSurface.height);
    }

    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s", channelDescriptor->Utf8DebugString().c_str());
}

void InputService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onChannelOpenRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("open request, priority: %d", request.priority());
    const aasdk::proto::enums::Status::Enum status = aasdk::proto::enums::Status::OK;
    if(Log::isDebug()) Log_d("open status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

    aasdk::proto::messages::ChannelOpenResponse response;
    response.set_status(status);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "InputService_channelOpen");
    promise->then([]() {}, std::bind(&InputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));

//    channel_->receive(this->shared_from_this());
}

void InputService::onBindingRequest(const aasdk::proto::messages::BindingRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onBindingRequest: %s", request.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("binding request, scan codes count: %d", request.scan_codes_size());

    aasdk::proto::enums::Status::Enum status = aasdk::proto::enums::Status::OK;
    const auto& supportedButtonCodes = inputDevice_->getSupportedButtonCodes();

    for(int i = 0; i < request.scan_codes_size(); ++i)
    {
        if(std::find(supportedButtonCodes.begin(), supportedButtonCodes.end(), request.scan_codes(i)) == supportedButtonCodes.end())
        {
            if(Log::isDebug()) Log_d("binding request, scan code: %d is not supported", request.scan_codes(i));

            status = aasdk::proto::enums::Status::FAIL;
            break;
        }
    }

    aasdk::proto::messages::BindingResponse response;
    response.set_status(status);

    if(status == aasdk::proto::enums::Status::OK)
    {
        inputDevice_->start(*this);
    }

    if(Log::isDebug()) Log_d("binding request, status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

    auto promise = aasdk::channel::SendPromise::defer(strand_, "InputService_binding");
    promise->then([]() {}, std::bind(&InputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendBindingResponse(response, std::move(promise));
//    channel_->receive(this->shared_from_this());
}

void InputService::onChannelError(const aasdk::error::Error& e)
{
    if (!isRunning_){
        if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
        return;
    }
//    Log_e("channel error: %s", e.what());
    serviceEventHandler_->onError(e);
}

void InputService::onButtonEvent(const projection::ButtonEvent& event)
{
    if(Log::isDebug()) Log_d("onButtonEvent");
    if(!isRunning_) return;

    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());

    strand_->dispatch([this, self = this->shared_from_this(), event = std::move(event), timestamp = std::move(timestamp)]() {
        aasdk::proto::messages::InputEventIndication inputEventIndication;
        inputEventIndication.set_timestamp(timestamp.count());

        if(event.code == aasdk::proto::enums::ButtonCode::SCROLL_WHEEL)
        {
            auto relativeEvent = inputEventIndication.mutable_relative_input_event()->add_relative_input_events();
            relativeEvent->set_delta(event.wheelDirection == projection::WheelDirection::LEFT ? -1 : 1);
            relativeEvent->set_scan_code(event.code);
        }
        else
        {
            auto buttonEvent = inputEventIndication.mutable_button_event()->add_button_events();
            buttonEvent->set_meta(0);
            buttonEvent->set_is_pressed(event.type == projection::ButtonEventType::PRESS);
            buttonEvent->set_long_press(false);
            buttonEvent->set_scan_code(event.code);
        }

        auto promise = aasdk::channel::SendPromise::defer(strand_, "InputService_buttonEvent");
        promise->then([]() {}, std::bind(&InputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
        channel_->sendInputEventIndication(inputEventIndication, std::move(promise));
    });
}

void InputService::sendButtonPress(aasdk::proto::enums::ButtonCode::Enum buttonCode, projection::WheelDirection wheelDirection, projection::ButtonEventType buttonEventType)
{
    if(Log::isDebug()) Log_d("injecting button press");
    if(buttonCode == aasdk::proto::enums::ButtonCode::SCROLL_WHEEL)
    {
        onButtonEvent({projection::ButtonEventType::NONE, wheelDirection, buttonCode});

    }
    else
    {
        if(buttonEventType == projection::ButtonEventType::NONE){
            onButtonEvent({projection::ButtonEventType::PRESS, projection::WheelDirection::NONE, buttonCode});
            onButtonEvent({projection::ButtonEventType::RELEASE, projection::WheelDirection::NONE, buttonCode});
        }
        else
        {
            onButtonEvent({buttonEventType, projection::WheelDirection::NONE, buttonCode});
        }
    }
}

void InputService::onTouchEvent(aasdk::proto::messages::InputEventIndication inputEventIndication)
{
    if(!isRunning_) return;

    strand_->dispatch([this, self = this->shared_from_this(), inputEventIndication = std::move(inputEventIndication)]() {

        auto promise = aasdk::channel::SendPromise::defer(strand_, "InputService_touchEvent");
        promise->then([]() {}, std::bind(&InputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
        channel_->sendInputEventIndication(inputEventIndication, std::move(promise));
    });
}

void InputService::onMouseEvent(const projection::TouchEvent& event)
{
    if (!isRunning_) return;

    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());

    strand_->dispatch([this, self = this->shared_from_this(), event = std::move(event), timestamp = std::move(timestamp)]() {
        aasdk::proto::messages::InputEventIndication inputEventIndication;
        inputEventIndication.set_timestamp(timestamp.count());

        auto touchEvent = inputEventIndication.mutable_touch_event();
        touchEvent->set_touch_action(event.type);
        auto touchLocation = touchEvent->add_touch_location();
        touchLocation->set_x(event.x);
        touchLocation->set_y(event.y);
        touchLocation->set_pointer_id(0);

        auto promise = aasdk::channel::SendPromise::defer(strand_, "InputService_mouseEvent");
        promise->then([]() {}, std::bind(&InputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
        channel_->sendInputEventIndication(inputEventIndication, std::move(promise));
    });
}

}
