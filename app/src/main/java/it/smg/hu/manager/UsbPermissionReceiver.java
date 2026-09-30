package it.smg.hu.manager;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

/** Receives permission and detach broadcasts even while MainActivity is not visible. */
public class UsbPermissionReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        USBManager manager = USBManager.instance();
        if (manager == null || intent == null) {
            return;
        }
        String action = intent.getAction();
        if (USBManager.ACTION_USB_PERMISSION.equals(action)) {
            UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
            boolean granted = intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false);
            manager.onUsbPermissionResult(device, granted);
        } else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
            manager.onUsbDetached(intent.getParcelableExtra(UsbManager.EXTRA_DEVICE));
        }
    }
}
