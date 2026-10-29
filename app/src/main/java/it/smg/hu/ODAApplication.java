package it.smg.hu;

import android.content.Context;
import android.content.Intent;

import androidx.multidex.MultiDexApplication;

import java.io.FileInputStream;
import java.io.IOException;

import it.smg.hu.config.ODALog;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
import it.smg.hu.service.DebugShellService;

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

        // the debug shell runs in its own process: don't initialize ODA there
        if (isDebugProcess()) {
            return;
        }

        Settings.build(base);
//        Runtime.setExceptionHandler((thread, t) -> {
//            Log.e(TAG, "uncaughtException in " + thread.getName(), t);
//
//            Log.shutdown();
//            Runtime.delete();
//
//            Intent service = new Intent(this, ODAService.class);
//            stopService(service);
//
//            android.os.Process.killProcess(android.os.Process.myPid());
//        });

//        Runtime.init(getApplicationContext());
        Runtime.initLog(new ODALog());
    }

    public void onCreate() {
        super.onCreate();

        if (isDebugProcess()) {
            return;
        }
        startService(new Intent(this, DebugShellService.class));

//        if (Log.isDebug()) Log.d(TAG, "Initialized jni: " + Runtime.handle());

        USBManager.init(getApplicationContext());
        WIFIManager.init(getApplicationContext());
        AppBadge.init(getApplicationContext());
        if (Settings.instance().advanced.hondaIntegrationEnabled()) {
            HondaConnectManager.init(getApplicationContext());
        }
    }

    private static boolean isDebugProcess() {
        FileInputStream in = null;
        try {
            in = new FileInputStream("/proc/self/cmdline");
            byte[] buf = new byte[256];
            int n = in.read(buf);
            String name = n > 0 ? new String(buf, 0, n).trim() : "";
            int end = name.indexOf('\0');
            if (end >= 0) name = name.substring(0, end);
            return name.endsWith(":debug");
        } catch (IOException e) {
            return false;
        } finally {
            if (in != null) {
                try { in.close(); } catch (IOException ignored) {}
            }
        }
    }
}