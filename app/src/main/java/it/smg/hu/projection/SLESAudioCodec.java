package it.smg.hu.projection;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

public class SLESAudioCodec implements IAudioCodec {
    private static final String TAG = "SLESAudioCodec";

    private it.smg.libs.slesaudiocodec.SLESAudioCodec audioCodec_;

    private int sampleRate_;
    private int channelConfig_;
    private int sampleSize_;
    protected boolean running_;

    public SLESAudioCodec(int sampleRate, int channelConfig, int sampleSize){
        sampleRate_ = sampleRate;
        channelConfig_ = channelConfig;
        sampleSize_ = sampleSize;
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
        if (audioCodec_ == null) {
            audioCodec_ = new it.smg.libs.slesaudiocodec.SLESAudioCodec(sampleRate_, channelConfig_, sampleSize_);
            running_ = true;
        }
    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "Suspend");
        if (running_) {
            running_ = false;

            if (audioCodec_ != null) {
                audioCodec_.shutdown();
                audioCodec_ = null;
            }

            if (Log.isInfo()) Log.i(TAG, "audioCodec_ destroyed");
        }

    }
}
