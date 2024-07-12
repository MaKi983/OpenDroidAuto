#pragma once

#include <proto/ChannelOpenRequestMessage.pb.h>
#include <proto/NavigationDistanceEventMessage.pb.h>
#include <proto/NavigationTurnEventMessage.pb.h>
#include <proto/NavigationStatusMessage.pb.h>
#include <error/Error.hpp>


namespace aasdk
{
namespace channel
{
namespace navigation
{

class INavigationStatusServiceChannelEventHandler
{
public:
    typedef std::shared_ptr<INavigationStatusServiceChannelEventHandler> Pointer;

    INavigationStatusServiceChannelEventHandler() = default;
    virtual ~INavigationStatusServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
    virtual void onStatusUpdate(const proto::messages::NavigationStatus& navStatus) = 0;
    virtual void onTurnEvent(const proto::messages::NavigationTurnEvent& turnEvent) = 0;
    virtual void onDistanceEvent(const proto::messages::NavigationDistanceEvent& distanceEvent) = 0;
};

}
}
}
