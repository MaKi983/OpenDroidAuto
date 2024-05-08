package it.smg.hu.projection;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

import java.nio.ByteBuffer;

import it.smg.hu.config.Settings;
import it.smg.libs.common.Log;

public abstract class AudioOutput extends it.smg.libs.aasdk.projection.AudioOutput /*implements Runnable*/ {
    private AudioTrack audioTrack_;

    protected Settings settings_;
    protected boolean running_;

    public AudioOutput(){
        settings_ = Settings.instance();
    }

    private static int channels2num(int channels){
        switch (channels){
            case 1:
                return AudioFormat.CHANNEL_OUT_MONO;
            case 2:
                return AudioFormat.CHANNEL_OUT_STEREO;
            case 4:
                return AudioFormat.CHANNEL_OUT_QUAD;
            case 6:
                return AudioFormat.CHANNEL_OUT_5POINT1;
            case 8:
                return AudioFormat.CHANNEL_OUT_7POINT1_SURROUND;
            default:
                return -1;
        }
    }

    public static int sampleSizeFromInt(int sampleSize){
        switch (sampleSize){
            case 16:
                return AudioFormat.ENCODING_PCM_16BIT;
            case 8:
                return AudioFormat.ENCODING_PCM_8BIT;
            default:
                return -1;
        }
    }

    @Override
    public boolean open() {
        return true;
    }

    @Override
    public void write(long timestamp, ByteBuffer buffer) {
        if (running_) {
            int size = buffer.limit();
            byte[] data = new byte[size];
            buffer.get(data);
            audioTrack_.write(data, 0, size);
        }
    }

    @Override
    public void start() {
        if (audioTrack_ == null) {
            try {
                int bufferSize_ = AudioTrack.getMinBufferSize(sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_));
                if (Log.isInfo()) Log.i(tag(), "Buffer size " + bufferSize_ + "*2");
                audioTrack_ = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_), bufferSize_ * 2, AudioTrack.MODE_STREAM);
            } catch (Exception e) {
                Log.e(tag(), "error in audiotrack creation", e);
                return;
            }

            if (Log.isInfo()) Log.i(tag(), "initialized");
        }

        if (audioTrack_ != null) {
            audioTrack_.play();
        }

        running_ = true;
    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(tag(), "Stopping");
        suspend();

        if (Log.isInfo()) Log.i(tag(), "Stop");
    }

    @Override
    public void suspend() {
        if (Log.isInfo()) Log.i(tag(), "Suspend");
        if (running_) {
            running_ = false;

            if (audioTrack_ != null) {
                if (audioTrack_.getPlayState() == AudioTrack.PLAYSTATE_PLAYING) {
                    audioTrack_.flush();
                    audioTrack_.stop();
                }
                audioTrack_.release();
                audioTrack_ = null;
            }

            if (Log.isInfo()) Log.i(tag(), "audtiotrack released");
        }

    }

    public abstract String tag();

}
