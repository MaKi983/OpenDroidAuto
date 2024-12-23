#pragma once

#include <channel/av/AudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class SpeechAudioServiceChannel: public AudioServiceChannel
{
public:
    SpeechAudioServiceChannel(io::strand& strand, messenger::IMessenger::Pointer messenger);
};

}
}
}
