package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

public abstract class AudioInput implements IAudioInput {

    protected int bufferSize_;
    protected int sampleRate_;
    protected int sampleSize_;
    protected int sourceType_;
    protected int channelConfig_;

    public AudioInput(){
        handle_ = nativeSetup();
    }

    @Keep
    @Override
    public int getSampleSize() {
        return sampleSize_;
    }

    @Keep
    @Override
    public int getChannelCount() {
        return channelConfig_;
    }

    @Keep
    @Override
    public int getSampleRate() {
        return sampleRate_;
    }

    @Keep
    @Override
    public int getBufferSize() {
        return bufferSize_;
    }

    @Override
    public void delete(){
        nativeDelete();
    }

    static {
        nativeInit();
    }

    private native long nativeSetup();
    private native void nativeDelete();
    private static native void nativeInit();

    @Keep
    private long handle_;
}
