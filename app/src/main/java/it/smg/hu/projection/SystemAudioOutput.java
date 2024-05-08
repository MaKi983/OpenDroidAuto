package it.smg.hu.projection;

public class SystemAudioOutput extends AudioOutput {

    private static final String TAG = "SystemAudioOutput";

    public SystemAudioOutput(){
        super();

        channelConfig_ = settings_.audio.systemChannelCount();
        sampleSize_ = settings_.audio.systemSampleSize();
        sampleRate_ = settings_.audio.systemSampleRate();
    }

    @Override
    public String tag() {
        return TAG;
    }
}
