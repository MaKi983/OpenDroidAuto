package it.smg.hu.projection;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

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
    private boolean micSessionActive_;

    public AudioInput(){
        settings_ = Settings.instance();
        hondaConnectManager_ = settings_.advanced.hondaIntegrationEnabled() ? HondaConnectManager.instance() : null;
        micSessionActive_ = false;

        sampleSize_ = settings_.audio.micSampleSize();
        sourceType_ = settings_.audio.micSource();
        sampleRate_ = settings_.audio.micSampleRate();
        channelConfig_ = settings_.audio.micChannelCount();
    }

    @Keep
    @Override
    public boolean open() {
        int audioFormat = sampleSizeFromInt(sampleSize_);
        int channelMask = channelConfigFromInt(channelConfig_);
        bufferSize_ = AudioRecord.getMinBufferSize(sampleRate_, channelMask, audioFormat);

        if (bufferSize_ <= 0) {
            Log.e(TAG, "Invalid AudioRecord min buffer size " + bufferSize_ + " sampleRate=" + sampleRate_ + " channelMask=" + channelMask + " sampleSize=" + sampleSize_);
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

    public static int channelConfigFromInt(int channelCount) {
        switch (channelCount) {
            case 2:
                return AudioFormat.CHANNEL_IN_STEREO;
            case 1:
            default:
                return AudioFormat.CHANNEL_IN_MONO;
        }
    }

    private int sourceFromInt(int source) {
        if (source <= 0) {
            return MediaRecorder.AudioSource.MIC;
        }
        return source;
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

        if (bufferSize_ <= 0 && !open()) {
            return false;
        }

        if (settings_.advanced.hondaIntegrationEnabled() && settings_.advanced.hondaMicVrEnabled() && hondaConnectManager_ != null) {
            hondaConnectManager_.startMicSession();
            micSessionActive_ = true;
        }

        audioRecord_ = new AudioRecord(sourceFromInt(sourceType_), sampleRate_, channelConfigFromInt(channelConfig_), sampleSizeFromInt(sampleSize_), bufferSize_ * 4);

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

        isActive_ = true;
        return true;
    }

    @Keep
    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "stop");
        AudioRecord oldAudioRecord = audioRecord_;
        audioRecord_ = null;

        if (oldAudioRecord != null) {
            if (isActive_) {
                if (Log.isDebug()) Log.d(TAG, "stop audioRecord");
                try {
                    oldAudioRecord.stop();
                } catch (IllegalStateException e) {
                    Log.e(TAG, "stop audioRecord error", e);
                }
            }
            if (Log.isDebug()) Log.d(TAG, "release audioRecord");
            oldAudioRecord.release();
        }

        isActive_ = false;
        releaseMicSession();
        if (Log.isDebug()) Log.d(TAG, "stop done");
    }

    private void releaseMicSession() {
        if (micSessionActive_ && hondaConnectManager_ != null) {
            hondaConnectManager_.stopMicSession();
        }
        micSessionActive_ = false;
    }
}
