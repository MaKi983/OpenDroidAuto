#pragma once

#include "proto/NavigationStatusMessage.pb.h"
#include "proto/NavigationDistanceEventMessage.pb.h"
#include "proto/NavigationTurnEventMessage.pb.h"

namespace projection
{

    class INavigationStatusEvent
    {
    public:
        typedef INavigationStatusEvent* Pointer;

        virtual ~INavigationStatusEvent() = default;

        virtual void navigationStatusUpdate(const aasdk::proto::messages::NavigationStatus& navStatus) = 0;
        virtual void navigationTurnEvent(const aasdk::proto::messages::NavigationTurnEvent& turnEvent) = 0;
        virtual void navigationDistanceEvent(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent) = 0;
    };



}