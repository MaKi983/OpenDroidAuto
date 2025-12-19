package it.smg.hu.projection;

import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.common.Log;

public class MediaAudioOutput extends AudioOutput {

    private static final String TAG = "MediaAudioOutput";

    public MediaAudioOutput(){
        super();

        channelConfig_ = settings_.audio.mediaChannelCount();
        sampleSize_ = settings_.audio.mediaSampleSize();
        sampleRate_ = settings_.audio.mediaSampleRate();

        if (settings_.advanced.hondaIntegrationEnabled()){
            audioCodecStreamType_ = hondaConnectManager_.mediaAudioStream(ChannelId.MEDIA_AUDIO);
        }
    }

    @Override
    public void start() {
        if (settings_.advanced.hondaIntegrationEnabled()){
            if (Log.isDebug()) Log.d(TAG, "requestAudioFocus");
            hondaConnectManager_.requestAudioFocus();
        }

        super.start();
    }

    @Override
    public void stop() {
        if (settings_.advanced.hondaIntegrationEnabled()){
            if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus");
            hondaConnectManager_.releaseAudioFocus();
        }

        super.stop();
    }

    @Override
    public void suspend() {
        if (settings_.advanced.hondaIntegrationEnabled()){
            if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus");
            hondaConnectManager_.releaseAudioFocus();
        }

        super.suspend();
    }

    @Override
    public String tag() {
        return TAG;
    }
}
