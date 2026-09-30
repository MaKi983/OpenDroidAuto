package it.smg.hu.ui;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.hu.manager.ConnectionManager;
import it.smg.hu.projection.InputDevice;
import it.smg.hu.service.ODAService;
import it.smg.hu.ui.notification.AppBadge;
import it.smg.hu.ui.notification.NotificationFactory;
import it.smg.libs.common.Log;

public class PlayerActivity extends Activity implements ServiceConnection, SurfaceHolder.Callback, InputDevice.OnKeyHolder {

    private final static String TAG = "PlayerActivity";
    private ODAService odaService_;

    private LocalBroadcastManager localBroadcastManager_;
    private BroadcastReceiver localReceiver_;

    private SurfaceView surfaceView_;
    private View.OnKeyListener keyListener_;

    private String startMode_;
    private boolean isRunning_ = false;
    private boolean isServiceBound_ = false;

    private static boolean isActive_;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Log.isDebug()) Log.d(TAG, "onCreate");

        localBroadcastManager_ = LocalBroadcastManager.getInstance(this);

        if (savedInstanceState != null){
            startMode_ = savedInstanceState.getString("startMode");
            isRunning_ = savedInstanceState.getBoolean("isRunning");
        } else {
            Bundle b = getIntent().getExtras();
            startMode_ = b == null ? null : b.getString("mode");
        }

        if (Log.isDebug()) Log.d(TAG, "start mode: " + startMode_);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.activity_player);

        HondaConnectManager hondaManager = hondaManager();
        if (hondaManager != null){
            hondaManager.initialize();
        }

        surfaceView_ = findViewById(R.id.surfaceView);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        if (startMode_ == null) {
            if (Log.isDebug()) Log.d(TAG, "startMode_");
        }
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        if (Log.isDebug()) Log.d(TAG, "onSaveInstanceState");
        super.onSaveInstanceState(outState);
        outState.putSerializable("startMode", startMode_);
        outState.putBoolean("isRunning", isRunning_);
    }

    @Override
    public void onBackPressed() {
        if (Log.isDebug()) Log.d(TAG, "onBackPressed");
        exitSession();
    }

    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (Log.isInfo()) Log.i(TAG, "Back button long pressed");

            exitSession();
            return true;
        }
        return super.onKeyLongPress(keyCode, event);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (Log.isDebug()) Log.d(TAG, "onResume");

        Intent odaServiceIntent = new Intent(this, ODAService.class);
        startService(odaServiceIntent);
        // Track the binding from the moment it is accepted, not from
        // onServiceConnected: an onPause arriving before the connection lands
        // would otherwise skip unbindService and leak this ServiceConnection,
        // and the next onResume would bind the same object a second time.
        if (bindService(odaServiceIntent, this, BIND_AUTO_CREATE | BIND_ABOVE_CLIENT | BIND_IMPORTANT)) {
            isServiceBound_ = true;
        }

        HondaConnectManager hondaManager = hondaManager();
        if (hondaManager != null){
            hondaManager.initAudioBinding();
        }

        if (NotificationFactory.instance() != null) {
            NotificationFactory.instance().dismissAll();
        }
        if (AppBadge.instance() != null) {
            AppBadge.instance().dismiss();
        }

        initReceivers();
        isActive_ = true;
    }

    @Override
    protected void onPause() {
        if (Log.isDebug()) Log.d(TAG, "onPause");
        super.onPause();
        if (odaService_ != null) {
            odaService_.releaseFocus();
        }

        if (isServiceBound_) {
            isServiceBound_ = false;
            try {
                unbindService(this);
            } catch (IllegalArgumentException e) {
                Log.e(TAG, "service was not bound", e);
            }
        }

        HondaConnectManager hondaManager = hondaManager();
        if (hondaManager != null){
            hondaManager.sendToBackground();
        }

        // The projection exit must always return to the dashboard cleanly.  A
        // floating badge here can reopen a half-torn-down PlayerActivity and
        // create a second Android Auto session.
        if (AppBadge.instance() != null) {
            AppBadge.instance().dismiss();
        }

        if (localReceiver_ != null) {
            localBroadcastManager_.unregisterReceiver(localReceiver_);
            localReceiver_ = null;
        }
        isActive_ = false;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (Log.isDebug()) Log.d(TAG, "onKeyDown: " + keyCode);

        if (event.getKeyCode() != KeyEvent.KEYCODE_BACK && keyListener_ != null){
            return keyListener_.onKey(surfaceView_, keyCode, event);
        }

        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (Log.isDebug()) Log.d(TAG, "onKeyUp: " + keyCode);

        if (event.getKeyCode() != KeyEvent.KEYCODE_BACK && keyListener_ != null){
            return keyListener_.onKey(surfaceView_, keyCode, event);
        }

        return super.onKeyUp(keyCode, event);
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (Log.isDebug()) Log.d(TAG, "onStop");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (Log.isDebug()) Log.d(TAG, "onDestroy");
    }

    private void initReceivers(){
        if (Log.isDebug()) Log.d(TAG, "Registering LocalReceiver");
        IntentFilter localFilter = new IntentFilter();
        localFilter.addAction(ODAService.STOP_ACTION);
        localFilter.addAction(ODAService.STOP_VIDEO_INDICATION);
        localReceiver_ = new LocalReceiver();
        localBroadcastManager_.registerReceiver(localReceiver_, localFilter);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {}

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {}

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d(TAG, "surfaceDestroyed");
        if (odaService_ != null) {
            odaService_.releaseFocus();
        }
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        if (Log.isDebug()) Log.d(TAG, "onServiceConnected ODAService");
        ODAService.ServiceBinder binder = (ODAService.ServiceBinder) service;
        odaService_ = binder.getService();

        isServiceBound_ = true;

        if (isRunning_){
            odaService_.gainFocus();
        } else {
            start();
        }
    }

    /**
     * The Honda glue, or null when it is switched off or was never initialised.
     * HondaConnectManager.init() only runs at boot when the setting was already
     * enabled, so turning it on later leaves instance() null.
     */
    private HondaConnectManager hondaManager(){
        if (!Settings.instance().advanced.hondaIntegrationEnabled()){
            return null;
        }
        return HondaConnectManager.instance();
    }

    private void start(){

        HondaConnectManager hondaManager = hondaManager();
        if (hondaManager != null){
            hondaManager.adjustPermission();
        }

        if (startMode_ == null) {
            ConnectionManager.instance().failed(getString(R.string.connection_mode_missing));
            finish();
            return;
        }

        switch (startMode_){
            case ODAService.MODE_USB:
                odaService_.startUsb(surfaceView_, this);
                break;
            case ODAService.MODE_WIFI:
                odaService_.startWifi(surfaceView_, this);
                break;
            default:
                if (Log.isWarn()) Log.w(TAG, "unknown startMode: " + startMode_);
                return;
        }

        isRunning_ = true;
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        if (Log.isDebug()) Log.d(TAG, "onServiceDisconnected");
        odaService_ = null;
        // isServiceBound_ stays true on purpose: the service process died but the
        // binding is still registered, so onPause must still unbind it. Clearing
        // the flag here leaked the ServiceConnection on every service crash.
    }

    @Override
    public void setOnKeyListener(View.OnKeyListener listener) {
        keyListener_ = listener;
    }

    private class LocalReceiver extends BroadcastReceiver {
        private final static String TAG = "LocalReceiver";

        @Override
        public void onReceive(Context context, Intent intent) {
            if (Log.isDebug()) Log.d(TAG, "received action " + intent.getAction());
            if (ODAService.STOP_ACTION.equalsIgnoreCase(intent.getAction())){
                finish();

            } else if (ODAService.STOP_VIDEO_INDICATION.equalsIgnoreCase(intent.getAction())){
                moveTaskToBack(true);
            }
        }
    }

    public static boolean isActive(){
        return isActive_;
    }

    private void exitSession() {
        if (AppBadge.instance() != null) {
            AppBadge.instance().dismiss();
        }
        if (odaService_ != null) {
            ConnectionManager.instance().disconnecting(getString(R.string.connection_user_stopping));
            odaService_.shutdown();
            finish();
        } else {
            ConnectionManager.instance().userExited(getString(R.string.connection_user_stopped));
            finish();
        }
    }
}
