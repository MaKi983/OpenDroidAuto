package it.smg.hu.projection;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Build;
import android.view.Surface;
import android.view.SurfaceView;

import androidx.annotation.Keep;
import androidx.annotation.RequiresApi;

import java.io.IOException;
import java.nio.ByteBuffer;

import it.smg.libs.common.Log;

@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
public class NativeVideoOutput extends VideoOutput implements Runnable {

    private static final String TAG = "NativeVideoOutput";

    /**
     * Input-buffer wait, in microseconds. This runs on the transport thread that
     * delivers video, so a long wait stalls the whole session - audio and control
     * messages included. The previous 3 s meant a decoder hiccup froze everything;
     * roughly two frames is enough to ride out jitter and still fail fast.
     */
    private static final long cDequeueTimeoutUs = 66_000;

    // Same reasoning as OMXVideoOutput: write()/stop() are @Keep entry points
    // reached from different native threads, so the codec reference has to be
    // published safely and read into a local before use.
    private volatile MediaCodec codec_;
    private volatile boolean configured_;
    private volatile boolean running_;
    private Thread codecThread_;

    public NativeVideoOutput(SurfaceView surfaceView){
        super(surfaceView);
    }

    @Override
    protected String tag() {
        return TAG;
    }

    @Keep
    @Override
    public boolean open() {
        return true;
    }

    @Keep
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
            running_ = true;
            codec_.start();
            codecThread_.start();
            return true;
        } catch (IOException e) {
            return false;
        }
    }

    @Keep
    @Override
    public void write(long timestamp, ByteBuffer buf) {
        MediaCodec codec = codec_;
        if (configured_ && running_ && codec != null) {
            try {
                int index = codec.dequeueInputBuffer(cDequeueTimeoutUs);
                if (index >= 0) {
                    ByteBuffer buffer;
                    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                        buffer = codec.getInputBuffers()[index];
                        buffer.clear();
                    } else {
                        buffer = codec.getInputBuffer(index);
                    }
                    if (buffer != null) {
                        buffer.put(buf);
                        buffer.flip();
                        codec.queueInputBuffer(index, 0, buf.limit(), timestamp, 0);
                    }
                }
            } catch (IllegalStateException e) {
                // The codec was released underneath us by a concurrent stop().
                if (Log.isWarn()) Log.w(TAG, "dropping frame, codec no longer usable");
            }
        }
    }

    @Keep
    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "shutdown");
        if (running_) {
            running_ = false;
            configured_ = false;
            try {
                if (codecThread_ != null) {
                    codecThread_.join(1000);
                }
            } catch (InterruptedException ignored) {
                Thread.currentThread().interrupt();
            }

            // Cleared before release so a writer that is still in flight sees null
            // instead of reaching into a freed codec.
            MediaCodec codec = codec_;
            codec_ = null;
            if (codec != null) {
                try {
                    codec.flush();
                    codec.stop();
                } catch (IllegalStateException e) {
                    Log.e(TAG, "error stopping codec", e);
                }
                codec.release();
            }
        }
    }

    @Override
    public void run() {
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        while (running_) {
            MediaCodec codec = codec_;
            if (configured_ && codec != null) {
                try {
                    int index = codec.dequeueOutputBuffer(info, 10000);
                    if (index >= 0) {
                        if (Log.isVerbose()) Log.v(TAG, "outputBufferIndex: " + index);
                        if (Log.isVerbose()) {
                            ByteBuffer buffer = Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP
                                    ? codec.getOutputBuffer(index)
                                    : codec.getOutputBuffers()[index];
                            Log.v(TAG, "outputBuffer: " + buffer);
                        }

                        // setting true is telling system to render frame onto Surface
                        codec.releaseOutputBuffer(index, true);
                    }
                } catch (IllegalStateException e) {
                    // Released by a concurrent stop(); the loop condition will pick
                    // that up on the next pass.
                    if (Log.isWarn()) Log.w(TAG, "output loop stopping, codec released");
                }
            } else {
                // Nothing to drain yet: without this the loop spins at 100% CPU,
                // which this hardware cannot spare.
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }
        if (Log.isVerbose()) Log.v(TAG, "thread closed");
    }
}
