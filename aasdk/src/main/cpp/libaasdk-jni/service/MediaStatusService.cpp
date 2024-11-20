#include <Log.h>
#include "service/MediaStatusService.h"

namespace service
{

    MediaStatusService::MediaStatusService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IMediaStatusEvent::Pointer mediaEvent, IServiceEventHandler::Pointer serviceEventHandler)
            : strand_(ioService)
            , channel_(std::make_shared<aasdk::channel::av::MediaStatusServiceChannel>(strand_, std::move(messenger)))
            , isRunning_(false)
    {
        this->mediaEvent_ = mediaEvent;
    }

    MediaStatusService::~MediaStatusService(){
//        delete channel_;
    }

    void MediaStatusService::start()
    {
        isRunning_ = true;
//        strand_.dispatch([this, self = this->shared_from_this()]() {
//        strand_.post([this, self = this->shared_from_this()]() {
            if(Log::isInfo()) Log_i("start");
            channel_->receive(this->shared_from_this());
//        });
    }

    void MediaStatusService::stop()
    {
        if(Log::isInfo()) Log_i("stop");
        isRunning_ = false;
    }

    void MediaStatusService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
    {
        if(Log::isInfo()) Log_i("fill features");

        auto* channelDescriptor = response.add_channels();
        auto mediaStatusChannel = channelDescriptor->mutable_media_infochannel();
        channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));
    }

    void MediaStatusService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
    {
        if(Log::isVerbose() && Log::logProtocol()) Log_v("onChannelOpenRequest: %s", request.Utf8DebugString().c_str());

        if(Log::isDebug()) Log_d("open request, priority: %d", request.priority());
        const aasdk::proto::enums::Status::Enum status = aasdk::proto::enums::Status::OK;
        if(Log::isDebug()) Log_d("open status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

        aasdk::proto::messages::ChannelOpenResponse response;
        response.set_status(status);

        auto promise = aasdk::channel::SendPromise::defer(strand_, "MediaStatusService_channelOpen");
        promise->then([]() {}, std::bind(&MediaStatusService::onChannelError, this->shared_from_this(), std::placeholders::_1));
        channel_->sendChannelOpenResponse(response, std::move(promise));

//        channel_->receive(this->shared_from_this());
    }


    void MediaStatusService::onChannelError(const aasdk::error::Error& e)
    {
        if (!isRunning_){
            if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
            return;
        }
//        Log_e("channel error: %s", e.what());
        serviceEventHandler_->onError(e);
    }

    void MediaStatusService::onMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata)
    {
        if(Log::isDebug()) Log_d("Metadata update, track: %s, artist: %s, album: %s, length: %d, album_art: %d bytes", (metadata.has_track_name() ? metadata.track_name().c_str() : ""), (metadata.has_artist_name()?metadata.artist_name().c_str():""), (metadata.has_album_name()?metadata.album_name().c_str():""), (metadata.has_track_length() ? metadata.track_length() : -1), (metadata.has_album_art() ? metadata.album_art().size() : -1));
        mediaEvent_->mediaMetadataUpdate(metadata);
//        channel_->receive(this->shared_from_this());
    }

    void MediaStatusService::onPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback)
    {
        if(Log::isDebug()) Log_d("Playback update, source: %s, state: %s, progress: %d", (playback.has_media_source() ? playback.media_source().c_str() : ""), (playback.has_playback_state() ? aasdk::proto::messages::MediaInfoChannelPlaybackData_PlaybackState_Name(playback.playback_state()).c_str() : ""), (playback.has_track_progress() ? playback.track_progress() : -1) );
        mediaEvent_->mediaPlaybackUpdate(playback);
//        channel_->receive(this->shared_from_this());
    }

}