package it.smg.hu.projection;

import android.media.AudioTrack;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.libs.common.Log;


public abstract class AudioOutput extends it.smg.libs.aasdk.projection.AudioOutput {

    protected Settings settings_;
    protected int audioCodecStreamType_;
    protected HondaConnectManager hondaConnectManager_;

    private IAudioCodec audioCodec_;

    public AudioOutput(){
        settings_ = Settings.instance();
        if (settings_.advanced.hondaIntegrationEnabled()) {
            hondaConnectManager_ = HondaConnectManager.instance();
        }

        audioCodecStreamType_ = AudioTrack.MODE_STREAM;
    }

    @Keep
    @Override
    public boolean open() {
        if (Log.isInfo()) Log.i(tag(), "Create Android Audio codec");
        audioCodec_ = new AudioCodec(tag(), audioCodecStreamType_, getSampleRate(), getChannelCount(), getSampleSize());
        return true;
    }

    @Keep
    @Override
    public void write(long timestamp, ByteBuffer buffer) {
        audioCodec_.write(buffer, timestamp);
    }

    @Keep
    @Override
    public void start() {
        if (audioCodec_ != null) {
            audioCodec_.start();
        }
    }

    @Keep
    @Override
    public void stop() {
        if (audioCodec_ != null) {
            audioCodec_.stop();
//            audioCodec_ = null;
        }
    }

    @Keep
    @Override
    public void suspend() {
        if (audioCodec_ != null) {
            audioCodec_.stop();
        }
    }

    @Override
    public void delete() {
        if (audioCodec_ != null){
            audioCodec_.delete();
            audioCodec_ = null;
        }
        super.delete();
    }

    public abstract String tag();

}
