package it.smg.libs.aasdk.projection;

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

    private long handle_;
}
