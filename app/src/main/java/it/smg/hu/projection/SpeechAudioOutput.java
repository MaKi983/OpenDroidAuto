package it.smg.hu.projection;

import it.smg.libs.aasdk.messenger.ChannelId;

public class SpeechAudioOutput extends AudioOutput {

    private static final String TAG = "SpeechAudioOutput";

    public SpeechAudioOutput(){
        super();

        channelConfig_ = settings_.audio.speechChannelCount();
        sampleSize_ = settings_.audio.speechSampleSize();
        sampleRate_ = settings_.audio.speechSampleRate();

        if (settings_.advanced.hondaIntegrationEnabled()){
            audioCodecStreamType_ = hondaConnectManager_.mediaAudioStream(ChannelId.SPEECH_AUDIO);
        }
    }

    @Override
    public String tag() {
        return TAG;
    }
}
