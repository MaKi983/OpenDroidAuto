package it.smg.hu.projection;

import it.smg.hu.config.Settings;
import it.smg.hu.ui.PlayerActivity;
import it.smg.hu.ui.notification.NotificationFactory;

public class MediaStatusEvent extends it.smg.libs.aasdk.projection.MediaStatusEvent {

    public final static String PLAYBACK_STATE_EXTRA = "playbackState";
    public final static String MEDIA_SOURCE_EXTRA = "mediaSource";
    public final static String TRACK_PROGRESS_EXTRA = "trackProgress";
    public final static String TRACK_NAME_EXTRA = "trackName";
    public final static String ARTIST_NAME_EXTRA = "artistName";
    public final static String ALBUM_NAME_EXTRA = "albumName";
    public final static String ALBUM_ART_EXTRA = "albumArt";
    public final static String TRACK_LENGTH_EXTRA = "trackLength";

    private Settings settings_;
    private NotificationFactory notificationFactory_;

    public MediaStatusEvent(){
        settings_ = Settings.instance();
        notificationFactory_ = NotificationFactory.instance();
    }

    private PlaybackUpdateEvent playbackUpdateEvent_;
    private MetadataUpdateEvent metadataUpdateEvent_;

    @Override
    public void mediaPlaybackUpdate(PlaybackUpdateEvent playbackUpdateEvent) {
        if (!settings_.video.showMediaNotification()) {
            return;
        }

        playbackUpdateEvent_ = playbackUpdateEvent;

        if (metadataUpdateEvent_ != null){
            if (!PlayerActivity.isActive()){
                notificationFactory_.notifyMediaMetadataUpdate(playbackUpdateEvent_, metadataUpdateEvent_);
            }
        }
    }

    @Override
    public void mediaMetadataUpdate(MetadataUpdateEvent metadataUpdateEvent) {
        if (!settings_.video.showMediaNotification()) {
            return;
        }

        metadataUpdateEvent_ = metadataUpdateEvent;

        if (!PlayerActivity.isActive()){
            notificationFactory_.notifyMediaMetadataUpdate(playbackUpdateEvent_, metadataUpdateEvent);
        }
    }
}
