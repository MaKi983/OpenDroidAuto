#pragma once

#include "proto/ButtonCodeEnum.pb.h"
#include "proto/TouchActionEnum.pb.h"
#include "io/Promise.hpp"

namespace projection
{

enum class ButtonEventType
{
    NONE,
    PRESS,
    RELEASE
};

enum class WheelDirection
{
    NONE,
    LEFT,
    RIGHT
};

struct ButtonEvent
{
    ButtonEventType type;
    WheelDirection wheelDirection;
    aasdk::proto::enums::ButtonCode::Enum code;
};

struct TouchEvent
{
    aasdk::proto::enums::TouchAction::Enum type;
    uint32_t x;
    uint32_t y;
    uint32_t pointerId;
};

}