#include <messenger/MessageType.hpp>

namespace aasdk
{
namespace messenger
{

std::string messageTypeToString(MessageType messageType)
{
    switch(messageType)
    {
    case MessageType::SPECIFIC:
        return "SPECIFIC";
    case MessageType::CONTROL:
        return "CONTROL";
    default:
        return "(unknown)";
    }
}

}
}
