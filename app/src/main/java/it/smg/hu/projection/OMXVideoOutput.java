package it.smg.hu.projection;

import android.view.SurfaceView;

import java.nio.ByteBuffer;

import it.smg.libs.common.Log;
import it.smg.libs.omxvideocodec.OMXVideoCodec;

public class OMXVideoOutput extends VideoOutput /*implements Runnable*/ {

    private static final String TAG = "OMXVideoOutput";
    private OMXVideoCodec videoCodec_;

    private int frameSize_;
    private boolean configured_;
    private boolean running_;

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

    @Override
    public boolean open() {
        return true;
    }

    @Override
    public boolean init() {
        videoCodec_ = new OMXVideoCodec(fps_);
        videoCodec_.setSurface(surfaceView_.getHolder().getSurface(), surfaceView_.getWidth(), surfaceView_.getHeight());
        videoCodec_.init();
        configured_ = true;
        running_ = true;
        return true;
    }

    @Override
    public void write(long timestamp, ByteBuffer data) {
        if (configured_ && running_) {
            if (Log.isVerbose()) Log.v(TAG, "video message size: " + data.limit());
            videoCodec_.mediaDecode(timestamp, data, data.limit());
        }
    }

    @Override
    public void stop() {
        if(Log.isInfo()) Log.i(TAG, "stop");
        if (running_) {
            running_ = false;
            configured_ = false;
            videoCodec_.shutdown();
            if(Log.isInfo()) Log.i(TAG, "deleted");
            videoCodec_ = null;
        }
    }

    @Override
    protected String tag() {
        return TAG;
    }
}
