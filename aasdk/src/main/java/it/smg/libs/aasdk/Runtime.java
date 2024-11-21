package it.smg.libs.aasdk;

import android.content.Context;

import androidx.annotation.Keep;

import it.smg.libs.common.ILog;
import it.smg.libs.common.Log;

public class Runtime {

    private static final String TAG = "Runtime";

    private static native long nativeInit();
    private static native void nativeFinalize(long handle);
    private static native void nativeStartIOServiceWorker(long handle, int threads);
    private static native void nativeStopIOServiceWorker(long handle);

    private static long handle;

    private static Thread.UncaughtExceptionHandler exceptionHandler;

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("common");
        System.loadLibrary("aasdk-jni");
    }

    public static void delete(){
        nativeFinalize(handle);
        handle = 0;
    }

    @Keep
    private static void initExceptionHanlder(){
        if (exceptionHandler != null) {
            Thread.setDefaultUncaughtExceptionHandler(exceptionHandler);
        }
    }

    public static void setExceptionHandler(Thread.UncaughtExceptionHandler exceptionHandler){
        Runtime.exceptionHandler = exceptionHandler;
    }

    public static void startThreads(int threads){
        nativeStartIOServiceWorker(handle, threads);
    }

    public static void stopThreads(){
        nativeStopIOServiceWorker(handle);
    }

    public static long handle(){
        return handle;
    }

    public static void initLog(ILog log) {
        Log.init(log);
    }

    public static void init(Context ctx){
        initExceptionHanlder();

        handle = nativeInit();
    }

}
