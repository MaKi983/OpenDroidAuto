package it.smg.libs.aasdk.service;

import androidx.annotation.Keep;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IBluetoothDevice;

public class BluetoothService implements IService {

    private static final String TAG = "BluetoothService";

    private IBluetoothDevice bluetoothDevice_;

    public BluetoothService(Messenger messenger, IBluetoothDevice bluetoothDevice) {
        handle_ = nativeSetup(messenger, bluetoothDevice);
        bluetoothDevice_ = bluetoothDevice;
    }

    @Override
    public void start() {
        nativeStart();
    }

    @Override
    public void stop() {
        nativeStop();
    }

    @Override
    public void delete() {
        nativeDelete();
        bluetoothDevice_.delete();
        bluetoothDevice_ = null;
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, IBluetoothDevice bluetoothDevice);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();

    @Keep
    protected long handle_;
}
