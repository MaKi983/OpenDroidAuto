package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

public abstract class AudioOutput implements IAudioOutput {

    protected int sampleRate_;
    protected int sampleSize_;
    protected int channelConfig_;

    public AudioOutput(){
        handle_ = nativeSetup();
    }

    @Keep
    @Override
    public int getSampleSize(){
        return sampleSize_;
    }

    @Keep
    @Override
    public int getChannelCount(){
        return channelConfig_;
    }

    @Keep
    @Override
    public int getSampleRate(){
        return sampleRate_;
    }

    @Override
    public void delete() {
        nativeDelete();
    }

    public abstract String tag();

    static {
        nativeInit();
    }

    private native long nativeSetup();
    private native void nativeDelete();
    private static native void nativeInit();

    @Keep
    private long handle_;
}
