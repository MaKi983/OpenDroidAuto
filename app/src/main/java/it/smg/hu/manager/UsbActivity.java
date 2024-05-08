package it.smg.hu.manager;

import android.app.Activity;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;

import androidx.annotation.Nullable;

import it.smg.libs.common.Log;

public class UsbActivity extends Activity {

    private static final String TAG = "UsbActivity";

    private Intent newUsbDeviceIntent = null;

    private USBManager usbManager_;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        usbManager_ = USBManager.instance();

        Intent intent = getIntent();
        if (Log.isInfo()) Log.i(TAG, "intent action: " + intent.getAction());
        if ( UsbManager.ACTION_USB_DEVICE_ATTACHED.equalsIgnoreCase(intent.getAction())) {
            if (Log.isInfo()) Log.i(TAG, "new usb device intent: " + intent);
            newUsbDeviceIntent = intent;
        }
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        if (Log.isDebug()) Log.d(TAG, "onNewIntent, action " + intent.getAction());
        if ( UsbManager.ACTION_USB_DEVICE_ATTACHED.equalsIgnoreCase(intent.getAction())) {
            if (Log.isInfo()) Log.i(TAG, "attached new device, keep intent");
            newUsbDeviceIntent = intent;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (Log.isDebug()) Log.d(TAG, "onResume");

        Intent intent = newUsbDeviceIntent; //getIntent();
        if (Log.isDebug()) Log.d(TAG, "onResume intent: " + intent);
        if (intent != null) {
            if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equalsIgnoreCase(intent.getAction())) {
                UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                if (Log.isDebug()) Log.d(TAG, "attached device " + device);
                if (device != null) {
                    usbManager_.checkDevice(device);
                }
            } else if (USBManager.ACTION_USB_PERMISSION.equalsIgnoreCase(intent.getAction())) {
                UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                    if (Log.isDebug()) Log.d(TAG, "permission granted for device " + device);
                    if (device != null) {
                        usbManager_.checkDevice(device);
                    }
                } else {
                    if (Log.isWarn()) Log.w(TAG, "permission negated for device " + device + " do nothing");
                }
            }
        }
        newUsbDeviceIntent = null;
        finish();
    }

}
