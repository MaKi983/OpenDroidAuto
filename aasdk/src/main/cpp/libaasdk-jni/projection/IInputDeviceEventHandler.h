#pragma once

#include "InputEvent.h"
#include "proto/InputEventIndicationMessage.pb.h"


namespace projection
{

class IInputDeviceEventHandler
{
public:
    virtual ~IInputDeviceEventHandler() = default;

    virtual void onButtonEvent(const ButtonEvent& event) = 0;
    virtual void onTouchEvent(aasdk::proto::messages::InputEventIndication inputEventIndication) = 0;
    virtual void onMouseEvent(const projection::TouchEvent& event) = 0;

};

}