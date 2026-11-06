package it.smg.libs.omxvideocodec;

import android.view.Surface;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

public class OMXVideoCodec {

    private static final String TAG = "OMXVideoCodec";

    private static native void nativeInit();
    private native long nativeSetup();
    private native void nativeSurfaceInit(Object surface, int width, int height);
    private native boolean nativeDecoderInit(int fps);
    private native void nativeDelete();
    private native void nativeConsume(ByteBuffer buf, int len, long t);
    private native void nativeSetSps(ByteBuffer buf, int len);

    @Keep
    private long handle_ = 0;

    /** Guards handle_ against decode racing with teardown. */
    private final Object codecLock_ = new Object();

    private Surface surfaceView_;
    private int width_;
    private int height_;
    private int fps_;

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("common");
        System.loadLibrary("omxvideocodec-jni");

        nativeInit();
    }

    public OMXVideoCodec(int fps){
        handle_ = nativeSetup();
        fps_ = fps;
    }

    public void setSurface(Surface surface, int width, int height){
        surfaceView_ = surface;
        width_ = width;
        height_ = height;
        nativeSurfaceInit (surface, height_, width_);
    }

    public boolean init() {
        return nativeDecoderInit(fps_);
    }

    public void shutdown() {
        // Serialised against mediaDecode: frames keep arriving on a native
        // io_service thread while the session is being torn down, and feeding one
        // into a decoder whose native side has just been freed is a use-after-free.
        synchronized (codecLock_) {
            if (handle_ == 0) {
                return;
            }
            nativeDelete();
            if (Log.isVerbose()) Log.v(TAG, "Native deleted");
            handle_ = 0;
        }
    }

    public void mediaDecode(long timestamp, ByteBuffer buf, int len) {
        if (Log.isVerbose()) Log.v(TAG, "mediaDecode");
        synchronized (codecLock_) {
            if (handle_ == 0) {
                return;
            }
            if (isSps(buf)) {
                nativeSetSps(buf, len);
            }
            nativeConsume(buf, len, timestamp);
        }
    }

    private boolean isSps(ByteBuffer buf) {
        // 4-byte Annex B start code plus the NAL header byte. A runt frame used to
        // throw IndexOutOfBounds straight into the native caller, killing the
        // io_service thread that delivered it.
        if (buf.limit() < 5) {
            return false;
        }
        return (buf.get(4) & 0x1f) == 7;
    }
}
