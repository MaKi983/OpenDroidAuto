package it.smg.hu.ui.settings;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;

import java.util.concurrent.Callable;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.libs.common.Log;

public class AdvancedFragment extends BaseSettingsFragment {

    private static final String TAG = "AdvancedFragment";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.advanced_fragment, container, false);

        settings = Settings.instance();

        CheckBox enableDebug = view.findViewById(R.id.enable_debug);
        initCheckBox(enableDebug, settings.advanced, Settings.Advanced.ADVANCED_ENABLED_DEBUG, Settings.Advanced.ADVANCED_ENABLED_DEBUG_DEFAULT_VALUE, () -> {
            Log.reload();
            return null;
        });

        EditText logDir = view.findViewById(R.id.log_dir);
        initEditText(logDir, settings.advanced, Settings.Advanced.ADVANCED_LOG_DIR, Settings.Advanced.ADVANCED_LOG_DIR_DEFAULT_VALUE, () -> {
            Log.reload();
            return null;
        });

        CheckBox enableWifi = view.findViewById(R.id.enable_wifi);
        initCheckBox(enableWifi, settings.advanced, Settings.Advanced.ADVANCED_ENABLE_WIFI, Settings.Advanced.ADVANCED_ENABLE_WIFI_DEFAULT_VALUE);

        Spinner logLevel = view.findViewById(R.id.loglevel);
        initSpinner(logLevel, R.array.loglevel_labels, R.array.loglevel_values, settings.advanced, Settings.Advanced.ADVANCED_LOG_LEVEL, Settings.Advanced.ADVANCED_LOG_LEVEL_DEFAULT_VALUE, () -> {
            Log.setLogLevel(settings.advanced.logLevel());
            return null;
        });

        CheckBox logProtocol = view.findViewById(R.id.logProtocol);
        initCheckBox(logProtocol, settings.advanced, Settings.Advanced.ADVANCED_LOG_PROTOCOL, Settings.Advanced.ADVANCED_LOG_PROTOCOL_DEFAULT_VALUE, () -> {
            Log.setLogProtocol(settings.advanced.logProtocol());
            return null;
        });

        CheckBox enableHondaIntegration = view.findViewById(R.id.enable_hondaintegration);
        initCheckBox(enableHondaIntegration, settings.advanced, Settings.Advanced.ADVANCED_ENABLE_HONDA_INTEGRATION, Settings.Advanced.ADVANCED_ENABLE_HONDA_INTEGRATION_DEFAULT_VALUE, () -> {
            if (Settings.instance().advanced.hondaIntegrationEnabled()) {
                HondaConnectManager.init(getActivity().getApplicationContext());
            }
            return null;
        });

        Spinner swMode = view.findViewById(R.id.sw_mode);
        initSpinner(swMode, R.array.swmode, R.array.swmode, settings.advanced, Settings.Advanced.ADVANCED_SW_MODE, Settings.Advanced.ADVANCED_SW_MODE_DEFAULT_VALUE, () -> {
            EditText swIdx =  view.findViewById(R.id.steeringwheel_idx);
            switch (settings.advanced.swMode()){
                case HondaConnectManager.SWMode.SW_SERVICE:
                    swIdx.setEnabled(true);
                    break;
                case HondaConnectManager.SWMode.MODEMGR_KEY_CALLBACK:
                    swIdx.setEnabled(false);
                    break;
                default:
                    break;
            }

            return null;
        });

        EditText modeMgrAudioIdx = view.findViewById(R.id.modemgraudio_idx);
        initEditText(modeMgrAudioIdx, settings.advanced, Settings.Advanced.ADVANCED_MODEMGRAUDIO_IDX, Settings.Advanced.ADVANCED_MODEMGRAUDIO_IDX_DEFAULT_VALUE);

        EditText steeringWheelIdx = view.findViewById(R.id.steeringwheel_idx);
        initEditText(steeringWheelIdx, settings.advanced, Settings.Advanced.ADVANCED_SW_IDX, Settings.Advanced.ADVANCED_SW_IDX_DEFAULT_VALUE);

        return view;
    }

    @Override
    protected String tag() {
        return TAG;
    }
}
