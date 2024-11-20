package it.smg.hu.projection;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

import java.nio.ByteBuffer;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicBoolean;

import it.smg.libs.common.Log;

public class AudioCodec implements IAudioCodec, Runnable {
    private static final String TAG = "AudioCodec";

    private final String name_;
    private AudioTrack audioTrack_;
    private final int sampleRate_;
    private final int channelConfig_;
    private final int sampleSize_;
    protected AtomicBoolean running_;

    private final Queue<byte[]> queue_;
    private Thread codecThread_;

    public AudioCodec(String name, int sampleRate, int channelConfig, int sampleSize){
        name_ = name;
        sampleRate_ = sampleRate;
        channelConfig_ = channelConfig;
        sampleSize_ = sampleSize;
        running_ = new AtomicBoolean(false);

        queue_ = new ConcurrentLinkedQueue<>();
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
//        if (running_.get()) {
            if (Log.isVerbose()) Log.v(TAG, "buffer size: " + buffer.limit());
            final int size = buffer.limit();
            final byte[] data = new byte[size];
            buffer.get(data);

            queue_.add(data);
//        }
    }

//    @Override
//    public void write(byte[] buffer, long timestamp) {
//        if (running_.get()) {
//            if (Log.isVerbose()) Log.v(TAG, "buffer size: " + buffer.length);
////            final int size = buffer.limit();
////            final byte[] data = new byte[size];
////            buffer.get(data);
//
//            queue_.add(buffer);
//        }
//    }

    @Override
    public void start() {
        if (Log.isInfo()) Log.i(TAG, "Start");

//        if (audioTrack_ == null) {
//            try {
//                int bufferSize_ = AudioTrack.getMinBufferSize(sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_));
//                if (Log.isInfo()) Log.i(TAG, "Buffer size " + bufferSize_ + "*2");
//                audioTrack_ = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_), bufferSize_ * 3, AudioTrack.MODE_STREAM);
//            } catch (Exception e) {
//                Log.e(TAG, "error in audiotrack creation", e);
//                return;
//            }
//
//            if (Log.isInfo()) Log.i(TAG, "initialized");
//        }

        codecThread_ = new Thread(this);
        codecThread_.setName(name_);
        codecThread_.start();

//        if (audioTrack_ != null) {
//            audioTrack_.play();
//        }

        running_.set(true);
    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "Stop");
        if (running_.get()) {
            running_.set(false);

            if (codecThread_ != null){
                try {
                    codecThread_.join();
                    if (Log.isDebug()) Log.d(TAG + "_" + codecThread_.getName(), "thread joined");
                } catch (InterruptedException ignored) {}
                codecThread_ = null;
            }

//            if (audioTrack_ != null) {
//                if (audioTrack_.getPlayState() == AudioTrack.PLAYSTATE_PLAYING) {
//                    audioTrack_.flush();
//                    audioTrack_.stop();
//                }
//                audioTrack_.release();
//                audioTrack_ = null;
//            }
//
//            if (Log.isInfo()) Log.i(TAG, "audtiotrack released");

            queue_.clear();
            if (Log.isDebug()) Log.d(TAG, "queue empty");
        }
    }

    @Override
    public void delete() {
    }

    @Override
    public void run() {
        if (audioTrack_ == null) {
            try {
                int bufferSize_ = AudioTrack.getMinBufferSize(sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_));
                if (Log.isInfo()) Log.i(TAG, "Buffer size " + bufferSize_ + "*2");
                audioTrack_ = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_), bufferSize_ * 3, AudioTrack.MODE_STREAM);
            } catch (Exception e) {
                Log.e(TAG, "error in audiotrack creation", e);
                return;
            }

            if (Log.isInfo()) Log.i(TAG, "initialized");
        }

        if (audioTrack_ != null) {
            audioTrack_.play();
        }

        if (Log.isVerbose()) Log.v(TAG + "_" + codecThread_.getName(), "running thread");
        while (running_.get()) {
            byte[] data = queue_.poll();
            if (data != null){
                audioTrack_.write(data, 0, data.length);
            }
        }

        if (audioTrack_ != null) {
            if (Log.isInfo()) Log.i(TAG, "stop audiotrack");
            if (audioTrack_.getPlayState() == AudioTrack.PLAYSTATE_PLAYING) {
                audioTrack_.flush();
                audioTrack_.stop();
            }
            audioTrack_.release();
            audioTrack_ = null;
        }

        if (Log.isInfo()) Log.i(TAG, "audtiotrack released");

        if (Log.isVerbose()) Log.v(TAG + "_" + codecThread_.getName(), "thread ended");
    }
}
