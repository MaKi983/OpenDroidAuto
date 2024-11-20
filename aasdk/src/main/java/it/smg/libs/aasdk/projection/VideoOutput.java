package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

public abstract class VideoOutput implements IVideoOutput {

    protected int fps_;
    protected int resolution_;
    protected int dpi_;
    protected Rect margin_;

    public VideoOutput(){
        handle_ = nativeSetup();
    }

    @Keep
    @Override
    public int getVideoFPS() {
        return fps_;
    }

    @Keep
    @Override
    public int getVideoResolution() {
        return resolution_;
    }

    @Keep
    @Override
    public int getScreenDPI() {
        return dpi_;
    }

    @Keep
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

    @Keep
    private long handle_;
}
