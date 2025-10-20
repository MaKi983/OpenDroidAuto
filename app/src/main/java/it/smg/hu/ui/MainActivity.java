package it.smg.hu.ui;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.os.Handler;
import android.widget.Toast;

import androidx.fragment.app.FragmentActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import it.smg.hu.config.Settings;
import it.smg.hu.service.ODAService;
import it.smg.hu.R;
import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
import it.smg.hu.ui.main.HomeFragment;
import it.smg.hu.ui.notification.AppBadge;
import it.smg.hu.ui.notification.NotificationFactory;
import it.smg.libs.common.Log;

public class MainActivity extends FragmentActivity {

    public static final int SETTINGS_ACTIVITY_REQUEST = 12345;

    private final static String TAG = "MainActivity";

    private USBManager usbManager_;
    private WIFIManager wifiManager_;
    private LocalBroadcastManager localBroadcastManager_;

    private Intent newUsbDeviceIntent_ = null;
    private boolean isOpenFromMain_ = false;
    private boolean isOpenFromUsb_ = false;
    private boolean isServiceBound_ = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Log.isDebug()) Log.d(TAG, "onCreate, intent: " + getIntent());
        if (Log.isDebug()) Log.d(TAG, "onCreate, intent action: " + getIntent().getAction());

        isOpenFromMain_ = false;
        isOpenFromUsb_ = false;

        localBroadcastManager_ = LocalBroadcastManager.getInstance(this);
        usbManager_ = USBManager.instance();
        wifiManager_ = WIFIManager.instance();

        setContentView(R.layout.activity_main);

        Intent intent = getIntent();
        if (Log.isInfo()) Log.i(TAG, "intent action: " + intent.getAction());

        if ( UsbManager.ACTION_USB_DEVICE_ATTACHED.equalsIgnoreCase(intent.getAction())) {
            if (Log.isInfo()) Log.i(TAG, "new usb device intent: " + intent);
            newUsbDeviceIntent_ = intent;
            getWindow().setLayout(0, 0);
            isOpenFromMain_ = false;
            isOpenFromUsb_ = true;
        } else if (Intent.ACTION_MAIN.equalsIgnoreCase(intent.getAction())) {
            HomeFragment f = new HomeFragment();
            Bundle args = new Bundle();
            args.putBoolean(HomeFragment.AUTOSTART, true);
            f.setArguments(args);

            getSupportFragmentManager()
                    .beginTransaction()
                    .replace(R.id.home_content, f)
                    .commit();

            isOpenFromMain_ = true;
            isOpenFromUsb_ = false;
        }

    }

    private void registerReceiver(){
        IntentFilter wifiFilter = new IntentFilter();
        wifiFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(wifiReceiver, wifiFilter);

        IntentFilter filter = new IntentFilter();
        filter.addAction(USBManager.ATTACH_AOAP_DEVICE);
        localBroadcastManager_.registerReceiver(usbReceiver, filter);
    }

    private final BroadcastReceiver wifiReceiver = new BroadcastReceiver() {
        private final static String TAG = "MainActivity-WifiReceiver";
        @Override
        public void onReceive(Context context, Intent intent) {
            if (Log.isDebug()) Log.d(TAG, "Received action: " + intent.getAction());
            if (wifiManager_ != null) {
                wifiManager_.checkNetwork();
            }
        }
    };

    private final BroadcastReceiver usbReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String TAG = "MainActivity-UsbReceiver";

            if (Log.isDebug()) Log.d(TAG, intent.getAction());

            if (Settings.instance().video.disableStartUsbBadge()) {
                if (Log.isDebug()) Log.d(TAG, "stopping service because usb startup is disabled");
                exitApp();
                return;
            }

            if (isOpenFromUsb_) {
                if (intent.getAction().equals(USBManager.ATTACH_AOAP_DEVICE)) {
                    if (Log.isDebug()) Log.d(TAG, "open popup activity");
                    NotificationFactory.instance().notifyStartRequest();

//                    MainActivity.this.finish();
                }
            }
        }
    };

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

        Intent odaServiceIntent = new Intent(this, ODAService.class);
        startService(odaServiceIntent);

        registerReceiver();

        Intent intent = newUsbDeviceIntent_;
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

//        NotificationFactory.init(getApplicationContext());
//        NotificationFactory.instance().notifyStartRequest();
        AppBadge.instance().dismiss();

        newUsbDeviceIntent_ = null;
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(wifiReceiver);

        localBroadcastManager_.unregisterReceiver(usbReceiver);
        if (Log.isDebug()) Log.d(TAG, "onPause");
    }

    @Override
    protected void onStart() {
        if (Log.isDebug()) Log.d(TAG, "onStart");
        super.onStart();
    }

    @Override
    protected void onStop() {
        if (Log.isDebug()) Log.d(TAG, "onStop");
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (Log.isDebug()) Log.d(TAG, "onDestroy");
    }

    private boolean doubleBackToExitPressedOnce = false;

    public void exitApp(){
        Intent service = new Intent(this, ODAService.class);
        stopService(service);

        Log.shutdown();

        finish();
//        android.os.Process.killProcess(android.os.Process.myPid());
        return;
    }

    @Override
    public void onBackPressed() {
        if (doubleBackToExitPressedOnce) {
            super.onBackPressed();
            exitApp();
            return;
        }

        this.doubleBackToExitPressedOnce = true;
        Toast.makeText(this, "Please click BACK again to exit", Toast.LENGTH_SHORT).show();

        new Handler().postDelayed(() -> doubleBackToExitPressedOnce=false, 2000);
    }
}