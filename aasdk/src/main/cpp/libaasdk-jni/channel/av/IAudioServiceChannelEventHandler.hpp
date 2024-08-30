#pragma once

#include <stdint.h>
#include <proto/AVChannelSetupRequestMessage.pb.h>
#include <proto/AVChannelStartIndicationMessage.pb.h>
#include <proto/AVChannelStopIndicationMessage.pb.h>
#include <proto/ChannelOpenRequestMessage.pb.h>
#include <messenger/Timestamp.hpp>
#include <common/Data.hpp>
#include <error/Error.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class IAudioServiceChannelEventHandler
{
public:
    typedef std::shared_ptr<IAudioServiceChannelEventHandler> Pointer;

    IAudioServiceChannelEventHandler() = default;
    virtual ~IAudioServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onAVChannelSetupRequest(const proto::messages::AVChannelSetupRequest& request) = 0;
    virtual void onAVChannelStartIndication(const proto::messages::AVChannelStartIndication& indication) = 0;
    virtual void onAVChannelStopIndication(const proto::messages::AVChannelStopIndication& indication) = 0;
    virtual void onAVMediaWithTimestampIndication(messenger::Timestamp::ValueType, const common::DataConstBuffer& buffer) = 0;
    virtual void onAVMediaIndication(const common::DataConstBuffer& buffer) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
};

}
}
}
