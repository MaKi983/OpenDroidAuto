package it.smg.hu.projection;

import android.view.SurfaceView;

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

    @Override
    public boolean open() {
        return true;
    }

    @Override
    public boolean init() {
        return true;
    }

    @Override
    public void write(long timestamp, ByteBuffer data) {}

    @Override
    public void stop() {}
}
