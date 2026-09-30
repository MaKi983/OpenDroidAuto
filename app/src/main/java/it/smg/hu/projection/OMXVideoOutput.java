package it.smg.hu.projection;

import android.view.SurfaceView;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;
import it.smg.libs.omxvideocodec.OMXVideoCodec;

public class OMXVideoOutput extends VideoOutput /*implements Runnable*/ {

    private static final String TAG = "OMXVideoOutput";
    // write() runs on a native io_service thread while stop() comes from the
    // session teardown on another thread, so every field they share has to be
    // published safely - otherwise the writer can still see running_ == true and
    // reach into a codec that has already been shut down.
    private volatile OMXVideoCodec videoCodec_;

    private int frameSize_;
    private volatile boolean configured_;
    private volatile boolean running_;

    public OMXVideoOutput(SurfaceView surfaceView){
        super(surfaceView);
        frameSize_ = getFrameSizeFromResolution();
    }

    private int getFrameSizeFromResolution(){
        switch (resolution_){
            case 1: //480p
                return 800*480;
            case 2:
                return 1280*720;
            case 3:
                return 1920*1080;
            case 4:
                return 2560*1440;
            default:
                return -1;
        }
    }

    @Keep
    @Override
    public boolean open() {
        return true;
    }

    @Keep
    @Override
    public boolean init() {
        videoCodec_ = new OMXVideoCodec(fps_);
        videoCodec_.setSurface(surfaceView_.getHolder().getSurface(), surfaceView_.getWidth(), surfaceView_.getHeight());
        videoCodec_.init();
        configured_ = true;
        running_ = true;
        return true;
    }

    @Keep
    @Override
    public void write(long timestamp, ByteBuffer data) {
        // Read the codec once: stop() can null it out between the guard and the
        // call, which used to be an NPE (or worse, a decode into a codec whose
        // native side was already deleted) on every teardown while frames were
        // still arriving.
        OMXVideoCodec videoCodec = videoCodec_;
        if (configured_ && running_ && videoCodec != null) {
            if (Log.isVerbose()) Log.v(TAG, "video message size: " + data.limit());
            videoCodec.mediaDecode(timestamp, data, data.limit());
        }
    }

    @Keep
    @Override
    public void stop() {
        if(Log.isInfo()) Log.i(TAG, "stop");
        if (running_) {
            running_ = false;
            configured_ = false;
            OMXVideoCodec videoCodec = videoCodec_;
            videoCodec_ = null;
            if (videoCodec != null) {
                videoCodec.shutdown();
            }
            if(Log.isInfo()) Log.i(TAG, "deleted");
        }
    }

    @Override
    protected String tag() {
        return TAG;
    }
}
