package it.smg.libs.aasdk.projection;

public abstract class AudioInput implements IAudioInput {

    protected int bufferSize_;
    protected int sampleRate_;
    protected int sampleSize_;
    protected int sourceType_;
    protected int channelConfig_;

    public AudioInput(){
        handle_ = nativeSetup();
    }

    @Override
    public int getSampleSize() {
        return sampleSize_;
    }

    @Override
    public int getChannelCount() {
        return channelConfig_;
    }

    @Override
    public int getSampleRate() {
        return sampleRate_;
    }

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

    private long handle_;
}
