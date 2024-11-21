package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

public abstract class MediaStatusEvent implements IMediaStatusEvent {

    public MediaStatusEvent(){
        handle_ = nativeSetup();
    }

    static {
        nativeInit();
    }

    @Override
    public void delete() {
        nativeDelete();
    }

    private static native void nativeInit();
    private native long nativeSetup();
    private native void nativeDelete();

    @Keep
    private long handle_;
}
