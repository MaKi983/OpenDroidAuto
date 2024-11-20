package it.smg.hu.projection;

import android.media.AudioRecord;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

import it.smg.hu.config.Settings;
import it.smg.libs.common.Log;

public class AudioInput extends it.smg.libs.aasdk.projection.AudioInput {

    private static final String TAG = "AudioInput";

    private final Settings settings_;

    private AudioRecord audioRecord_;
    private boolean isActive_;

    public AudioInput(){
        settings_ = Settings.instance();

        sampleSize_ = settings_.audio.micSampleSize();;
        sourceType_ = settings_.audio.micSource();
        sampleRate_ = settings_.audio.micSampleRate();
        channelConfig_ = settings_.audio.micChannelCount();
    }

    @Keep
    @Override
    public boolean open() {
        bufferSize_ = AudioRecord.getMinBufferSize(sampleRate_, channelConfig_, sampleSizeFromInt(sampleSize_));

        return true;
    }

    public static int sampleSizeFromInt(int sampleSize){
        switch (sampleSize){
            case 16:
                return 2;
            case 8:
                return 3;
            default:
                return -1;
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
        return audioRecord_.read(buffer,  buffer.limit());
    }

    @Keep
    @Override
    public boolean start() {
        audioRecord_ = new AudioRecord(sourceType_, sampleRate_, channelConfig_, sampleSizeFromInt(sampleSize_), bufferSize_ * 4);

        if (Log.isDebug()) Log.d(TAG, "State: " + audioRecord_.getState() + ", recording state: " + audioRecord_.getRecordingState());
        if (audioRecord_.getState() != AudioRecord.STATE_INITIALIZED){
            return false;
        }

        audioRecord_.startRecording();
        if (audioRecord_.getRecordingState() != AudioRecord.RECORDSTATE_RECORDING ){
            isActive_ = false;
            Log.e(TAG, "wrong recording state " + audioRecord_.getRecordingState());
            return false;
        } else {
            isActive_ = true;
            return true;
        }
    }

    @Keep
    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "stop");
        if (audioRecord_ != null) {
            if (isActive_) {
                if (Log.isDebug()) Log.d(TAG, "stop audioRecord");
                audioRecord_.stop();
            }
            if (Log.isDebug()) Log.d(TAG, "release audioRecord");
            audioRecord_.release();
        }

        isActive_ = false;
        if (Log.isDebug()) Log.d(TAG, "stop done");

        audioRecord_ = null;
    }
}
