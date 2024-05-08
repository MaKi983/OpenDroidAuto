package it.smg.hu.ui.settings;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import java.util.concurrent.Callable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.libs.aasdk.projection.ISensor;

public class VideoFragment extends BaseSettingsFragment {

    private static final String TAG = "VideoFragment";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.video_fragment, container, false);

        settings = Settings.instance();

        Spinner resolution = view.findViewById(R.id.video_resolution);
        initSpinner(resolution, R.array.video_resolution_labels, R.array.video_resolution_values, settings.video, Settings.Video.VIDEO_RESOLUTION, Settings.Video.VIDEO_RESOLUTION_DEFAULT_VALUE);

        Spinner fps = view.findViewById(R.id.video_fps);
        initSpinner(fps, R.array.video_fps_labels, R.array.video_fps_values, settings.video, Settings.Video.VIDEO_FPS, Settings.Video.VIDEO_FPS_DEFAULT_VALUE);

        EditText dpi = view.findViewById(R.id.video_dpi);
        initEditText(dpi, settings.video, Settings.Video.VIDEO_DPI, Settings.Video.VIDEO_DPI_DEFAULT_VALUE);

        EditText margin = view.findViewById(R.id.video_margin);
        initEditText(margin, settings.video, Settings.Video.VIDEO_MARGIN, Settings.Video.VIDEO_MARGIN_DEFAULT_VALUE, new Callable<Void>() {
            @Override
            public Void call() {
                String value = settings.video.margin();
                if (value == null || value.trim().isEmpty()) {
                    settings.video.margin(Settings.Video.VIDEO_MARGIN_DEFAULT_VALUE);
                    margin.setText(Settings.Video.VIDEO_MARGIN_DEFAULT_VALUE);
                } else {
                    Pattern pattern = Pattern.compile("\\d+,\\d+",  Pattern.CASE_INSENSITIVE);
                    Matcher m = pattern.matcher(value);
                    if (!m.matches()){
                        Toast.makeText(getContext(), "Wrong margin format, it must be x,y", Toast.LENGTH_SHORT).show();
                        settings.video.margin(Settings.Video.VIDEO_MARGIN_DEFAULT_VALUE);
                        margin.setText(Settings.Video.VIDEO_MARGIN_DEFAULT_VALUE);
                    }
                }
                return null;
            }
        });

        Spinner nightMode = view.findViewById(R.id.video_night_mode);
        initSpinner(nightMode, R.array.video_nightmode, R.array.video_nightmode, settings.video, Settings.Video.VIDEO_NIGHT_MODE, Settings.Video.VIDEO_NIGHT_MODE_DEFAULT_VALUE, new Callable<Void>() {
            @Override
            public Void call() {
                String value = settings.video.nightMode();
                if (value.equalsIgnoreCase(ISensor.LIGHT)) {
                    SensorManager sensorManager = (SensorManager)getContext().getSystemService(Context.SENSOR_SERVICE);
                    Sensor lightSensor = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
                    if (lightSensor == null){
                        Toast.makeText(getContext(), "Light sensor not available, switch to default TIME/GPS", Toast.LENGTH_LONG).show();
                        settings.video.nightMode(Settings.Video.VIDEO_NIGHT_MODE_DEFAULT_VALUE);

                        String[] elements = getContext().getResources().getStringArray(R.array.video_nightmode);
                        for (int i = 0; i < elements.length; i++){
                            String e = elements[i];
                            if (Settings.Video.VIDEO_NIGHT_MODE_DEFAULT_VALUE.equalsIgnoreCase(e)){
                                nightMode.setSelection(i);
                                break;
                            }
                        }

                    }
                }
                return null;
            }
        });

        CheckBox showMediaNotification = view.findViewById(R.id.showMediaNotification);
        initCheckBox(showMediaNotification, settings.video, Settings.Video.VIDEO_SHOW_MEDIA_NOTIFICATION, Settings.Video.VIDEO_SHOW_MEDIA_NOTIFICATION_DEFAULT_VALUE);

        CheckBox showNavigationNotification = view.findViewById(R.id.showNavigationNotification);
        initCheckBox(showNavigationNotification, settings.video, Settings.Video.VIDEO_SHOW_NAVIGATION_NOTIFICATION, Settings.Video.VIDEO_SHOW_NAVIGATION_NOTIFICATION_DEFAULT_VALUE);

        CheckBox showAppBadge = view.findViewById(R.id.showAppBadge);
        initCheckBox(showAppBadge, settings.video, Settings.Video.VIDEO_SHOW_APP_BADGE, Settings.Video.VIDEO_SHOW_APP_BADGE_DEFAULT_VALUE);

        return view;

    }

    @Override
    protected String tag() {
        return TAG;
    }
}
