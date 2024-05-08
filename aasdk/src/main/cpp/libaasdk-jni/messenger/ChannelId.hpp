#pragma once

#include <string>

namespace aasdk
{
namespace messenger
{

enum class ChannelId
{
    CONTROL,
    INPUT,
    SENSOR,
    VIDEO,
    MEDIA_AUDIO,
    SPEECH_AUDIO,
    SYSTEM_AUDIO,
    AV_INPUT,
    BLUETOOTH,
    NAVIGATION,
    MEDIA_STATUS,
    NONE = 255
};

std::string channelIdToString(ChannelId channelId);

}
}
