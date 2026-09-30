package it.smg.hu.projection;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

import java.nio.ByteBuffer;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import it.smg.libs.common.Log;

public class AudioCodec implements IAudioCodec, Runnable {
    private static final String TAG = "AudioCodec";

    private final String name_;
    private final int streamType_;
    private final int sampleRate_;
    private final int channelConfig_;
    private final int sampleSize_;
    protected AtomicBoolean running_;

    private final BlockingQueue<byte[]> queue_;
    /**
     * The thread that currently owns playback. Written by start()/stop() on the
     * session threads and read by the playback loop to notice it has been
     * superseded, so it is published explicitly.
     */
    private volatile Thread codecThread_;

    public AudioCodec(String name, int streamType, int sampleRate, int channelConfig, int sampleSize){
        name_ = name;
        streamType_ = streamType;
        sampleRate_ = sampleRate;
        channelConfig_ = channelConfig;
        sampleSize_ = sampleSize;
        running_ = new AtomicBoolean(false);

        queue_ = new ArrayBlockingQueue<>(32);
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
        if (Log.isVerbose()) Log.v(TAG, "buffer size: " + buffer.limit());
        ByteBuffer source = buffer.slice();
        final int size = source.remaining();
        if (size <= 0) {
            return;
        }
        final byte[] data = new byte[size];
        source.get(data);

        if (!queue_.offer(data)) {
            queue_.poll();
            queue_.offer(data);
        }
    }

    @Override
    public void start() {
        if (Log.isInfo()) Log.i(TAG, "Start");

        if (running_.getAndSet(true)) {
            return;
        }
        Thread codecThread = new Thread(this);
        codecThread.setName(name_);
        // Published before start() so the new thread's own identity check passes.
        codecThread_ = codecThread;
        codecThread.start();
    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "Stop");
        if (running_.get()) {
            running_.set(false);

            Thread codecThread = codecThread_;
            // Cleared before joining, so a thread still blocked inside
            // AudioTrack.write() sees it has been superseded and bails out on its
            // own once the write returns, even if the join below times out.
            codecThread_ = null;
            if (codecThread != null){
                try {
                    codecThread.join(1000);
                    if (Log.isDebug()) Log.d(TAG + "_" + name_, "thread joined");
                } catch (InterruptedException ignored) {
                    Thread.currentThread().interrupt();
                }
            }

            queue_.clear();
            if (Log.isDebug()) Log.d(TAG, "queue empty");
        }
    }

    @Override
    public void delete() {
    }

    @Override
    public void run() {
        // The AudioTrack is owned by this thread alone. It used to be a field, so
        // a playback thread that outlived its stop() (the join below is bounded)
        // could release the track a freshly started thread was already writing to.
        final Thread self = Thread.currentThread();
        AudioTrack audioTrack;
        try {
            int bufferSize = AudioTrack.getMinBufferSize(sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_));
            if (bufferSize <= 0) {
                Log.e(TAG, "Invalid AudioTrack buffer size " + bufferSize);
                running_.set(false);
                return;
            }
            if (Log.isInfo()) Log.i(TAG, "Buffer size " + bufferSize + "*3");
            audioTrack = new AudioTrack(streamType_, sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_), bufferSize * 3, AudioTrack.MODE_STREAM);
        } catch (Exception e) {
            Log.e(TAG, "error in audiotrack creation", e);
            running_.set(false);
            return;
        }

        if (Log.isInfo()) Log.i(TAG, "initialized");

        try {
            if (Log.isInfo()) Log.i(TAG, "starting audiotrack");
            audioTrack.play();

            if (Log.isVerbose()) Log.v(TAG + "_" + name_, "running thread");
            // codecThread_ != self means stop() (or a restart) has moved on
            // without us: give up playback instead of fighting the new thread.
            while (running_.get() && codecThread_ == self) {
                byte[] data = null;
                try {
                    data = queue_.poll(50, TimeUnit.MILLISECONDS);
                } catch (InterruptedException ignored) {
                    Thread.currentThread().interrupt();
                    break;
                }
                if (data != null){
                    audioTrack.write(data, 0, data.length);
                }
            }
        } finally {
            if (Log.isInfo()) Log.i(TAG, "stop audiotrack");
            try {
                if (audioTrack.getPlayState() == AudioTrack.PLAYSTATE_PLAYING) {
                    audioTrack.flush();
                    audioTrack.stop();
                }
            } catch (IllegalStateException e) {
                Log.e(TAG, "error stopping audiotrack", e);
            }
            audioTrack.release();
            if (Log.isInfo()) Log.i(TAG, "audiotrack released");
        }

        if (Log.isVerbose()) Log.v(TAG + "_" + name_, "thread ended");
    }
}
