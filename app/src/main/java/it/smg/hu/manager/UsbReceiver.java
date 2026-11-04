package it.smg.hu.manager;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

import it.smg.hu.ui.MainActivity;
import it.smg.hu.ui.notification.NotificationFactory;
import it.smg.libs.common.Log;

public class UsbReceiver extends BroadcastReceiver {

    private static final String TAG = "UsbReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        if (Log.isDebug()) Log.d(TAG, "onReceive action= " + intent.getAction());
        USBManager manager = USBManager.instance();

        if (manager == null || intent == null) {
            return;
        }

        String action = intent.getAction();
        if (USBManager.ACTION_USB_PERMISSION.equals(action)) {
            UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
            boolean granted = intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false);
            manager.onUsbPermissionResult(device, granted);
        } else if (USBManager.USB_DEVICE_ATTACHED.equals(action)) {
            UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
            if (Log.isDebug()) Log.d(TAG, "attached device " + device);
            manager.onUsbDeviceDetected(intent.getParcelableExtra(UsbManager.EXTRA_DEVICE));
        } else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
            manager.onUsbDetached(intent.getParcelableExtra(UsbManager.EXTRA_DEVICE));
        } else if (USBManager.ATTACH_AOAP_DEVICE.equals(action)) {
            if (!MainActivity.isActive()){
                if (Log.isDebug()) Log.d(TAG, "open popup activity");
                NotificationFactory.instance().notifyStartRequest();
            }
        }
    }
}
