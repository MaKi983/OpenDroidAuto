package it.smg.hu.ui;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.os.Bundle;

import androidx.fragment.app.FragmentActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import it.smg.hu.R;
import it.smg.hu.manager.ConnectionManager;
import it.smg.hu.manager.ConnectionState;
import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
import it.smg.hu.service.ODAService;
import it.smg.hu.ui.main.HomeFragment;
import it.smg.hu.ui.notification.AppBadge;
import it.smg.libs.common.Log;

/** Connection host. It launches the player only after a transport is ready. */
public class MainActivity extends FragmentActivity {
    private final static String TAG = "MainActivity";
    public static final int SETTINGS_ACTIVITY_REQUEST = 12345;

    private USBManager usbManager_;
    private WIFIManager wifiManager_;
    private LocalBroadcastManager localBroadcastManager_;
    private boolean receiversRegistered_;
    private boolean playerLaunchPending_;
    private static boolean isActive_;

    private final BroadcastReceiver wifiReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (wifiManager_ != null) {
                wifiManager_.checkNetwork();
            }
        }
    };

    private final BroadcastReceiver connectionReceiver = new BroadcastReceiver() {
        final String TAG = "MainActivity-connectionReceiver";
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (Log.isDebug()) Log.d(TAG, action);
            if (ConnectionManager.ACTION_STATE_CHANGED.equals(action)) {
                String state = intent.getStringExtra(ConnectionManager.EXTRA_STATE);
                if (ConnectionState.ERROR.name().equals(state) || ConnectionState.IDLE.name().equals(state)
                        || ConnectionState.EXITED.name().equals(state)) {
                    playerLaunchPending_ = false;
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Log.isDebug()) Log.d(TAG, "onCreate, intent action: " + getIntent().getAction());

        usbManager_ = USBManager.instance();
        wifiManager_ = WIFIManager.instance();
        localBroadcastManager_ = LocalBroadcastManager.getInstance(this);

        setContentView(R.layout.activity_main);
        getSupportFragmentManager().beginTransaction()
                .replace(R.id.home_content, new HomeFragment(), "HomeFragment")
                .commit();
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        if (Log.isDebug()) Log.d(TAG, "onNewIntent, action " + intent.getAction());
    }

    @Override
    protected void onResume() {
        if (Log.isDebug()) Log.d(TAG, "onResume");
        super.onResume();
        playerLaunchPending_ = PlayerActivity.isActive();
        registerConnectionReceivers();

        if (AppBadge.instance() != null){
            AppBadge.instance().dismiss();
        }

        if (usbManager_ != null) {
            usbManager_.rescanAttachedDevices();
        }

        if (wifiManager_ != null) {
            wifiManager_.checkNetwork();
        }

        isActive_ = true;
    }

    @Override
    protected void onPause() {
        if (Log.isDebug()) Log.d(TAG, "onPause");
        unregisterConnectionReceivers();
        super.onPause();
        isActive_ = false;
    }

    private void registerConnectionReceivers() {
        if (receiversRegistered_) {
            return;
        }
        registerReceiver(wifiReceiver, new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION));
        IntentFilter localFilter = new IntentFilter();
        localFilter.addAction(WIFIManager.CONNECT_WIFI);
        localFilter.addAction(ConnectionManager.ACTION_STATE_CHANGED);
        localBroadcastManager_.registerReceiver(connectionReceiver, localFilter);
        receiversRegistered_ = true;
    }

    private void unregisterConnectionReceivers() {
        if (!receiversRegistered_) {
            return;
        }
        unregisterReceiver(wifiReceiver);
        localBroadcastManager_.unregisterReceiver(connectionReceiver);
        receiversRegistered_ = false;
    }

    public void startConnectionManually(String mode) {
        if (mode == null || playerLaunchPending_ || PlayerActivity.isActive()) {
            return;
        }
        if (!ConnectionManager.instance().isManualStartAllowed()) {
            return;
        }
        if (ODAService.MODE_WIFI.equals(mode) && usbManager_ != null && usbManager_.aoapDevice() != null) {
            return;
        }
        playerLaunchPending_ = true;
        Intent player = new Intent(this, PlayerActivity.class);
        player.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        player.putExtra("mode", mode);
        startActivity(player);
    }

    public static boolean isActive(){
        return isActive_;
    }

    /** Called by the home screen and PlayerActivity to prevent a reconnect loop. */
    public void exitSession() {
        playerLaunchPending_ = false;
        ConnectionManager.instance().userExited(getString(R.string.connection_user_stopped));
        stopService(new Intent(this, ODAService.class));
    }

    /** Closes the task and terminates the process after service cleanup has run. */
    public void exitApplication() {
        exitSession();
        finish();

        Thread terminator = new Thread(() -> {
            try {
                Thread.sleep(750L);
            } catch (InterruptedException ignored) {
                Thread.currentThread().interrupt();
            }
            android.os.Process.killProcess(android.os.Process.myPid());
            System.exit(0);
        }, "ODA-app-exit");
        terminator.start();
    }

    @Override
    public void onBackPressed() {
        exitSession();
        exitApplication();
    }

}
