package it.smg.libs.aasdk.usb;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;

import androidx.annotation.Keep;

import java.nio.charset.Charset;

import it.smg.libs.common.Log;

public final class LibUsbDevice {

    private static final String TAG = "LibUsbDevice";

    public final String manufacter;
    public final String product;
    public final String serial;

    private UsbManager usbManager_;
    private UsbDevice usbDevice_;
    private UsbDeviceConnection usbConnection_;

    private static native void nativeInit();
    private native long nativeSetup(LibUsb libUsb);
    private native int nativeDeviceDescriptor();
    private native int nativeOpen(int fd);
    private native void nativeClose();
    private native byte[] nativeManufacter();
    private native byte[] nativeProduct();
    private native byte[] nativeSerial();

    /*package*/ LibUsbDevice(LibUsb libUsb, UsbDevice device, UsbManager usbManager){
        usbDevice_ = device;
        usbManager_ = usbManager;
        handle_ = nativeSetup(libUsb);

        String _manufacter = String.valueOf(usbDevice_.getVendorId());
        String _product = String.valueOf(usbDevice_.getProductId());
        String _serial = "Unknown";

        if (open()) {
            int result = nativeDeviceDescriptor();
            if (result == LibUsb.SUCCESS) {
                _manufacter = new String(nativeManufacter(), Charset.forName("US-ASCII"));
                _product = new String(nativeProduct(), Charset.forName("US-ASCII"));
                _serial = new String(nativeSerial(), Charset.forName("US-ASCII"));
            }
            close();
        }

        manufacter = _manufacter;
        product = _product;
        serial = _serial;

        if (Log.isDebug()) Log.d(TAG, "created device: " + manufacter + " - " + product + " - " + serial);
    }

    public boolean open() {
        if (Log.isDebug()) Log.d(TAG, "open");
        if (usbConnection_ != null){
            if (Log.isWarn()) Log.w(TAG, "usbConnection already opened");
            return false;
        }

        usbConnection_ = usbManager_.openDevice(usbDevice_);
        if (usbConnection_ != null){
            if (Log.isDebug()) Log.d(TAG, "create native connection");
            int code = nativeOpen(usbConnection_.getFileDescriptor());
            if (Log.isVerbose()) Log.v(TAG, "connection create with code " + code);

            if ( code == LibUsb.SUCCESS ){
                return true;
            } else {
                usbConnection_.close();
                usbConnection_ = null;
                return false;
            }
        }

        return false;
    }

    public void close(){
        if (Log.isDebug()) Log.d(TAG, "closing connection");
        nativeClose();
        if (Log.isVerbose()) Log.v(TAG, "closed native");
        if (usbConnection_ != null) {
            usbConnection_.close();
            usbConnection_ = null;
            if (Log.isVerbose()) Log.v(TAG, "closed android connection");
        }
    }

    public UsbDevice originalDevice(){
        return usbDevice_;
    }

    static {
        nativeInit();
    }

    // Native context
    @Keep
    private long handle_;
}
