#include <channel/av/SpeechAudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

SpeechAudioServiceChannel::SpeechAudioServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : AudioServiceChannel(strand, std::move(messenger), messenger::ChannelId::SPEECH_AUDIO)
{

}

}
}
}
