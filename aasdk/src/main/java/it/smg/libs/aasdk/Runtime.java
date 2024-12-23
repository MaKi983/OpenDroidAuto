package it.smg.libs.aasdk;

import androidx.annotation.Keep;

import it.smg.libs.common.ILog;
import it.smg.libs.common.Log;

public class Runtime {

    private static final String TAG = "Runtime";
    private static native void nativeInit();
    private native long nativeSetup();
    private native void nativeDelete();
    private native void nativeStartIOServiceWorker(int threads);
    private native void nativeStopIOServiceWorker();

    @Keep
    private long handle_;

//    private static Thread.UncaughtExceptionHandler exceptionHandler;

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("common");
        System.loadLibrary("aasdk-jni");

        nativeInit();
    }

    public Runtime(){
        handle_ = nativeSetup();
    }

    public void delete(){
        nativeDelete();
        handle_ = 0;
    }

//    @Keep
//    private static void initExceptionHanlder(){
//        if (exceptionHandler != null) {
//            Thread.setDefaultUncaughtExceptionHandler(exceptionHandler);
//        }
//    }

//    public static void setExceptionHandler(Thread.UncaughtExceptionHandler exceptionHandler){
//        Runtime.exceptionHandler = exceptionHandler;
//    }

    public void startThreads(int threads){
        nativeStartIOServiceWorker(threads);
    }

    public void stopThreads(){
        nativeStopIOServiceWorker();
    }
    public static void initLog(ILog log) {
        Log.init(log);
    }

//    public static void init(Context ctx){
//        initExceptionHanlder();
//    }

}
