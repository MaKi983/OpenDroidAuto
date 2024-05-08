#pragma once

#include <boost/asio.hpp>
#include <messenger/IMessenger.hpp>
#include <channel/Promise.hpp>


namespace aasdk
{
namespace channel
{

class ServiceChannel
{
protected:
    ServiceChannel(boost::asio::io_service::strand& strand,
                   messenger::IMessenger::Pointer messenger,
                   messenger::ChannelId channelId);

    virtual ~ServiceChannel() = default;
    void send(messenger::Message::Pointer message, SendPromise::Pointer promise);

    boost::asio::io_service::strand& strand_;
    messenger::IMessenger::Pointer messenger_;
    messenger::ChannelId channelId_;
};

}
}
