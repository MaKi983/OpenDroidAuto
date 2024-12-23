#include <Log.h>
#include "service/NavigationStatusService.h"
#include "proto/ManeuverTypeEnum.pb.h"
#include "proto/ManeuverDirectionEnum.pb.h"
#include "proto/DistanceUnitEnum.pb.h"

namespace service
{

    NavigationStatusService::NavigationStatusService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::INavigationStatusEvent::Pointer navigationEvent, IServiceEventHandler::Pointer serviceEventHandler)
            : strand_(ioService)
            , channel_(std::make_shared<aasdk::channel::navigation::NavigationStatusServiceChannel>(strand_, std::move(messenger)))
            , serviceEventHandler_(serviceEventHandler)
            , isRunning_(false)
    {
        this->navigationEvent_ = navigationEvent;
    }

    NavigationStatusService::~NavigationStatusService(){
        if (Log::isVerbose()) Log_v("destructor");
        channel_.reset();
        serviceEventHandler_ = nullptr;
    }

    void NavigationStatusService::start()
    {
        isRunning_ = true;
//        strand_.dispatch([this, self = this->shared_from_this()]() {
//        strand_.post([this, self = this->shared_from_this()]() {
            if(Log::isInfo()) Log_i("start");
            channel_->receive(this->shared_from_this());
//        });
    }

    void NavigationStatusService::stop()
    {
        if(Log::isInfo()) Log_i("stop");
        isRunning_ = false;
    }

    void NavigationStatusService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
    {
        if(Log::isInfo()) Log_i("fill features");

        auto* channelDescriptor = response.add_channels();
        channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));
        auto navStatusChannel = channelDescriptor->mutable_navigation_channel();
        navStatusChannel->set_minimum_interval_ms(1000);
        navStatusChannel->set_type(aasdk::proto::enums::NavigationTurnType::IMAGE);
        auto* imageOptions = new aasdk::proto::data::NavigationImageOptions();
        imageOptions->set_colour_depth_bits(32);
        imageOptions->set_height(256);
        imageOptions->set_width(256);
        imageOptions->set_dunno(255);
        navStatusChannel->set_allocated_image_options(imageOptions);
    }

    void NavigationStatusService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
    {
        if(Log::isVerbose() && Log::logProtocol()) Log_v("onChannelOpenRequest: %s", request.Utf8DebugString().c_str());

        if(Log::isDebug()) Log_d("open request, priority: %d", request.priority());
        const aasdk::proto::enums::Status::Enum status = aasdk::proto::enums::Status::OK;
        if(Log::isDebug()) Log_d("open status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

        aasdk::proto::messages::ChannelOpenResponse response;
        response.set_status(status);

        auto promise = aasdk::channel::SendPromise::defer(strand_, "NavigationStatusService_channelOpen");
        promise->then([]() {}, std::bind(&NavigationStatusService::onChannelError, this->shared_from_this(), std::placeholders::_1));
        channel_->sendChannelOpenResponse(response, std::move(promise));

//        channel_->receive(this->shared_from_this());
    }


    void NavigationStatusService::onChannelError(const aasdk::error::Error& e)
    {
        if (!isRunning_){
            if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
            return;
        }
//        Log_e("channel error: %s", e.what());
        serviceEventHandler_->onError(e);
    }

    void NavigationStatusService::onStatusUpdate(const aasdk::proto::messages::NavigationStatus& navStatus)
    {
        if(Log::isInfo()) Log_i("Navigation Status Update, Status: %s", (navStatus.has_status() ? aasdk::proto::messages::NavigationStatus_Enum_Name(navStatus.status()).c_str() : "") );
        navigationEvent_->navigationStatusUpdate(navStatus);
//        channel_->receive(this->shared_from_this());
    }

    void NavigationStatusService::onTurnEvent(const aasdk::proto::messages::NavigationTurnEvent& turnEvent)
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s", turnEvent.Utf8DebugString().c_str());
        if(Log::isInfo()) Log_i("Turn Event, Street: %s, Maneuver: %s %s", (turnEvent.has_street_name() && !turnEvent.street_name().empty() ? turnEvent.street_name().c_str() : "") , (turnEvent.has_maneuverdirection() ? aasdk::proto::enums::ManeuverDirection_Enum_Name(turnEvent.maneuverdirection()).c_str() : ""), (turnEvent.has_maneuvertype() ? aasdk::proto::enums::ManeuverType_Enum_Name(turnEvent.maneuvertype()).c_str() : ""));
        navigationEvent_->navigationTurnEvent(turnEvent);
//        channel_->receive(this->shared_from_this());
    }

    void NavigationStatusService::onDistanceEvent(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent)
    {
        if (Log::isVerbose() && Log::logProtocol()) Log_v("%s", distanceEvent.Utf8DebugString().c_str());
        if(Log::isInfo()) Log_i("Distance Event, Distance (meters): %d, Time To Turn (seconds): %d, Distance: %d" /*(%s)*/, (distanceEvent.has_meters() ? distanceEvent.meters() : -1), (distanceEvent.has_timetostepseconds() ? distanceEvent.timetostepseconds() : -1), (distanceEvent.has_distancetostepmillis() ? distanceEvent.distancetostepmillis()/1000.0 : -1) /*, (distanceEvent.has_distanceunit() ? aasdk::proto::enums::DistanceUnit_Enum_Name(distanceEvent.distanceunit()).c_str() : "")*/ );
        navigationEvent_->navigationDistanceEvent(distanceEvent);
//        channel_->receive(this->shared_from_this());
    }

}
