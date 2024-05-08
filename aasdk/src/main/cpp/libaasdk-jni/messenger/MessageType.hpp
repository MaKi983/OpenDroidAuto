#pragma once

#include <stdint.h>
#include <string>

namespace aasdk
{
namespace messenger
{

enum class MessageType
{
    SPECIFIC = 0,
    CONTROL = 1 << 2
};

std::string messageTypeToString(MessageType messageType);

}
}
