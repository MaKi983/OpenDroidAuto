package it.smg.libs.common;

import java.nio.ByteBuffer;

public class Log {

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("common");
    }

    private static ILog log_ = new ILog(){};

    public static void init(ILog log){
        log_ = log;
        nativeInit();
    }

    public static void shutdown(){
        if (log_ != null){
            log_.shutdown();
        }
    }

    public static ILog instance(){
        return log_;
    }

    public static int logLevel() { return log_.logLevel_(); }

    public static void setLogLevel(int logLevel){
        log_.setLogLevel_(logLevel);
        nativeInit();
    }

    public static boolean logProtocol() { return log_.logProtocol_(); }

    public static void setLogProtocol(boolean logProtocol){
        log_.setLogProtocol_(logProtocol);
        nativeInit();
    }

    public static void reload() {
        log_.reload();
    }

    public static boolean isVerbose(){
        return log_.isVerbose_();
    }

    public static boolean isDebug(){
        return log_.isDebug_();
    }

    public static boolean isInfo(){
        return log_.isInfo_();
    }

    public static boolean isWarn(){
        return log_.isWarn_();
    }

    public static boolean isError(){
        return log_.isError_();
    }

    public static void v(String tag, byte[] buffer){
        log_.v_(tag, buffer);
    }

    public static void v(String tag, ByteBuffer buffer){
        log_.v_(tag, buffer);
    }

    public static void v(String tag, String message){
        log_.v_(tag, message);
    }

    public static void v(String tag, String message, Throwable t){
        log_.v_(tag, message, t);
    }

    public static void d(String tag, String message){
        log_.d_(tag, message);
    }

    public static void d(String tag, String message, Throwable t){
        log_.d_(tag, message, t);
    }

    public static void i(String tag, String message){
        log_.i_(tag, message);
    }

    public static void i(String tag, String message, Throwable t){
        log_.i_(tag, message, t);
    }

    public static void w(String tag, String message){
        log_.w_(tag, message);
    }

    public static void w(String tag, String message, Throwable t){
        log_.w_(tag, message, t);
    }

    public static void e(String tag, String message){
        log_.e_(tag, message);
    }

    public static void e(String tag, String message, Throwable t){
        log_.e_(tag, message, t);
    }

    private static native void nativeInit();

}
