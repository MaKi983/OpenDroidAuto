#pragma once

#include <string>

namespace aasdk
{
namespace messenger
{

enum class FrameType
{
    MIDDLE = 0,
    FIRST = 1 << 0,
    LAST = 1 << 1,
    BULK = FIRST | LAST
};

std::string frameTypeToString(FrameType frameType);

}
}
