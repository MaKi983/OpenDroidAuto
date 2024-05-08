package it.smg.hu.projection;

public class SpeechAudioOutput extends AudioOutput {

    private static final String TAG = "SpeechAudioOutput";

    public SpeechAudioOutput(){
        super();

        channelConfig_ = settings_.audio.speechChannelCount();
        sampleSize_ = settings_.audio.speechSampleSize();
        sampleRate_ = settings_.audio.speechSampleRate();
    }

    @Override
    public String tag() {
        return TAG;
    }
}
