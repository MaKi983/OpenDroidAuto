#pragma once

#include <channel/av/AudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class SystemAudioServiceChannel: public AudioServiceChannel
{
public:
    SystemAudioServiceChannel(io::strand& strand, messenger::IMessenger::Pointer messenger);
};

}
}
}
