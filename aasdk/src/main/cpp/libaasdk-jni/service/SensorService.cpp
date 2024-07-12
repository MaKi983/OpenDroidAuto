#include <Log.h>
#include "SensorService.h"
#include "proto/DrivingStatusEnum.pb.h"
#include "service/SensorService.h"

namespace service
{

SensorService::SensorService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, IServiceEventHandler::Pointer serviceEventHandler, bool nightMode)
        : strand_(ioService)
        , channel_(std::make_shared<aasdk::channel::sensor::SensorServiceChannel>(strand_, std::move(messenger)))
        , serviceEventHandler_(std::move(serviceEventHandler))
        , nightMode_(nightMode)
        , isRunning_(false)
{

}

SensorService::~SensorService(){
//    delete channel_;
}

void SensorService::start()
{
    isRunning_ = true;
    strand_.dispatch([this, self = this->shared_from_this()]() {
        if(Log::isInfo()) Log_i("start");
        channel_->receive(this->shared_from_this());
    });
}

void SensorService::stop()
{
    if(Log::isInfo()) Log_i("stop");
    isRunning_ = false;
}

void SensorService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
{
    if(Log::isInfo()) Log_i("fill features");

    auto* channelDescriptor = response.add_channels();
    channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));
    auto* sensorChannel = channelDescriptor->mutable_sensor_channel();
    sensorChannel->add_sensors()->set_type(aasdk::proto::enums::SensorType::DRIVING_STATUS);
    //sensorChannel->add_sensors()->set_type(aasdk::proto::enums::SensorType::LOCATION);
    sensorChannel->add_sensors()->set_type(aasdk::proto::enums::SensorType::NIGHT_DATA);

    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s", channelDescriptor->Utf8DebugString().c_str());
}

void SensorService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onChannelOpenRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("open request, priority: %d", request.priority());
    const aasdk::proto::enums::Status::Enum status = aasdk::proto::enums::Status::OK;
    if(Log::isDebug()) Log_d("open status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

    aasdk::proto::messages::ChannelOpenResponse response;
    response.set_status(status);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&SensorService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));

    channel_->receive(this->shared_from_this());
}

void SensorService::onSensorStartRequest(const aasdk::proto::messages::SensorStartRequestMessage& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onSensorStartRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("sensor start request, type: %s", aasdk::proto::enums::SensorType::Enum_Name(request.sensor_type()).c_str());

    aasdk::proto::messages::SensorStartResponseMessage response;
    response.set_status(aasdk::proto::enums::Status::OK);

    auto promise = aasdk::channel::SendPromise::defer(strand_);

    if(request.sensor_type() == aasdk::proto::enums::SensorType::DRIVING_STATUS)
    {
        promise->then(std::bind(&SensorService::sendDrivingStatusUnrestricted, this->shared_from_this()),
                      std::bind(&SensorService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    }
    else if(request.sensor_type() == aasdk::proto::enums::SensorType::NIGHT_DATA)
    {
        promise->then(std::bind(&SensorService::sendNightData, this->shared_from_this()),
                      std::bind(&SensorService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    }
    else
    {
        promise->then([]() {}, std::bind(&SensorService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    }

    channel_->sendSensorStartResponse(response, std::move(promise));
    channel_->receive(this->shared_from_this());
}

void SensorService::sendDrivingStatusUnrestricted()
{
    aasdk::proto::messages::SensorEventIndication indication;
    indication.add_driving_status()->set_status(aasdk::proto::enums::DrivingStatus::UNRESTRICTED);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&SensorService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendSensorEventIndication(indication, std::move(promise));
}

void SensorService::sendNightData()
{
    aasdk::proto::messages::SensorEventIndication indication;
    indication.add_night_mode()->set_is_night(nightMode_);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&SensorService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendSensorEventIndication(indication, std::move(promise));
}

void SensorService::onChannelError(const aasdk::error::Error& e)
{
    if (!isRunning_){
        if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
        return;
    }
//    Log_e("channel error: %s", e.what());
//    serviceEventHandler_->onError(e);
}

void SensorService::setNightMode(bool nightMode)
{
    nightMode_ = nightMode;
    this->sendNightData();
}

}