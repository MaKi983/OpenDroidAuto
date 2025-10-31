package it.smg.hu.ui.main;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.ObjectAnimator;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.LinearInterpolator;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.fragment.app.Fragment;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
import it.smg.hu.service.ODAService;
import it.smg.hu.ui.MainActivity;
import it.smg.hu.ui.PlayerActivity;
import it.smg.hu.ui.SettingsActivity;
import it.smg.libs.aasdk.usb.LibUsbDevice;
import it.smg.libs.common.Log;

public class HomeFragment extends Fragment {

    private static final String TAG = "HomeFragment";

    public static final String AUTOSTART = "enableAutostart";

    private ImageView usbIconConnection_;
    private TextView txtUsbIconConnection_;
    private ImageView wifiIconConnection_;
    private TextView txtWifiIconConnection_;
    private Button startBtn_;
    private Button startUsbBtn_;

    private LinearLayout logoContainer;
    private LinearLayout progressContainer;
    private ObjectAnimator progressAnimator;
    private ProgressBar progressBar;
    private boolean progressCancelled = false;

    private BroadcastReceiver localReceiver_;
    private LocalBroadcastManager localBroadcastManager_;

    private USBManager usbManager_;
    private WIFIManager WIFIManager_;

    private Settings settings_;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        if (Log.isDebug()) Log.d(TAG, "onCreateView");

        super.onCreateView(inflater, container, savedInstanceState);
        View view = inflater.inflate(R.layout.home_fragment, container, false);

        settings_ = Settings.instance();
        localBroadcastManager_ = LocalBroadcastManager.getInstance(getContext());

        usbManager_ = USBManager.instance();
        WIFIManager_ = WIFIManager.instance();

        if (usbManager_.aoapDevice() == null){
            usbManager_.searchForAoapDevice();
        }

        initUI(view);

        return view;
    }

    private void initUI(View view){
        usbIconConnection_ = view.findViewById(R.id.usbIcon);
        usbIconConnection_.setImageResource(R.drawable.usb_dis);
        txtUsbIconConnection_ = view.findViewById(R.id.txtUsbIcon);
        txtUsbIconConnection_.setText("");

        Button settingsBtn = view.findViewById(R.id.settingsBtn);

        wifiIconConnection_ = view.findViewById(R.id.wifiIcon);
        wifiIconConnection_.setImageResource(R.drawable.wifi_dis);
        txtWifiIconConnection_ = view.findViewById(R.id.txtWifiIcon);
        txtWifiIconConnection_.setText("");
        startBtn_ = view.findViewById(R.id.startBtn);
        startBtn_.setEnabled(false);
        startBtn_.setAlpha(0.5f);

        if (settings_.advanced.enableWiFi()) {
            startBtn_.setVisibility(View.VISIBLE);
            wifiIconConnection_.setVisibility(View.VISIBLE);
            txtWifiIconConnection_.setVisibility(View.VISIBLE);
        } else {
            startBtn_.setVisibility(View.GONE);
            wifiIconConnection_.setVisibility(View.GONE);
            txtWifiIconConnection_.setVisibility(View.GONE);
        }

        startUsbBtn_ = view.findViewById(R.id.startUsbBtn);
        startUsbBtn_.setEnabled(false);
        startUsbBtn_.setAlpha(0.5f);
        Button exit = view.findViewById(R.id.exitBtn);

        settingsBtn.setOnClickListener(c -> {
            Intent i = new Intent(getContext(), SettingsActivity.class);
            startActivityForResult(i, MainActivity.SETTINGS_ACTIVITY_REQUEST);
        });

        startBtn_.setOnClickListener(c -> {
            if (Log.isInfo()) Log.i(TAG, "Starting in WiFi mode");
            start(ODAService.MODE_WIFI);
        });

        startUsbBtn_.setOnClickListener(c -> {
            if (Log.isInfo()) Log.i(TAG, "Starting in USB mode");
            start(ODAService.MODE_USB);
        });

        exit.setOnClickListener(c -> {
            // TODO: fix
            ((MainActivity) getActivity()).exitApp();
        });

        usbIconConnection_.setOnClickListener(v -> {
            AlertDialog dialog = new DeviceListDialog(getActivity());
            dialog.show();
        });

        logoContainer = view.findViewById(R.id.logoContainer);
        progressContainer = view.findViewById(R.id.progressContainer);
        progressBar = view.findViewById(R.id.progressbar);

        initUsbDeviceIcon();
    }

    private void initUsbDeviceIcon(){
        if (usbManager_.aoapDevice() != null){
            LibUsbDevice device = usbManager_.aoapDevice();
            if (Log.isDebug()) Log.d(TAG, "current aoap device " + device);
            usbIconConnection_.setImageResource(R.drawable.usb);
            txtUsbIconConnection_.setText(device.product);
            startUsbBtn_.setEnabled(true);
            startUsbBtn_.setAlpha(1f);

            autoStartAnimation();
        }
    }

    public void start(String mode){
        Intent i = new Intent(getContext(), PlayerActivity.class);
        i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        i.putExtra("mode", mode);
        startActivity(i);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (Log.isDebug()) Log.d(TAG, "onActivityResult " + resultCode);
        if (requestCode == MainActivity.SETTINGS_ACTIVITY_REQUEST){

            if (settings_.advanced.enableWiFi()) {
                startBtn_.setVisibility(View.VISIBLE);
                wifiIconConnection_.setVisibility(View.VISIBLE);
                txtWifiIconConnection_.setVisibility(View.VISIBLE);
            } else {
                startBtn_.setVisibility(View.GONE);
                wifiIconConnection_.setVisibility(View.GONE);
                txtWifiIconConnection_.setVisibility(View.GONE);
            }

            initUsbDeviceIcon();
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onStart() {
        super.onStart();
        if (Log.isDebug()) Log.d(TAG, "onStart");

        if (Log.isDebug()) Log.d(TAG, "Registering LocalReceiver");
        IntentFilter localFilter = new IntentFilter();
        localFilter.addAction(USBManager.ATTACH_AOAP_DEVICE);
        localFilter.addAction(USBManager.DETACH_AOAP_DEVICE);
        localFilter.addAction(WIFIManager.CONNECT_WIFI);
        localFilter.addAction(WIFIManager.DISCONNECT_WIFI);
        localReceiver_ = new LocalReceiver();
        LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(getActivity());
        localBroadcastManager.registerReceiver(localReceiver_, localFilter);
    }

    @Override
    public void onStop() {
        super.onStop();
        if (Log.isDebug()) Log.d(TAG, "onStop");
        LocalBroadcastManager.getInstance(getActivity()).unregisterReceiver(localReceiver_);

        cancelAutoStartAnimation();
    }

    private void autoStartAnimation() {
        Log.d(TAG, "autoStartAnimation: " + settings_.car.autoStartTimer());
        if (settings_.car.autoStartTimer() <= 0) return;

        logoContainer.setVisibility(View.GONE);
        progressContainer.setVisibility(View.VISIBLE);

        // Animate progress over 3 seconds
        progressAnimator = ObjectAnimator.ofInt(progressBar, "progress", 0, 100);
        progressAnimator.setDuration(settings_.car.autoStartTimer() * 1000L);
        progressAnimator.setInterpolator(new LinearInterpolator());
        progressAnimator.addListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                Log.d(TAG, "progressAnimator end: " + progressCancelled);
                progressContainer.setVisibility(View.GONE);
                if (!progressCancelled) {
                    start(ODAService.MODE_USB);
                }
            }
        });

        progressAnimator.start();
    }

    private void cancelAutoStartAnimation() {
        if (progressAnimator != null && progressAnimator.isRunning()) {
            progressCancelled = true;
            progressAnimator.cancel();
        }

        logoContainer.setVisibility(View.VISIBLE);
        progressContainer.setVisibility(View.GONE);
    }

    private class LocalReceiver extends BroadcastReceiver {
        private final static String TAG = "HomeFragment-LocalReceiver";

        @Override
        public void onReceive(Context context, Intent intent) {
            if (Log.isDebug()) Log.d(TAG, "received action " + intent.getAction());
            if (USBManager.DETACH_AOAP_DEVICE.equalsIgnoreCase(intent.getAction())){
                usbIconConnection_.setImageResource(R.drawable.usb_dis);
                txtUsbIconConnection_.setText("");
                startUsbBtn_.setEnabled(false);
                startUsbBtn_.setAlpha(0.5f);

                cancelAutoStartAnimation();
            } else if (USBManager.ATTACH_AOAP_DEVICE.equalsIgnoreCase(intent.getAction())){
                LibUsbDevice device = usbManager_.aoapDevice();
                usbIconConnection_.setImageResource(R.drawable.usb);
                txtUsbIconConnection_.setText(device.product);
                startUsbBtn_.setEnabled(true);
                startUsbBtn_.setAlpha(1f);

                autoStartAnimation();
            } else if (WIFIManager.CONNECT_WIFI.equalsIgnoreCase(intent.getAction())){
                String ssid = intent.getStringExtra(WIFIManager.EXTRA_SSID);
                wifiIconConnection_.setImageResource(R.drawable.wifi);
                txtWifiIconConnection_.setText(ssid);
                startBtn_.setEnabled(true);
                startBtn_.setAlpha(1f);
            } else if (WIFIManager.DISCONNECT_WIFI.equalsIgnoreCase(intent.getAction())){
                wifiIconConnection_.setImageResource(R.drawable.wifi_dis);
                txtWifiIconConnection_.setText("");
                startBtn_.setEnabled(false);
                startBtn_.setAlpha(0.5f);
            }
        }
    }

}