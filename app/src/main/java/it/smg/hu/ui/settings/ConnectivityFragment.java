package it.smg.hu.ui.settings;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;

import it.smg.hu.R;
import it.smg.hu.config.Settings;

public class ConnectivityFragment extends BaseSettingsFragment {

    private static final String TAG = "ConnectivityFragment";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.conn_fragment, container, false);

        settings = Settings.instance();

        CheckBox enableAd2p = view.findViewById(R.id.enable_ad2p);
        initCheckBox(enableAd2p, settings.connectivity, Settings.Connectivity.CONN_ENABLED_AD2P, Settings.Connectivity.CONN_ENABLED_AD2P_DEFAULT_VALUE);

        CheckBox enableHfp = view.findViewById(R.id.enable_hfp);
        initCheckBox(enableHfp, settings.connectivity, Settings.Connectivity.CONN_ENABLED_HFP, Settings.Connectivity.CONN_ENABLED_HFP_DEFAULT_VALUE);

        return view;

    }

    @Override
    protected String tag() {
        return TAG;
    }
}
