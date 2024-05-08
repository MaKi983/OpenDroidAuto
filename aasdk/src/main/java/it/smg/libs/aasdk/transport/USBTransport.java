package it.smg.libs.aasdk.transport;

import it.smg.libs.aasdk.usb.AOAPDevice;

public class USBTransport extends Transport {

    private AOAPDevice aoapDevice_;

    public USBTransport(AOAPDevice aoapDevice){
        aoapDevice_ = aoapDevice;

        handle_ = nativeSetup(aoapDevice);
    }

    @Override
    public void delete() {
        aoapDevice_.delete();
        aoapDevice_ = null;
        nativeDelete();
        handle_ = 0;
    }

    @Override
    public void stop() {
        nativeStop();
    }

    static {
        nativeInit();
    }
    private static native void nativeInit();

    private native long nativeSetup(AOAPDevice aoapDevice);
    private native void nativeStop();
    private native void nativeDelete();

    private long handle_;
}
