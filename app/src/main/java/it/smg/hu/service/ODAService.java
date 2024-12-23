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

import androidx.annotation.Keep;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
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

    private final IBinder mBinder = new ServiceBinder();

    private LocalBroadcastManager localBroadcastManager_;
    private NotificationFactory notificationFactory_;
    private AndroidAutoEntity androidAutoEntity_;

    private  USBManager usbManager_;
    private  WIFIManager wifiManager_;

    private Thread startThread_;

    private Handler mainHandler_;

    private boolean isRunning_;

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

    public void startUsb(SurfaceView surfaceView){
        startThread_ = new Thread(() -> {
            Looper.prepare();

            if (usbManager_.aoapDevice() != null) {
                isRunning_ = true;

                if (Log.isVerbose()) Log.v(TAG, "aoap device available, start in usb mode");
                try {
                    LibUsbDevice device = usbManager_.aoapDevice();
                    if (device.open()) {
                        if (Log.isInfo()) Log.i(TAG, "device opened");
                        androidAutoEntity_ = AndroidAutoEntityFactory.create(this, device, surfaceView);
                        androidAutoEntity_.start(this);
                    } else {
                        Log.e(TAG, "Error in open usb device");
                        onAndroidAutoQuitOnError("USB OPEN DEVICE", -1);
                        return;
                    }
                } catch (Exception e){
                    Log.e(TAG, "error", e);
                    onAndroidAutoQuitOnError("USB GENERIC ERROR", -1);
                    return;
                }
            }
            if (Log.isInfo()) Log.i(TAG, "start usb thead completed");
        });
        startThread_.start();
    }

    public void startWifi(SurfaceView surfaceView){
        startThread_ = new Thread(() -> {
            Looper.prepare();

            try {
                String ipAddress = wifiManager_.getIpAddress();
                if (ipAddress != null) {
                    isRunning_ = true;

//                    if (Log.isInfo()) Log.i(TAG, "Connect to ip " + ipAddress);
                    TCPEndpoint tcpEndpoint = new TCPEndpoint(ipAddress);
                    androidAutoEntity_ = AndroidAutoEntityFactory.create(this, tcpEndpoint, surfaceView);
                    androidAutoEntity_.start(this);
                }
                if (Log.isInfo()) Log.i(TAG, "start wifi thead completed");
            } catch (TCPConnectException e){
                Log.e(TAG, "TCP Connection error", e);
                stop();
            }
        });
        startThread_.start();
    }

    public void shutdown(){
        androidAutoEntity_.shutdown();
    }

    public void releaseFocus(){
        if (androidAutoEntity_ != null) {
            androidAutoEntity_.releaseFocus();
        }
    }

    public void gainFocus(){
        androidAutoEntity_.gainFocus();
    }

    public void stop(){
        if (!isRunning_) {
            if (Log.isInfo()) Log.i(TAG, "service not running, already stopped?");
            return;
        }

        isRunning_ = false;

        if (Log.isInfo()) Log.i(TAG, "Stop");

        if (androidAutoEntity_ != null) {
            androidAutoEntity_.stop();
        }

        if (androidAutoEntity_ != null) {
            androidAutoEntity_.delete();
            androidAutoEntity_ = null;
        }

        startThread_ = null;

        Intent stopIntent = new Intent(ODAService.STOP_ACTION);
        localBroadcastManager_.sendBroadcast(stopIntent);

        Intent service = new Intent(this, ODAService.class);
        stopService(service);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    public void onDestroy() {
        if (Log.isDebug()) Log.d(TAG, "onDestroy");
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
        stop();
    }

    @Keep
    @Override
    public void onAndroidAutoQuitOnError(String error, int nativeErrorCode){
        Log.e(TAG, "closing with error " + error + "(" + nativeErrorCode + ")");

        mainHandler_.post(() -> {
            Toast.makeText(this, "Closed due to " + error + " error", Toast.LENGTH_LONG).show();
        });

        stop();
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
