package it.smg.libs.aasdk.projection;

public abstract class NavigationStatusEvent implements INavigationStatusEvent {

    public NavigationStatusEvent(){
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
