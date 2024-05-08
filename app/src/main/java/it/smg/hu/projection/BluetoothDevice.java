package it.smg.hu.projection;

import android.bluetooth.BluetoothAdapter;

import java.util.Set;

import it.smg.libs.common.Log;

public class BluetoothDevice extends it.smg.libs.aasdk.projection.BluetoothDevice {

    private static final String TAG = "BluetoothDevice";

    private final BluetoothAdapter bluetoothAdapter_;

    public BluetoothDevice(){
        bluetoothAdapter_ = BluetoothAdapter.getDefaultAdapter();
    }

    @Override
    public void stop() {
        if (Log.isDebug()) Log.d(TAG, "stop (do nothing)");
    }

    @Override
    public boolean isPaired(String address) {
        if (Log.isDebug()) Log.d(TAG, "isPaired phoneAddress: " + address);

        if (address != null && !address.trim().isEmpty()) {
            Set<android.bluetooth.BluetoothDevice> pairedDevices = bluetoothAdapter_.getBondedDevices();
            // If there are paired devices
            if (pairedDevices.size() > 0) {
                // Loop through paired devices
                for (android.bluetooth.BluetoothDevice device : pairedDevices) {
                    // Add the name and address to an array adapter to show in a ListView
                    if (Log.isDebug()) Log.d(TAG, "check paired device " + device.getName() + ": " + device.getAddress());
                    if (address.equalsIgnoreCase(device.getAddress())) {
                        if (Log.isDebug()) Log.d(TAG, "found device " + device.getName());
                        return true;
                    }
                }
            }
        }

        if (Log.isDebug()) Log.d(TAG, "phoneAddress null or not paired");
        return false;
    }

    @Override
    public boolean pair(String address) {
        return true;
    }

    @Override
    public String getLocalAddress() {
        String macAddress = "";
        if (isAvailable()){
            if (android.os.Build.VERSION.SDK_INT <= android.os.Build.VERSION_CODES.JELLY_BEAN) {
                macAddress = bluetoothAdapter_.getAddress();
                if (macAddress == null){
                    macAddress = "";
                }
            }
        }
        if (Log.isDebug()) Log.d(TAG, "localAddress: " + macAddress);
        return macAddress;
    }

    @Override
    public boolean isAvailable() {
        if (Log.isDebug()) Log.d(TAG, "isAvailable");

        if (bluetoothAdapter_ == null) {
            if (Log.isWarn()) Log.w(TAG, "Device not available");
            return false;
        } else if (!bluetoothAdapter_.isEnabled()) {
            if (Log.isWarn()) Log.w(TAG, "Device not enabled");
            return false;
        } else {
            if (Log.isDebug()) Log.d(TAG, "Device enabled");
            return true;
        }
    }
}
