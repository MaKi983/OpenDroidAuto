package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

public interface IMediaStatusEvent {
    void mediaPlaybackUpdate(PlaybackUpdateEvent playbackUpdateEvent);

    void mediaMetadataUpdate(MetadataUpdateEvent metadataUpdateEvent);

    void delete();

    @Keep
    enum PlaybackState {
        NONE,
        TRACK_CHANGE,
        PLAY,
        PAUSE
    }

    @Keep
    class PlaybackUpdateEvent {
        public final PlaybackState playbackState;
        public final String mediaSource;
        public final int trackProgress;

        public PlaybackUpdateEvent(PlaybackState playbackState, String mediaSource, int trackProgress){
            this.playbackState = playbackState;
            this.mediaSource = mediaSource;
            this.trackProgress = trackProgress;
        }
    }

    @Keep
    class MetadataUpdateEvent {
        public final String trackName;
        public final String artistName; //optional
        public final String albumName; //optional
        public final byte[] albumArt; //optional
        public final int trackLength;

        public MetadataUpdateEvent(String trackName, String artistName, String albumName, ByteBuffer albumArt, int trackLength){
            this.trackName = trackName;
            this.artistName = artistName;
            this.albumName = albumName;
            this.albumArt = clone(albumArt);
            this.trackLength = trackLength;
        }

        private byte[] clone(ByteBuffer b){
            if (b == null){
                return null;
            }

            final byte[] clone = new byte[b.capacity()];
            b.get(clone);
            return clone;
        }
    }
}
