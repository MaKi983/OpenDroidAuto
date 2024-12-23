#include <projection/IBluetoothDevice.h>
#include <Log.h>

#include "service/BluetoothService.h"

namespace service
{

BluetoothService::BluetoothService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IBluetoothDevice::Pointer bluetoothDevice, IServiceEventHandler::Pointer serviceEventHandler)
        : strand_(ioService)
        , channel_(std::make_shared<aasdk::channel::bluetooth::BluetoothServiceChannel>(strand_, std::move(messenger)))
        , serviceEventHandler_(serviceEventHandler)
        , bluetoothDevice_(bluetoothDevice)
        , isRunning_(false)
{

}

BluetoothService::~BluetoothService(){
    if (Log::isVerbose()) Log_v("destructor");
    channel_.reset();
    serviceEventHandler_ = nullptr;
}

void BluetoothService::start()
{
    isRunning_ = true;
//    strand_.dispatch([this, self = this->shared_from_this()]() {
//    strand_.post([this, self = this->shared_from_this()]() {
        if(Log::isInfo()) Log_i("start");
        channel_->receive(this->shared_from_this());
//    });
}

void BluetoothService::stop()
{
    if(Log::isInfo()) Log_i("stop");
    isRunning_ = false;
    bluetoothDevice_->stop();
}

void BluetoothService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
{
    if(Log::isInfo()) Log_i("fill features");

    if(bluetoothDevice_->isAvailable())
    {
        std::string localAddress = bluetoothDevice_->getLocalAddress();
        if(Log::isInfo()) Log_i("sending local adapter address: %s", localAddress.c_str());

        auto* channelDescriptor = response.add_channels();
        channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));
        auto bluetoothChannel = channelDescriptor->mutable_bluetooth_channel();
        bluetoothChannel->set_adapter_address(localAddress);
        if (bluetoothDevice_->isEnabledHfp()) {
            if(Log::isInfo()) Log_i("Using pairing methods Hfp");
            bluetoothChannel->add_supported_pairing_methods(aasdk::proto::enums::BluetoothPairingMethod_Enum_HFP);
        }
        if (bluetoothDevice_->isEnabledAd2p()) {
            if(Log::isInfo()) Log_i("Using pairing methods Ad2p");
            bluetoothChannel->add_supported_pairing_methods(aasdk::proto::enums::BluetoothPairingMethod_Enum_A2DP);
        }

        if(Log::isVerbose() && Log::logProtocol()) Log_v("%s", channelDescriptor->Utf8DebugString().c_str());
    }
}

void BluetoothService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onChannelOpenRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("open request, priority: %d", request.priority());
    const aasdk::proto::enums::Status::Enum status = aasdk::proto::enums::Status::OK;
    if(Log::isDebug()) Log_d("open status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

    aasdk::proto::messages::ChannelOpenResponse response;
    response.set_status(status);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "BluetoothService_channelOpen");
    promise->then([]() {}, std::bind(&BluetoothService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));

//    channel_->receive(this->shared_from_this());
}

void BluetoothService::onBluetoothPairingRequest(const aasdk::proto::messages::BluetoothPairingRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onBluetoothPairingRequest: %s", request.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("pairing request, address: %s", request.phone_address().c_str());

    aasdk::proto::messages::BluetoothPairingResponse response;

    const auto isPaired = bluetoothDevice_->isPaired(request.phone_address());
    response.set_already_paired(isPaired);
    response.set_status(isPaired ? aasdk::proto::enums::BluetoothPairingStatus::OK : aasdk::proto::enums::BluetoothPairingStatus::FAIL);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "BluetoothService_pairing");
    promise->then([]() {}, std::bind(&BluetoothService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendBluetoothPairingResponse(response, std::move(promise));

//    channel_->receive(this->shared_from_this());
}

void BluetoothService::onChannelError(const aasdk::error::Error& e)
{
    if (!isRunning_){
        if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
        return;
    }
//    Log_e("channel error: %s", e.what());
    serviceEventHandler_->onError(e);
}

}