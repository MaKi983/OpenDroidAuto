package it.smg.hu.ui.notification;

import it.smg.libs.aasdk.projection.IMediaStatusEvent.*;

public class MediaUpdateEvent implements INotificationEvent {
    final PlaybackUpdateEvent playbackUpdateEvent;
    final MetadataUpdateEvent metadataUpdateEvent;

    MediaUpdateEvent(PlaybackUpdateEvent playbackUpdateEvent, MetadataUpdateEvent metadataUpdateEvent){
        this.playbackUpdateEvent = playbackUpdateEvent;
        this.metadataUpdateEvent = metadataUpdateEvent;
    }
}
