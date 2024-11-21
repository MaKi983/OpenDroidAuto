package it.smg.hu.projection;

import android.content.Context;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;

import androidx.annotation.Keep;

import it.smg.hu.config.Settings;
import it.smg.libs.common.Log;

public class InputDevice extends it.smg.libs.aasdk.projection.InputDevice implements View.OnTouchListener /*,View.OnKeyListener*/ {
    private static final String TAG = "InputDevice";

    private final Rect screenGeometry_;
    private final Rect touchScreenGeometry_;
    private final boolean isTouchScreenAvailable_;

    private SurfaceView surfaceView_;

    public InputDevice(Context ctx, SurfaceView surfaceView){
        surfaceView_ = surfaceView;

        Settings settings = Settings.instance();
        switch (settings.video.resolution()){
            case 2: // 720p
                screenGeometry_ = new Rect(1280, 720);
                break;
            case 3: // 1080p
                screenGeometry_ = new Rect(1920, 1080);;
                break;
            case 4: // 1440p
                screenGeometry_ = new Rect(2560, 1440);;
                break;
            default: // 480p
                screenGeometry_ = new Rect(800, 480);
                break;
        }

        isTouchScreenAvailable_ = ctx.getPackageManager().hasSystemFeature("android.hardware.touchscreen");
        if (Log.isDebug()) Log.d(TAG, "isAvailable: " + isTouchScreenAvailable_);

        if (isTouchScreenAvailable_) {
            DisplayMetrics metrics = ctx.getResources().getDisplayMetrics();
            touchScreenGeometry_ = new Rect(metrics.widthPixels, metrics.heightPixels);
        } else {
            touchScreenGeometry_ = new Rect(0,0);
        }
        if (Log.isDebug()) Log.d(TAG, "using toucscreen geometry: " + touchScreenGeometry_);
    }

    @Keep
    @Override
    public void start() {
        gainFocus();
    }

    @Keep
    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "stop");
        releaseFocus();
        surfaceView_ = null;
    }

    @Keep
    @Override
    public void gainFocus() {
        if (surfaceView_ != null) {
            if (Log.isDebug()) Log.d(TAG,"gainFocus -> add touchlistener to surfaceview");
            surfaceView_.setOnTouchListener(this);
        }
    }

    @Keep
    @Override
    public void releaseFocus() {
        if (surfaceView_ != null) {
            if (Log.isDebug()) Log.d(TAG,"releaseFocus -> remove touchlistener from surfaceview");
            surfaceView_.setOnTouchListener(null);
        }
    }

    @Keep
    @Override
    public int[] getSupportedButtonCodes() {
        return new int[]{};
    }

    @Keep
    @Override
    public boolean hasTouchscreen() {
        return isTouchScreenAvailable_;
    }

    @Keep
    @Override
    public Rect getTouchscreenGeometry() {
        return touchScreenGeometry_;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int count = event.getPointerCount();
        if (count > 0) {
            TouchAction[] ta = new TouchAction[count];
            for (int i = 0; i < count; i++) {
                final int x = (int) ((event.getX(i) / touchScreenGeometry_.width) * screenGeometry_.width);
                final int y = (int) ((event.getY(i) / touchScreenGeometry_.height) * screenGeometry_.height);
                final int owner = event.getPointerId(i);
                ta[i] = new TouchAction(x, y, owner);
            }
            sendTouchEvent(event.getActionMasked(), event.getActionIndex(), ta);
        }
        return true;
    }
}
