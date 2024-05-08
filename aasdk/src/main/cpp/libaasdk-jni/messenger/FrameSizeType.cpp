#include <messenger/FrameSizeType.hpp>

namespace aasdk
{
namespace messenger
{

std::string frameSizeTypeToString(FrameSizeType frameType)
{
    switch(frameType)
    {
    case FrameSizeType::SHORT:
        return "SHORT";
    case FrameSizeType::EXTENDED:
        return "EXTENDED";
    default:
        return "(unknown)";
    }
}

}
}
