#pragma once

#include <channel/av/AudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class MediaAudioServiceChannel: public AudioServiceChannel
{
public:
    MediaAudioServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger);
};

}
}
}
