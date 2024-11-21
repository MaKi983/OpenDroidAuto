package it.smg.hu.projection;

import android.view.SurfaceView;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

public class FakeVideoOutput extends VideoOutput {

    private static final String TAG = "FakeVIdeoOutput";

    public FakeVideoOutput(SurfaceView surfaceView) {
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
        return true;
    }

    @Keep
    @Override
    public void write(long timestamp, ByteBuffer data) {}

    @Keep
    @Override
    public void stop() {}
}
