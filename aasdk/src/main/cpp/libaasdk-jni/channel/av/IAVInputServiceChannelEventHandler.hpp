#pragma once

#include <proto/AVChannelSetupRequestMessage.pb.h>
#include <proto/AVMediaAckIndicationMessage.pb.h>
#include <proto/AVInputOpenRequestMessage.pb.h>
#include <proto/ChannelOpenRequestMessage.pb.h>
#include <error/Error.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class IAVInputServiceChannelEventHandler
{
public:
    typedef IAVInputServiceChannelEventHandler* Pointer;

    IAVInputServiceChannelEventHandler() = default;
    virtual ~IAVInputServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onAVChannelSetupRequest(const proto::messages::AVChannelSetupRequest& request) = 0;
    virtual void onAVInputOpenRequest(const proto::messages::AVInputOpenRequest& request) = 0;
    virtual void onAVMediaAckIndication(const proto::messages::AVMediaAckIndication& indication) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
};

}
}
}
