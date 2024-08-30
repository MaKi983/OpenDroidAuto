package it.smg.hu.projection;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

public class SLESAudioCodec implements IAudioCodec {
    private static final String TAG = "SLESAudioCodec";

    private it.smg.libs.slesaudiocodec.SLESAudioCodec audioCodec_;

    private String name_;
    private int sampleRate_;
    private int channelConfig_;
    private int sampleSize_;
    protected boolean running_;

    public SLESAudioCodec(String name, int sampleRate, int channelConfig, int sampleSize){
        name_ = name;
        sampleRate_ = sampleRate;
        channelConfig_ = channelConfig;
        sampleSize_ = sampleSize;

        if (Log.isInfo()) Log.i(TAG, name_ + "create audiocodec");
        audioCodec_ = new it.smg.libs.slesaudiocodec.SLESAudioCodec(name_, sampleRate_, channelConfig_, sampleSize_);
    }

    @Override
    public void write(ByteBuffer buffer, long timestamp) {
        if (running_) {
            if (Log.isVerbose()) Log.v(TAG, "write buffer size: " + buffer.limit());
//            if (Log.isVerbose()) Log.v(tag(), buffer);
            audioCodec_.write(timestamp, buffer, buffer.limit());
        }
    }

    @Override
    public void start() {
        if (Log.isInfo()) Log.i(TAG, "Start");
        if (audioCodec_ != null) {
            audioCodec_.start();
            running_ = true;
        }
    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "Stop");
        if (running_) {
            running_ = false;

            if (audioCodec_ != null) {
                audioCodec_.stop();
            }

            if (Log.isInfo()) Log.i(TAG, "audioCodec_ destroyed");
        }
    }

    @Override
    public void delete() {
        if (Log.isInfo()) Log.i(TAG, "Delete");
        if (audioCodec_ != null) {
            audioCodec_.delete();
            audioCodec_ = null;
        }
    }
}
