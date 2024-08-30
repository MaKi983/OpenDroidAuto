package it.smg.libs.slesaudiocodec;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

public class SLESAudioCodec {

    private static final String TAG = "SLESAudioCodec";

    private static native void nativeInit();
    private native long nativeSetup(int sampleRate, int channelConfig, int sampleSize);
    private native void nativeConsume(ByteBuffer buf, int len, long t);
    private native void nativeDelete();
    private native void nativeStart();
    private native void nativeStop();

    private long handle_ = 0;

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("common");
        System.loadLibrary("slesaudiocodec-jni");

        nativeInit();
    }

    public SLESAudioCodec(String name, int sampleRate, int channelConfig, int sampleSize){
        handle_ = nativeSetup(sampleRate, channelConfig, sampleSize);
    }

    public boolean init() { return true; }

    public void delete() {
        nativeDelete();
        handle_ = 0;
    }

    public void write(long timestamp, ByteBuffer buf, int len) {
        if (Log.isVerbose()) Log.v(TAG, "write buffer size2: " + buf.limit());
        nativeConsume(buf, len, timestamp);
    }

    public void start(){
        new Thread(()->{
           nativeStart();
        }).start();
    }

    public void stop(){
        nativeStop();
    }
}
