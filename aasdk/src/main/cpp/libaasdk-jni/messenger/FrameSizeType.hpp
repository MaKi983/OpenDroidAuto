#pragma once

#include <string>

namespace aasdk
{
namespace messenger
{

enum class FrameSizeType
{
    SHORT,
    EXTENDED
};

std::string frameSizeTypeToString(FrameSizeType messageType);

}
}
