package it.smg.hu.projection;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Build;
import android.view.Surface;
import android.view.SurfaceView;

import androidx.annotation.RequiresApi;

import java.io.IOException;
import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
public class NativeVideoOutput extends VideoOutput implements Runnable {

    private static final String TAG = "NativeVideoOutput";

    private MediaCodec codec_;
    private boolean configured_;
    private boolean running_;
    private Thread codecThread_;

    public NativeVideoOutput(SurfaceView surfaceView){
        super(surfaceView);
    }

    @Override
    protected String tag() {
        return TAG;
    }

    @Override
    public boolean open() {
        return true;
    }

    @Override
    public boolean init() {
        try {
            Surface surface = surfaceView_.getHolder().getSurface();
            int width = surfaceView_.getWidth();
            int height = surfaceView_.getHeight();

            codecThread_ = new Thread(this);
            codecThread_.setName("NativeVideoOutput-thread");

            if (Log.isInfo()) Log.i(TAG, "create codec type= " + MediaFormat.MIMETYPE_VIDEO_AVC + ", width= " + width + ", height= " + height);
            codec_ = MediaCodec.createDecoderByType(MediaFormat.MIMETYPE_VIDEO_AVC);
            MediaFormat format = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, width, height);
            format.setInteger(MediaFormat.KEY_FRAME_RATE, fps_);
//            format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);
            format.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, 655360);
            codec_.configure(format, surface, null, 0);
            configured_ = true;
            codec_.start();
            codecThread_.start();
            running_ = true;
            return true;
        } catch (IOException e) {
            return false;
        }
    }

    @Override
    public void write(long timestamp, ByteBuffer buf) {
        if (configured_ && running_) {
            int index = codec_.dequeueInputBuffer(3000000);
            if (index >= 0) {
                ByteBuffer buffer;
                if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                    buffer = codec_.getInputBuffers()[index];
                    buffer.clear();
                } else {
                    buffer = codec_.getInputBuffer(index);
                }
                if (buffer != null) {
                    buffer.put(buf);
                    buffer.flip();
                    codec_.queueInputBuffer(index, 0, buf.limit(), timestamp, 0);
                }
            }
        }
    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "shutdown");
        if (running_) {
            running_ = false;
            configured_ = false;
            try {
                codecThread_.join();
            } catch (InterruptedException e) {}

            codec_.flush();
            codec_.stop();
            codec_.release();
            codec_ = null;

        }
    }

    @Override
    public void run() {
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        while (running_) {
            if (configured_) {
                int index = codec_.dequeueOutputBuffer(info, 0);
                if (index >= 0) {
                    if (Log.isVerbose()) Log.v(TAG, "outputBufferIndex: " + index);
                    ByteBuffer buffer = null;
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                        buffer = codec_.getOutputBuffer(index);
                    } else {
                        buffer = codec_.getOutputBuffers()[index];
                    }
                    if (Log.isVerbose()) Log.v(TAG, "outputBuffer: " + buffer);

                    // setting true is telling system to render frame onto Surface
                    codec_.releaseOutputBuffer(index, true);
                }
            }
        }
        if (Log.isVerbose()) Log.v(TAG, "thread closed");
    }
}
