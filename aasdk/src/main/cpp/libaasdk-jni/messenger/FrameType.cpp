#include <messenger/FrameType.hpp>

namespace aasdk
{
namespace messenger
{

std::string frameTypeToString(FrameType frameType)
{
    switch(frameType)
    {
    case FrameType::MIDDLE:
        return "MIDDLE";
    case FrameType::FIRST:
        return "FIRST";
    case FrameType::LAST:
        return "LAST";
    case FrameType::BULK:
        return "BULK";
    default:
        return "(unknown)";
    }
}

}
}
