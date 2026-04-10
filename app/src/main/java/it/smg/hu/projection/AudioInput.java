package it.smg.hu.projection;

import android.media.AudioFormat;
import android.media.AudioRecord;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.libs.common.Log;

public class AudioInput extends it.smg.libs.aasdk.projection.AudioInput {

    private static final String TAG = "AudioInput";

    private final Settings settings_;
    private final HondaConnectManager hondaConnectManager_;

    private AudioRecord audioRecord_;
    private boolean isActive_;

    public AudioInput(){
        settings_ = Settings.instance();
        hondaConnectManager_ = settings_.advanced.hondaIntegrationEnabled() ? HondaConnectManager.instance() : null;

        sampleSize_ = settings_.audio.micSampleSize();
        sourceType_ = settings_.audio.micSource();
        sampleRate_ = settings_.audio.micSampleRate();
        channelConfig_ = settings_.audio.micChannelCount();
    }

    @Keep
    @Override
    public boolean open() {
        bufferSize_ = AudioRecord.getMinBufferSize(sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_));

        if (bufferSize_ <= 0) {
            Log.e(TAG, "Invalid AudioRecord min buffer size " + bufferSize_ + " sampleRate=" + sampleRate_ + " channelMask=" + channelConfig_ + " sampleSize=" + sampleSize_);
            return false;
        }

        return true;
    }

    public static int sampleSizeFromInt(int sampleSize){
        switch (sampleSize){
            case 16:
                return AudioFormat.ENCODING_PCM_16BIT;
            case 8:
                return AudioFormat.ENCODING_PCM_8BIT;
            default:
                return AudioFormat.ENCODING_INVALID;
        }
    }

    public static int channels2num(int channelCount) {
        switch (channelCount) {
            case 2:
                return AudioFormat.CHANNEL_IN_STEREO;
            case 1:
            default:
                return AudioFormat.CHANNEL_IN_MONO;
        }
    }

    @Keep
    @Override
    public boolean isActive() {
        return isActive_;
    }

    @Keep
    @Override
    public int read(ByteBuffer buffer) {
        if (audioRecord_ == null || !isActive_) {
            return 0;
        }

        int bytesToRead = buffer.remaining();
        if (bytesToRead <= 0) {
            return 0;
        }

        return audioRecord_.read(buffer, bytesToRead);
    }

    @Keep
    @Override
    public boolean start() {
        if (audioRecord_ != null) {
            stop();
        }

        isActive_ = true;

        if (settings_.advanced.hondaIntegrationEnabled()) {
            hondaConnectManager_.startMicSession();
        }

        audioRecord_ = new AudioRecord(sourceType_, sampleRate_, channels2num(channelConfig_), sampleSizeFromInt(sampleSize_), bufferSize_ * 4);

        if (Log.isDebug()) Log.d(TAG, "State: " + audioRecord_.getState() + ", recording state: " + audioRecord_.getRecordingState());
        if (audioRecord_.getState() != AudioRecord.STATE_INITIALIZED){
            stop();
            return false;
        }

        try {
            audioRecord_.startRecording();
        } catch (IllegalStateException e) {
            Log.e(TAG, "Unable to start microphone recording", e);
            stop();
            return false;
        }

        if (audioRecord_.getRecordingState() != AudioRecord.RECORDSTATE_RECORDING ){
            Log.e(TAG, "wrong recording state " + audioRecord_.getRecordingState());
            stop();
            return false;
        }

        return true;
    }

    @Keep
    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "stop -> isActive= " + isActive_);

        if (isActive_) {
            if (settings_.advanced.hondaIntegrationEnabled()) {
                hondaConnectManager_.stopMicSession();
            }

            if (audioRecord_ != null) {
                if (Log.isDebug()) Log.d(TAG, "stop audioRecord");
                try {
                    audioRecord_.stop();
                } catch (IllegalStateException e) {
                    Log.e(TAG, "stop audioRecord error", e);
                }

                if (Log.isDebug()) Log.d(TAG, "release audioRecord");
                audioRecord_.release();
            }

            isActive_ = false;
        }
        if (Log.isDebug()) Log.d(TAG, "stop done");
    }

}
