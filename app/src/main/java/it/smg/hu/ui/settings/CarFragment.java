package it.smg.hu.ui.settings;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

import it.smg.hu.R;
import it.smg.hu.config.Settings;

public class CarFragment extends BaseSettingsFragment {

    private static final String TAG = "CarFragment";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.car_fragment, container, false);

        settings = Settings.instance();

        EditText huName = view.findViewById(R.id.hu_name);
        initEditText(huName, settings.car, Settings.Car.CAR_SETTINGS_HU_NAME, Settings.Car.CAR_SETTINGS_HU_NAME_DEFAULT_VALUE, null);

        EditText carModel = view.findViewById(R.id.car_model);
        initEditText(carModel, settings.car, Settings.Car.CAR_SETTINGS_MODEL, Settings.Car.CAR_SETTINGS_MODEL_DEFAULT_VALUE, null);

        EditText carYear = view.findViewById(R.id.car_year);
        initEditText(carYear, settings.car, Settings.Car.CAR_SETTINGS_YEAR, Settings.Car.CAR_SETTINGS_YEAR_DEFAULT_VALUE, null);

        EditText huMaker = view.findViewById(R.id.hu_make);
        initEditText(huMaker, settings.car, Settings.Car.CAR_SETTINGS_HU_MAKE, Settings.Car.CAR_SETTINGS_HU_MAKE_DEFAULT_VALUE, null);

        EditText huModel = view.findViewById(R.id.hu_model);
        initEditText(huModel, settings.car, Settings.Car.CAR_SETTINGS_HU_MODEL, Settings.Car.CAR_SETTINGS_HU_MODEL_DEFAULT_VALUE, null);

        EditText swBuild = view.findViewById(R.id.sw_build);
        initEditText(swBuild, settings.car, Settings.Car.CAR_SETTINGS_SW_BUILD, Settings.Car.CAR_SETTINGS_SW_BUILD_DEFAULT_VALUE, null);

        EditText swVersion = view.findViewById(R.id.sw_version);
        initEditText(swVersion, settings.car, Settings.Car.CAR_SETTINGS_SW_VERSION, Settings.Car.CAR_SETTINGS_SW_VERSION_DEFAULT_VALUE, null);

        CheckBox playMediaDuringVR = view.findViewById(R.id.play_media_during_vr);
        initCheckBox(playMediaDuringVR, settings.car, Settings.Car.CAR_SETTINGS_PLAY_MEDIA_DURING_VR, Settings.Car.CAR_SETTINGS_PLAY_MEDIA_DURING_VR_DEFAULT_VALUE);

        CheckBox hideClock = view.findViewById(R.id.hide_clock);
        initCheckBox(hideClock, settings.car, Settings.Car.CAR_SETTINGS_HIDE_CLOCK, Settings.Car.CAR_SETTINGS_HIDE_CLOCK_DEFAULT_VALUE);

        CheckBox leftDrive = view.findViewById(R.id.left_drive);
        initCheckBox(leftDrive, settings.car, Settings.Car.CAR_SETTINGS_LEFT_HAND_DRIVE, Settings.Car.CAR_SETTINGS_LEFT_HAND_DRIVE_DEFAULT_VALUE);

        CheckBox disableAutoStartOnUsb = view.findViewById(R.id.disableStartUsbBadge);
        initCheckBox(disableAutoStartOnUsb, settings.car, Settings.Car.DISABLE_START_USB_BADGE, Settings.Car.DISABLE_START_USB_BADGE_DEFAULT_VALUE);

        SeekBar autoStartTimerSeekBar = view.findViewById(R.id.autoStartTimerSeekBar);
        TextView autoStartTimerValue = view.findViewById(R.id.autoStartTimerValueNumber);
        initSeekBar(
                autoStartTimerSeekBar,
                autoStartTimerValue,
                settings.car,
                Settings.Car.AUTO_START_TIMER,
                0,   // default value
                0,   // minimum seconds
                10   // maximum seconds
        );

        return view;

    }

    @Override
    protected String tag() {
        return TAG;
    }
}
