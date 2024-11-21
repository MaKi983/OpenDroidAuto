package it.smg.libs.aasdk.usb;

import androidx.annotation.Keep;

public class AOAPDevice {

    private static final String TAG = "AOAPDevice";

    private LibUsbDevice usbDevice_;

    private static native void nativeInit();
    private native void nativeDelete();
    private native long nativeSetup(LibUsbDevice usbDevice);

    private AOAPDevice(LibUsbDevice usbDevice){
        usbDevice_ = usbDevice;

        handle_ = nativeSetup(usbDevice);
    }

    public void delete(){
        nativeDelete();
        usbDevice_.close();
    }

    public static AOAPDevice create(LibUsbDevice usbDevice) {
        return new AOAPDevice(usbDevice);
    }

    static {
        nativeInit();
    }

    @Keep
    private long handle_;
}
