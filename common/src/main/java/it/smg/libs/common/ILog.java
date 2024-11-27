package it.smg.libs.common;

import android.util.Log;

import java.nio.ByteBuffer;
import java.util.Arrays;

public interface ILog {

    int DEFAULT_LOG_LEVEL = Log.INFO;

    default int logLevel_(){
        return DEFAULT_LOG_LEVEL;
    }

    default void setLogLevel_(int logLevel){}

    default void setLogProtocol_(boolean logProtocol){}

    default boolean logProtocol_(){
        return false;
    }

    default void shutdown(){}

    default void reload(){}

    default boolean isVerbose_(){
        return DEFAULT_LOG_LEVEL <= Log.VERBOSE;
    }

    default boolean isDebug_(){
        return DEFAULT_LOG_LEVEL <= Log.DEBUG;
    }

    default boolean isInfo_(){
        return DEFAULT_LOG_LEVEL <= Log.INFO;
    }

    default boolean isWarn_(){
        return DEFAULT_LOG_LEVEL <= Log.WARN;
    }

    default boolean isError_(){
        return DEFAULT_LOG_LEVEL <= Log.ERROR;
    }

    default void v_(String tag, byte[] buffer){
        Log.v(tag, Arrays.toString(buffer));
    }

    default void v_(String tag, ByteBuffer buffer){
        Log.v(tag, "" + buffer);
    }

    default void v_(String tag, String message){
        Log.v(tag, message);
    }

    default void v_(String tag, String message, Throwable t){
        Log.v(tag, message, t);
    }

    default void d_(String tag, String message){
        Log.d(tag, message);
    }

    default void d_(String tag, String message, Throwable t){
        Log.d(tag, message, t);
    }

    default void i_(String tag, String message){
        Log.d(tag, message);
    }

    default void i_(String tag, String message, Throwable t){
        Log.d(tag, message, t);
    }

    default void w_(String tag, String message){
        Log.d(tag, message);
    }

    default void w_(String tag, String message, Throwable t){
        Log.d(tag, message, t);
    }

    default void e_(String tag, String message){
        Log.d(tag, message);
    }

    default void e_(String tag, String message, Throwable t){
        Log.d(tag, message, t);
    }
}
