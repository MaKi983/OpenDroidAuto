#pragma once

#include <proto/AVChannelSetupRequestMessage.pb.h>
#include <proto/AVChannelStartIndicationMessage.pb.h>
#include <proto/ChannelOpenRequestMessage.pb.h>
#include <proto/VideoFocusRequestMessage.pb.h>
#include <proto/AVChannelStopIndicationMessage.pb.h>
#include <messenger/Timestamp.hpp>
#include <common/Data.hpp>
#include <error/Error.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class IVideoServiceChannelEventHandler
{
public:
    typedef IVideoServiceChannelEventHandler* Pointer;

    IVideoServiceChannelEventHandler() = default;
    virtual ~IVideoServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onAVChannelSetupRequest(const proto::messages::AVChannelSetupRequest& request) = 0;
    virtual void onAVChannelStartIndication(const proto::messages::AVChannelStartIndication& indication) = 0;
    virtual void onAVChannelStopIndication(const proto::messages::AVChannelStopIndication& indication) = 0;
    virtual void onAVMediaWithTimestampIndication(messenger::Timestamp::ValueType, const common::DataConstBuffer& buffer) = 0;
    virtual void onAVMediaIndication(const common::DataConstBuffer& buffer) = 0;
    virtual void onVideoFocusRequest(const proto::messages::VideoFocusRequest& request) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
};

}
}
}
