package it.smg.hu.projection;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import it.smg.libs.aasdk.projection.ISensor;
import it.smg.libs.common.Log;

public class LightSensor implements ISensor, SensorEventListener {

    private static final String TAG = "LightSensor";

    private final Context context_;
    private Listener listener_;
    private int currentState_;

    public LightSensor(Context ctx){
        context_ = ctx;
        currentState_ = IS_DAY;

        SensorManager sensorManager = (SensorManager)context_.getSystemService(Context.SENSOR_SERVICE);
        Sensor lightSensor = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        if (lightSensor != null) {
            sensorManager.registerListener(this, lightSensor, SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        if(sensorEvent.sensor.getType() == Sensor.TYPE_LIGHT){
            float value = sensorEvent.values[0];

            if (listener_ != null){
                int state = value <= 5 ? IS_NIGHT : IS_DAY;
                if (state != currentState_) {
                    currentState_ = state;
                    listener_.onDayNightUpdate(currentState_ == IS_NIGHT);
                }
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {}

    @Override
    public void stop() {
        if (Log.isDebug()) Log.d(TAG, "stop");
        if (Log.isDebug()) Log.d(TAG, "remove sensorManager listener");
        SensorManager sensorManager = (SensorManager)context_.getSystemService(Context.SENSOR_SERVICE);
        sensorManager.unregisterListener(this);
    }

    @Override
    public boolean isNight() {
        return currentState_ == IS_NIGHT;
    }

    @Override
    public void setListener(Listener listener) {
        listener_ = listener;
    }
}
