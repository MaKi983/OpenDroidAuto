package it.smg.hu;

import android.content.Context;
import android.content.Intent;

import androidx.multidex.MultiDexApplication;

import it.smg.hu.config.ODALog;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.hu.manager.HondaPlatform;
import it.smg.hu.manager.ConnectionManager;
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
//        Runtime.delete();
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

        Settings.build(base);

        final Thread.UncaughtExceptionHandler previousHandler = Thread.getDefaultUncaughtExceptionHandler();
        Runtime.setExceptionHandler((thread, t) -> {
            // android.util.Log is used directly (not the ODA file-log pipe) so this
            // line always reaches logcat, even if ODALog/Settings aren't in a state
            // where the custom logger can write.
            android.util.Log.e(TAG, "uncaughtException in " + thread.getName(), t);

            try {
                Log.e(TAG, "uncaughtException in " + thread.getName(), t);
                Log.shutdown();

                Intent service = new Intent(this, ODAService.class);
                stopService(service);
            } catch (Throwable inner) {
                android.util.Log.e(TAG, "error while handling uncaughtException", inner);
            }

            // Hand off to Android's default handler so the crash still gets a
            // tombstone/"App has stopped" dialog and a FATAL EXCEPTION logcat entry,
            // instead of the process just disappearing with no trace.
            if (previousHandler != null) {
                previousHandler.uncaughtException(thread, t);
            } else {
                android.os.Process.killProcess(android.os.Process.myPid());
            }
        });

        Runtime.init(getApplicationContext());
        Runtime.initLog(new ODALog());
    }

    public void onCreate() {
        super.onCreate();

//        if (Log.isDebug()) Log.d(TAG, "Initialized jni: " + Runtime.handle());

        HondaPlatform.init(getApplicationContext());
        ConnectionManager.init(getApplicationContext());
        USBManager.init(getApplicationContext());
        WIFIManager.init(getApplicationContext());
        AppBadge.init(getApplicationContext());
        if (Settings.instance().advanced.hondaIntegrationEnabled()) {
            HondaConnectManager.init(getApplicationContext());
        }
    }
}
