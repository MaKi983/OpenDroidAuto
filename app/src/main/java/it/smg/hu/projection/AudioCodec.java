package it.smg.hu.projection;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Handler;
import android.os.HandlerThread;

import java.nio.ByteBuffer;

import it.smg.hu.config.Settings;
import it.smg.libs.common.Log;

public class AudioCodec implements IAudioCodec {
    private static final String TAG = "AudioCodec";

    private AudioTrack audioTrack_;
    private int sampleRate_;
    private int channelConfig_;
    private int sampleSize_;
    protected boolean running_;
    private HandlerThread codecThread_;
    private Handler codecHandler_;

    public AudioCodec(int sampleRate, int channelConfig, int sampleSize){
        sampleRate_ = sampleRate;
        channelConfig_ = channelConfig;
        sampleSize_ = sampleSize;
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
    public void write(ByteBuffer buffer, long timestamp) {
        if (running_) {
            if (Log.isVerbose()) Log.v(TAG, "buffer size: " + buffer.limit());
//            if (Log.isVerbose()) Log.v(TAG, buffer);
            final int size = buffer.limit();
            final byte[] data = new byte[size];
            buffer.get(data);
//            if (Log.isVerbose()) Log.v(TAG, data);

            codecHandler_.post(() -> {
//                if (Log.isVerbose()) Log.v(TAG, "in thread");
//                if (Log.isVerbose()) Log.v(TAG, data);
                audioTrack_.write(data, 0, size);
            });
        }
    }

    @Override
    public void start() {
        if (Log.isInfo()) Log.i(TAG, "Start");

        codecThread_ = new HandlerThread("AudioCodec-thread");
        codecThread_.start();
        codecHandler_ = new Handler(codecThread_.getLooper());

        if (audioTrack_ == null) {
            try {
                int bufferSize_ = AudioTrack.getMinBufferSize(sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_));
                if (Log.isInfo()) Log.i(TAG, "Buffer size " + bufferSize_ + "*2");
                audioTrack_ = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_), bufferSize_, AudioTrack.MODE_STREAM);
            } catch (Exception e) {
                Log.e(TAG, "error in audiotrack creation", e);
                return;
            }

            if (Log.isInfo()) Log.i(TAG, "initialized");
        }

        if (audioTrack_ != null) {
            audioTrack_.play();
        }

        running_ = true;
    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "Stop");
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

            if (Log.isInfo()) Log.i(TAG, "audtiotrack released");

//           codecHandler_.getLooper().quit();

            if (codecThread_ != null) {
                if (Log.isDebug()) Log.d(TAG, "interrupt codecThread_");
                codecThread_.interrupt();
                codecThread_ = null;
            }
            codecHandler_ = null;
        }
    }

}
