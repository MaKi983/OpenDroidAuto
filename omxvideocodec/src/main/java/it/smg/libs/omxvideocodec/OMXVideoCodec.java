package it.smg.libs.omxvideocodec;

import android.view.Surface;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

public class OMXVideoCodec {

    private static final String TAG = "OMXVideoCodec";
    private native void nativeSurfaceInit(long mNativeHandle, Object surface, int width, int height);
    private native long createNativeApp();
    private native boolean nativeInit(long mNativeHandle, int fps);
    private native void nativeFinalize(long mNativeHandle);
    private native void nativeConsume(long mNativeHandle, ByteBuffer buf, int len, long t);
    private native void nativeSetSps(long mNativeHandle, ByteBuffer buf, int len);

    private long handle_ = 0;

    private Surface surfaceView_;
    private int width_;
    private int height_;
    private int fps_;

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("common");
        System.loadLibrary("omxvideocodec-jni");
    }

    public OMXVideoCodec(int fps){
        handle_ = createNativeApp();

        fps_ = fps;
    }

    public void setSurface(Surface surface, int width, int height){
        surfaceView_ = surface;
        width_ = width;
        height_ = height;
        nativeSurfaceInit (handle_, surface, height_, width_);
    }

    public boolean init() {
        return nativeInit(handle_, fps_);
    }

    public void shutdown() {
        nativeFinalize(handle_);
        handle_ = 0;
    }

    public void mediaDecode(long timestamp, ByteBuffer buf, int len) {
        if (isSps(buf)) {
            nativeSetSps(handle_, buf, len);
        }
        nativeConsume(handle_, buf, len, timestamp);
    }

    private boolean isSps(ByteBuffer buf) {
        boolean sps = (buf.get(4) & 0x1f ) == 7;
        return sps;
    }
}
