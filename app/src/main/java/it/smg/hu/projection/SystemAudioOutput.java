package it.smg.hu.projection;

import it.smg.libs.aasdk.messenger.ChannelId;

public class SystemAudioOutput extends AudioOutput {

    private static final String TAG = "SystemAudioOutput";

    public SystemAudioOutput(){
        super();

        channelConfig_ = settings_.audio.systemChannelCount();
        sampleSize_ = settings_.audio.systemSampleSize();
        sampleRate_ = settings_.audio.systemSampleRate();

        if (settings_.advanced.hondaIntegrationEnabled()){
            audioCodecStreamType_ = hondaConnectManager_.mediaAudioStream(ChannelId.SYSTEM_AUDIO);
        }
    }

    @Override
    public String tag() {
        return TAG;
    }
}
