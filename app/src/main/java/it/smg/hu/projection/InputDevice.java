package it.smg.hu.projection;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.Keep;

import java.util.HashMap;
import java.util.Map;

import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.hu.ui.settings.KeymapFragment;
import it.smg.libs.common.Log;

public class InputDevice extends it.smg.libs.aasdk.projection.InputDevice implements View.OnTouchListener, View.OnKeyListener {
    private static final String TAG = "InputDevice";

    private final Rect screenGeometry_;
    private final Rect touchScreenGeometry_;
    private final boolean isTouchScreenAvailable_;
    private Map<Integer, Integer> supportedButtonCodes_;

    private SurfaceView surfaceView_;
    private InputDevice.OnKeyHolder keyHolder_;
    private Context context_;

    public InputDevice(Context ctx, SurfaceView surfaceView, InputDevice.OnKeyHolder keyHolder){
        surfaceView_ = surfaceView;
        keyHolder_ = keyHolder;
        context_ = ctx;

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

        supportedButtonCodes_ = new HashMap<>();
        for (KeymapFragment.KeyMap c : KeymapFragment.KeyMap.values()){
            int k = settings.keymap.key(c.keyName() + "_code");
            if (k == 0){
                settings.keymap.key(c.keyName() + "_code", c.defaultValue());
                k = c.defaultValue();
            }

            if (k != -1){
                int v = settings.keymap.key(c.keyName());
                if (v == 0){
                    settings.keymap.key(c.keyName(), c.defaultAction());
                    v = c.defaultAction();
                }
                supportedButtonCodes_.put(k, v);
            }
        }

        if (Log.isDebug()) Log.d(TAG, "supported button codes: " + supportedButtonCodes_);
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
        keyHolder_ = null;
    }

    @Keep
    @Override
    public void gainFocus() {
        Log.d(TAG, "gain focus");
        if (surfaceView_ != null) {
            if (Log.isDebug()) Log.d(TAG,"gainFocus -> add touchlistener to surfaceview");
            surfaceView_.setOnTouchListener(this);
        }
        if (keyHolder_ != null){
            keyHolder_.setOnKeyListener(this);
        }
    }

    @Keep
    @Override
    public void releaseFocus() {
        if (surfaceView_ != null) {
            if (Log.isDebug()) Log.d(TAG,"releaseFocus -> remove touchlistener from surfaceview");
            surfaceView_.setOnTouchListener(null);
        }
        if (keyHolder_ != null){
            keyHolder_.setOnKeyListener(null);
        }
    }

    @Keep
    @Override
    public int[] getSupportedButtonCodes() {
        int[] codes;
        if (supportedButtonCodes_ != null && !supportedButtonCodes_.isEmpty()){
            codes = new int[supportedButtonCodes_.size()];
            int i = 0;
            for (Integer c : supportedButtonCodes_.values() ) {
                codes[i] = c;
                i++;
            }
        } else {
            codes = new int[0];
        }

        return codes;
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

    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {
        if (supportedButtonCodes_ != null && supportedButtonCodes_.containsKey(keyCode)) {
            int button = supportedButtonCodes_.get(keyCode);
            if (Log.isVerbose()) Log.v(TAG, "found supported button " + button + "/" + keyCode);
            if (Log.isDebug()) {
                new Handler(Looper.getMainLooper()).post(() -> {
                    Toast.makeText(context_, "event key: " + keyCode + " action: " + event.getAction() + " -> button: " + button, Toast.LENGTH_SHORT).show();
                });
            }

            final int plusKeyCode = Settings.instance().keymap.key(KeymapFragment.KeyMap.PLUS.keyName() + "_code");
            final int minusKeyCode = Settings.instance().keymap.key(KeymapFragment.KeyMap.MINUS.keyName() + "_code");

            if (keyCode == plusKeyCode){
                HondaConnectManager.instance().increaseVolume();
            } else if (keyCode == minusKeyCode) {
                HondaConnectManager.instance().decreaseVolume();
            } else {
                int action = event.getAction(); // 1 - UP, 0 - DOWN
                sendButtonEvent(action, button);
            }

            return true;
        }

        return false;
    }

    public interface OnKeyHolder {
        void setOnKeyListener(View.OnKeyListener listener);
    }

}
