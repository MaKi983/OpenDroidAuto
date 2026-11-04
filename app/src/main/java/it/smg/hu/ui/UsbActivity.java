package it.smg.hu.ui;

import android.app.Activity;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;

import androidx.annotation.Nullable;

import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.UsbReceiver;
import it.smg.libs.common.Log;

public class UsbActivity extends Activity {

    private static final String TAG = "UsbActivity";

    private Intent newUsbDeviceIntent_ = null;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Log.isDebug()) Log.d(TAG, "onCreate");

        Intent intent = getIntent();
        if (Log.isInfo()) Log.i(TAG, "intent action: " + intent.getAction());
        if ( UsbManager.ACTION_USB_DEVICE_ATTACHED.equalsIgnoreCase(intent.getAction())) {
            if (Log.isInfo()) Log.i(TAG, "new usb device intent: " + intent);
            newUsbDeviceIntent_ = intent;
        }

        getWindow().setLayout(0, 0);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        if (Log.isDebug()) Log.d(TAG, "onNewIntent, action " + intent.getAction());
        if ( UsbManager.ACTION_USB_DEVICE_ATTACHED.equalsIgnoreCase(intent.getAction())) {
            if (Log.isInfo()) Log.i(TAG, "attached new device, keep intent");
            newUsbDeviceIntent_ = intent;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (Log.isDebug()) Log.d(TAG, "onResume");

        Intent intent = newUsbDeviceIntent_; //getIntent();
        if (Log.isDebug()) Log.d(TAG, "onResume intent: " + intent);
        if (intent != null){
            UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
            if (Log.isDebug()) Log.d(TAG, "attached device " + device);

            Intent newUsbDevice = new Intent(this, UsbReceiver.class);
            newUsbDevice.setAction(USBManager.USB_DEVICE_ATTACHED);
            newUsbDevice.putExtra(UsbManager.EXTRA_DEVICE, device);

            newUsbDeviceIntent_ = null;
            sendBroadcast(newUsbDevice);
        }

        if (Log.isDebug()) Log.d(TAG, "finish activity");
        finish();
    }

    @Override
    protected void onPause() {
        if (Log.isDebug()) Log.d(TAG, "onPause");
        super.onPause();
    }
}
