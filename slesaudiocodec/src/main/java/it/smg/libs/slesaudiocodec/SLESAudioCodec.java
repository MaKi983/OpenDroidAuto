package it.smg.libs.slesaudiocodec;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

public class SLESAudioCodec {

    private static final String TAG = "SLESAudioCodec";

    private static native void nativeInit();
    private native long nativeSetup(int sampleRate, int channelConfig, int sampleSize);
    private native void nativeConsume(ByteBuffer buf, int len, long t);
    private native void nativeDelete();

//    private native void nativeSurfaceInit(long mNativeHandle, Object surface, int width, int height);
//    private native long createNativeApp();
//
//    private native void nativeFinalize(long mNativeHandle);
//    private native void nativeConsume(long mNativeHandle, ByteBuffer buf, int len, long t);
//    private native void nativeSetSps(long mNativeHandle, ByteBuffer buf, int len);

    private long handle_ = 0;

//    private Surface surfaceView_;
//    private int width_;
//    private int height_;
//    private int fps_;

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("common");
        System.loadLibrary("slesaudiocodec-jni");

        nativeInit();
    }

    public SLESAudioCodec(int sampleRate, int channelConfig, int sampleSize){
        handle_ = nativeSetup(sampleRate, channelConfig, sampleSize);
    }

    public boolean init() { return true; }

    public void shutdown() {
        nativeDelete();
        handle_ = 0;
    }

    public void write(long timestamp, ByteBuffer buf, int len) {
        if (Log.isVerbose()) Log.v(TAG, "write buffer size2: " + buf.limit());
        nativeConsume(buf, len, timestamp);
    }

}
