package it.smg.libs.aasdk.projection;

public abstract class VideoOutput implements IVideoOutput {

    protected int fps_;
    protected int resolution_;
    protected int dpi_;
    protected Rect margin_;

    public VideoOutput(){
        handle_ = nativeSetup();
    }

    @Override
    public int getVideoFPS() {
        return fps_;
    }

    @Override
    public int getVideoResolution() {
        return resolution_;
    }

    @Override
    public int getScreenDPI() {
        return dpi_;
    }

    @Override
    public Rect getVideoMargins() {
        return margin_;
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
