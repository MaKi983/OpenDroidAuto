#pragma once

#include "io/Promise.hpp"
#include "InputEvent.h"

namespace projection
{

struct Rect {
    int width;
    int height;
};

class IInputDeviceEventHandler;

class IInputDevice
{
public:
    typedef IInputDevice* Pointer;
    typedef std::vector<aasdk::proto::enums::ButtonCode::Enum> ButtonCodes;

    virtual ~IInputDevice() = default;
    virtual void start(IInputDeviceEventHandler& eventHandler) = 0;
    virtual void stop() = 0;
    virtual ButtonCodes getSupportedButtonCodes() = 0;
    virtual bool hasTouchscreen() = 0;
    virtual Rect getTouchscreenGeometry() = 0;
};

}