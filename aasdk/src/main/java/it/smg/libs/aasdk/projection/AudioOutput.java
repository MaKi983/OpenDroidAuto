package it.smg.libs.aasdk.projection;

public abstract class AudioOutput implements IAudioOutput {

    protected int sampleRate_;
    protected int sampleSize_;
    protected int channelConfig_;

    public AudioOutput(){
        handle_ = nativeSetup();
    }

    @Override
    public int getSampleSize(){
        return sampleSize_;
    }

    @Override
    public int getChannelCount(){
        return channelConfig_;
    }

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

    private long handle_;
}
