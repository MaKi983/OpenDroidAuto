package it.smg.hu;

import android.content.Context;
import android.content.Intent;

import androidx.multidex.MultiDexApplication;

import it.smg.hu.config.ODALog;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
import it.smg.hu.service.ODAService;

import it.smg.hu.ui.notification.AppBadge;
import it.smg.libs.aasdk.Runtime;
import it.smg.libs.common.Log;

public class ODAApplication extends MultiDexApplication {

    private static final String TAG = "ODAApplication";

    @Override
    public void onTerminate() {
        super.onTerminate();
        Log.shutdown();
        Runtime.delete();
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

        Settings.build(base);
        Runtime.setExceptionHandler((thread, t) -> {
            Log.e(TAG, "uncaughtException in " + thread.getName(), t);

            Log.shutdown();
            Runtime.delete();

            Intent service = new Intent(this, ODAService.class);
            stopService(service);

            android.os.Process.killProcess(android.os.Process.myPid());
        });

        Runtime.init(getApplicationContext());
        Runtime.initLog(new ODALog());
    }

    public void onCreate() {
        super.onCreate();

        if (Log.isDebug()) Log.d(TAG, "Initialized jni: " + Runtime.handle());

        USBManager.init(getApplicationContext());
        WIFIManager.init(getApplicationContext());
        AppBadge.init(getApplicationContext());
    }
}