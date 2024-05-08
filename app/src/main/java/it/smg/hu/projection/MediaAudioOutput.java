package it.smg.hu.projection;

public class MediaAudioOutput extends AudioOutput {

    private static final String TAG = "MediaAudioOutput";

    public MediaAudioOutput(){
        super();

        channelConfig_ = settings_.audio.mediaChannelCount();
        sampleSize_ = settings_.audio.mediaSampleSize();
        sampleRate_ = settings_.audio.mediaSampleRate();
    }

    @Override
    public String tag() {
        return TAG;
    }
}
