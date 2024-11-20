package it.smg.hu.projection;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

import it.smg.hu.config.Settings;
import it.smg.libs.common.Log;


public abstract class AudioOutput extends it.smg.libs.aasdk.projection.AudioOutput {

    protected Settings settings_;

    private IAudioCodec audioCodec_;

    public AudioOutput(){
        settings_ = Settings.instance();
    }

    @Keep
    @Override
    public boolean open() {
        if (Log.isInfo()) Log.i(tag(), "Create Android Audio codec");
        audioCodec_ = new AudioCodec(tag(), getSampleRate(), getChannelCount(), getSampleSize());
        return true;
    }

    @Keep
    @Override
    public void write(long timestamp, ByteBuffer buffer) {
        audioCodec_.write(buffer, timestamp);
    }

//    @Override
//    public void write(long timestamp, byte[] buffer) {
//        audioCodec_.write(buffer, timestamp);
//    }

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
