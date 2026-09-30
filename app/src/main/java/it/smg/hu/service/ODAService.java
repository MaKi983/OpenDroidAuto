package it.smg.hu.service;

import android.app.Notification;
import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.view.SurfaceView;
import android.widget.Toast;

import java.util.concurrent.atomic.AtomicBoolean;

import androidx.annotation.Keep;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.hu.manager.ConnectionManager;
import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
import it.smg.hu.projection.InputDevice;
import it.smg.hu.ui.notification.NotificationFactory;
import it.smg.libs.aasdk.tcp.TCPConnectException;
import it.smg.libs.common.Log;
import it.smg.libs.aasdk.service.AndroidAutoEntity;
import it.smg.libs.aasdk.service.IAndroidAutoEntityEventHandler;
import it.smg.libs.aasdk.tcp.TCPEndpoint;
import it.smg.libs.aasdk.usb.LibUsbDevice;

public class ODAService extends Service implements IAndroidAutoEntityEventHandler {

    public static final String START_ACTION = "it.smg.hu.service.ODAService.START_ACTION";
    public static final String STOP_ACTION = "it.smg.hu.service.ODAService.STOP_ACTION";
    public static final String STOP_VIDEO_INDICATION = "it.smg.hu.service.ODAService.STOP_VIDEO_INDICATION";
    public static final String FORCE_CLOSE_ACTION = "it.smg.hu.service.ODAService.FORCE_CLOSE_ACTION";

    public static final String EXTRA_START_MODE = "startMode";
    public static final String MODE_USB = "modeUSB";
    public static final String MODE_WIFI = "modeWifi";

    private static final String TAG = "ODAService";
    private static final long SHUTDOWN_TIMEOUT_MS = 2000L;

    private final IBinder mBinder = new ServiceBinder();

    private LocalBroadcastManager localBroadcastManager_;
    private NotificationFactory notificationFactory_;
    /**
     * Written by the connection thread and read by the UI thread and by native
     * quit callbacks. It also gates whether a new session may start, so a stale
     * read here would either skip a teardown or lock connecting out entirely.
     */
    private volatile AndroidAutoEntity androidAutoEntity_;

    private  USBManager usbManager_;
    private  WIFIManager wifiManager_;

    private Thread startThread_;

    private Handler mainHandler_;

    private volatile boolean isRunning_;
    private volatile boolean stopRequested_;
    private volatile boolean shutdownRequested_;
    private String currentMode_;

    private final Runnable shutdownTimeout_ = new Runnable() {
        @Override
        public void run() {
            if (shutdownRequested_) {
                if (Log.isWarn()) Log.w(TAG, "graceful shutdown timed out; forcing teardown");
                ConnectionManager.instance().userExited(getString(R.string.connection_user_stopped));
                stop();
            }
        }
    };

    /**
     * Guards the session lifecycle. Tearing an AndroidAutoEntity down runs a
     * cascade of native destructors while io_service handlers are still in
     * flight; letting a new session start on top of that left two entities alive
     * at once, which showed up as SEGV in the asio strand and as a wedged main
     * thread. A flag rather than a lock on purpose: stop() is reached both from
     * the UI thread and from native callbacks, so blocking one on the other could
     * deadlock the very service that has to finish the teardown.
     */
    private final AtomicBoolean stopping_ = new AtomicBoolean(false);

    public ODAService() {}

    @Override
    public void onCreate() {
        if (Log.isInfo()) Log.i(TAG, "create");
        super.onCreate();

        NotificationFactory.init(getApplicationContext());
        notificationFactory_ = NotificationFactory.instance();

        usbManager_ = USBManager.instance();
        wifiManager_ = WIFIManager.instance();
        mainHandler_ = new Handler(Looper.getMainLooper());

        localBroadcastManager_ = LocalBroadcastManager.getInstance(this);
    }

    public void startUsb(SurfaceView surfaceView, InputDevice.OnKeyHolder keyHolder){
        if (!canStartSession()) {
            return;
        }
        currentMode_ = MODE_USB;
        stopRequested_ = false;
        ConnectionManager.instance().connecting(MODE_USB, getString(R.string.connection_usb_connecting));
        startThread_ = new Thread(() -> {
            Looper.prepare();

            if (usbManager_.aoapDevice() != null) {
                if (Log.isVerbose()) Log.v(TAG, "aoap device available, start in usb mode");
                try {
                    LibUsbDevice device = usbManager_.aoapDevice();
                    if (device.open()) {
                        if (Log.isInfo()) Log.i(TAG, "device opened");
                        androidAutoEntity_ = AndroidAutoEntityFactory.create(this, device, surfaceView, keyHolder);
                        if (stopRequested_) {
                            androidAutoEntity_.delete();
                            androidAutoEntity_ = null;
                            return;
                        }
                        androidAutoEntity_.start(this);
                        isRunning_ = true;
                        ConnectionManager.instance().active(getString(R.string.connection_usb_active));
                    } else {
                        Log.e(TAG, "Error in open usb device");
                        onAndroidAutoQuitOnError("USB OPEN DEVICE", -1);
                        return;
                    }
                } catch (Exception e){
                    Log.e(TAG, "error", e);
                    onAndroidAutoQuitOnError("USB GENERIC ERROR", -1);
//                    onAndroidAutoQuit();
                    return;
                }
            } else {
                ConnectionManager.instance().failed(getString(R.string.connection_usb_unavailable));
            }
            if (Log.isInfo()) Log.i(TAG, "start usb thead completed");
        });
        startThread_.start();
    }

    public void startWifi(SurfaceView surfaceView, InputDevice.OnKeyHolder keyHolder){
        if (!canStartSession()) {
            return;
        }
        currentMode_ = MODE_WIFI;
        stopRequested_ = false;
        ConnectionManager.instance().connecting(MODE_WIFI, getString(R.string.connection_wifi_connecting));
        startThread_ = new Thread(() -> {
            Looper.prepare();

            String ipAddress = wifiManager_.getIpAddress();
            try {
                if (ipAddress != null) {
//                    if (Log.isInfo()) Log.i(TAG, "Connect to ip " + ipAddress);
                    TCPEndpoint tcpEndpoint = new TCPEndpoint(ipAddress);
                    androidAutoEntity_ = AndroidAutoEntityFactory.create(this, tcpEndpoint, surfaceView, keyHolder);
                    if (stopRequested_) {
                        androidAutoEntity_.delete();
                        androidAutoEntity_ = null;
                        return;
                    }
                    androidAutoEntity_.start(this);
                    isRunning_ = true;
                    ConnectionManager.instance().active(getString(R.string.connection_wifi_active));
                } else {
                    ConnectionManager.instance().failed(getString(R.string.connection_wifi_gateway_error));
                }
                if (Log.isInfo()) Log.i(TAG, "start wifi thead completed");
            } catch (TCPConnectException e){
                Log.e(TAG, "TCP Connection error", e);
                // The phone refuses port 5277 unless Android Auto Wireless is
                // actually running on it. Say so on screen: the projection window
                // closes within a second either way, which otherwise looks
                // exactly like the app crashing.
                String reason = getString(R.string.connection_wifi_endpoint_error, ipAddress);
                ConnectionManager.instance().failed(reason);
                notifyUser(reason);
                stop();
            }
        });
        startThread_.start();
    }

    public void shutdown(){
        if (shutdownRequested_) {
            return;
        }
        shutdownRequested_ = true;
        AndroidAutoEntity entity = androidAutoEntity_;
        if (entity == null) {
            ConnectionManager.instance().userExited(getString(R.string.connection_user_stopped));
            stop();
            return;
        }

        mainHandler_.removeCallbacks(shutdownTimeout_);
        mainHandler_.postDelayed(shutdownTimeout_, SHUTDOWN_TIMEOUT_MS);
        try {
            entity.shutdown();
        } catch (Throwable error) {
            Log.e(TAG, "error requesting graceful Android Auto shutdown", error);
            ConnectionManager.instance().userExited(getString(R.string.connection_user_stopped));
            stop();
        }
    }

    public void releaseFocus(){
        if (androidAutoEntity_ != null) {
            androidAutoEntity_.releaseFocus();
        }
    }

    public void gainFocus(){
        if (androidAutoEntity_ != null) {
            androidAutoEntity_.gainFocus();
        }
    }

    /**
     * Whether a new projection session may be started right now. A session that
     * is still running, or one whose native teardown has not finished yet, must
     * be left alone.
     */
    private boolean canStartSession(){
        if (isRunning_ || androidAutoEntity_ != null || stopping_.get()) {
            if (Log.isWarn()) Log.w(TAG, "start ignored: previous session still active or shutting down");
            return false;
        }
        return true;
    }

    public void stop(){
        stopRequested_ = true;
        if (mainHandler_ != null) {
            mainHandler_.removeCallbacks(shutdownTimeout_);
        }

        // The native teardown is not reentrant, and stop() arrives from the UI
        // thread, from the connection thread and from native quit callbacks.
        if (!stopping_.compareAndSet(false, true)) {
            if (Log.isInfo()) Log.i(TAG, "stop already in progress");
            return;
        }

        try {
            if (!isRunning_ && androidAutoEntity_ == null) {
                if (Log.isInfo()) Log.i(TAG, "service not running, already stopped?");
                localBroadcastManager_.sendBroadcast(new Intent(ODAService.STOP_ACTION));
                stopService(new Intent(this, ODAService.class));
                return;
            }

            isRunning_ = false;

            if (Log.isInfo()) Log.i(TAG, "Stop");

            // Every step below is individually guarded: anything that escapes here
            // would leave androidAutoEntity_ set, and since that field gates
            // canStartSession() the app could never connect again without being
            // force stopped.
            if (Settings.instance().advanced.hondaIntegrationEnabled()){
                // init() only runs at boot when the setting was already on, so
                // enabling it later leaves the singleton null.
                HondaConnectManager hondaManager = HondaConnectManager.instance();
                if (hondaManager != null) {
                    try {
                        hondaManager.endAudioBinding();
                    } catch (Throwable t) {
                        Log.e(TAG, "error ending Honda audio binding", t);
                    }
                }
            }

            AndroidAutoEntity entity = androidAutoEntity_;
            LibUsbDevice sessionUsbDevice = MODE_USB.equals(currentMode_)
                    ? usbManager_.aoapDevice() : null;
            if (entity != null) {
                try {
                    entity.stop();
                } catch (Throwable t) {
                    Log.e(TAG, "error stopping Android Auto entity", t);
                }
                if (sessionUsbDevice != null) {
                    usbManager_.resetSession(sessionUsbDevice);
                }
                try {
                    entity.delete();
                } catch (Throwable t) {
                    Log.e(TAG, "error deleting Android Auto entity", t);
                }
            }

            if (sessionUsbDevice != null) {
                usbManager_.finishSession(sessionUsbDevice);
            }

            Intent stopIntent = new Intent(ODAService.STOP_ACTION);
            localBroadcastManager_.sendBroadcast(stopIntent);

            Intent service = new Intent(this, ODAService.class);
            stopService(service);
        } finally {
            // Cleared unconditionally so a failed teardown cannot wedge the
            // service into a state where no further session can be started.
            androidAutoEntity_ = null;
            startThread_ = null;
            currentMode_ = null;
            shutdownRequested_ = false;
            stopping_.set(false);
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    public void onDestroy() {
        if (Log.isDebug()) Log.d(TAG, "onDestroy");
        // stopService() from the home screen or the exit widget destroys this
        // service without going through stop(), which used to leave the session,
        // its io_service threads and the USB handle alive. The replacement service
        // then starts with androidAutoEntity_ == null and happily builds a second
        // session on top of the first one. stop() is idempotent via stopping_.
        stop();
        super.onDestroy();
    }

    @Override
    public boolean onUnbind(Intent intent) {
        if (Log.isDebug()) Log.d(TAG, "onUnbind");
        return super.onUnbind(intent);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (Log.isDebug()) Log.d(TAG, "Received start id " + startId + ": " + intent);

        Notification notification = notificationFactory_.create();
        startForeground(123456, notification);

//        return START_NOT_STICKY;
        return START_STICKY;
    }

    @Keep
    @Override
    public void onAndroidAutoQuit() {
        runNativeQuitOnMainThread(() -> {
            if (shutdownRequested_) {
                ConnectionManager.instance().userExited(getString(R.string.connection_user_stopped));
            } else {
                ConnectionManager.instance().detached(getString(R.string.connection_session_ended));
            }
            stop();
        });
    }

    @Keep
    @Override
    public void onAndroidAutoQuitOnError(String error, int nativeErrorCode){
        runNativeQuitOnMainThread(() -> {
            if (shutdownRequested_) {
                if (Log.isInfo()) Log.i(TAG, "transport closed while graceful shutdown was pending");
                ConnectionManager.instance().userExited(getString(R.string.connection_user_stopped));
                stop();
                return;
            }
            Log.e(TAG, "closing with error " + error + "(" + nativeErrorCode + ")");
            String message = getString(R.string.connection_native_error_code, error, nativeErrorCode);
            ConnectionManager.instance().failed(message);
            notifyUser(message);

            stop();
        });
    }

    /**
     * Native errors are delivered from an aasdk io_service worker. Destroying the
     * entity on that same worker leaves its currently executing asio handler
     * running against a destroyed strand, producing the SEGV seen on disconnect.
     * Queue the lifecycle work on Android's main thread so the worker can return
     * before Runtime.stopIOServiceWorkers() joins it.
     */
    private void runNativeQuitOnMainThread(Runnable action) {
        if (mainHandler_ == null || Looper.myLooper() == Looper.getMainLooper()) {
            action.run();
        } else {
            mainHandler_.post(action);
        }
    }

    /** Shows a message to the user from any thread. */
    private void notifyUser(String message){
        mainHandler_.post(() -> Toast.makeText(this, message, Toast.LENGTH_LONG).show());
    }

    @Keep
    @Override
    public void onAVChannelStopIndication() {
        if (Log.isInfo()) Log.i(TAG, "stop video indication");
        Intent stopIntent = new Intent(ODAService.STOP_VIDEO_INDICATION);
        localBroadcastManager_.sendBroadcast(stopIntent);
    }

    public class ServiceBinder extends Binder {
        public ODAService getService() {
            return ODAService.this;
        }
    }

}
