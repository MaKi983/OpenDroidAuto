package it.smg.hu.ui.settings;

import static it.smg.hu.config.Settings.Keymap.KEYMAP_BTN_LEFT;
import static it.smg.hu.config.Settings.Keymap.KEYMAP_BTN_MINUS;
import static it.smg.hu.config.Settings.Keymap.KEYMAP_BTN_PLUS;
import static it.smg.hu.config.Settings.Keymap.KEYMAP_BTN_RIGHT;
import static it.smg.hu.config.Settings.Keymap.KEYMAP_BTN_SOURCE;

import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.hu.projection.InputDevice;
import it.smg.libs.common.Log;

public class KeymapFragment extends BaseSettingsFragment implements View.OnClickListener, View.OnLongClickListener, View.OnKeyListener {

    private static final String TAG = "KeymapFragment";

    private Button currentButton_;
    private Handler handler_;
    private Runnable unsetKeyListener_;
    private InputDevice.OnKeyHolder keyHolder_;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.keymap_fragment, container, false);

        settings = Settings.instance();
        handler_ = new Handler();
        if (settings.advanced.hondaIntegrationEnabled()){
            HondaConnectManager.instance().initAudioBinding();
            HondaConnectManager.instance().requestAudioFocus();
            keyHolder_ = (InputDevice.OnKeyHolder) getActivity();
        }

        unsetKeyListener_ = () -> {
            Log.v(TAG, "unset keyListener");
            if (keyHolder_ != null) {
                keyHolder_.setOnKeyListener(null);
            }
            currentButton_ = null;
        };

        for (KeyMap c : KeyMap.values()){
            if (c.id() != -1){
                Button b = view.findViewById(c.id());

                int currentValue = settings.keymap.key(c.keyName() + "_code");
                if (currentValue == 0){
                    b.setText("" + c.defaultValue_);
                } else if (currentValue != -1){
                    b.setText("" + currentValue);
                } else {
                    b.setText("");
                }

                b.setOnClickListener(this);
                b.setOnLongClickListener(this);
                b.setTag(c);
            }
        }

        Spinner btnLeft = view.findViewById(R.id.btn_left);
        initSpinner(btnLeft, R.array.keymap_btn_labels, R.array.keymap_btn_values, settings.keymap, KEYMAP_BTN_LEFT, Settings.Keymap.KEYMAP_BTN_LEFT_DEFAULT_VALUE);

        Spinner btnRight = view.findViewById(R.id.btn_right);
        initSpinner(btnRight, R.array.keymap_btn_labels, R.array.keymap_btn_values, settings.keymap, Settings.Keymap.KEYMAP_BTN_RIGHT, Settings.Keymap.KEYMAP_BTN_RIGHT_DEFAULT_VALUE);

        Spinner btnSource = view.findViewById(R.id.btn_source);
        initSpinner(btnSource, R.array.keymap_btn_labels, R.array.keymap_btn_values, settings.keymap, Settings.Keymap.KEYMAP_BTN_SOURCE, Settings.Keymap.KEYMAP_BTN_SOURCE_DEFAULT_VALUE);

        Spinner btnPlus = view.findViewById(R.id.btn_plus);
        initSpinner(btnPlus, R.array.keymap_btn_labels, R.array.keymap_btn_values, settings.keymap, Settings.Keymap.KEYMAP_BTN_PLUS, Settings.Keymap.KEYMAP_BTN_PLUS_DEFAULT_VALUE);

        Spinner btnMinus = view.findViewById(R.id.btn_minus);
        initSpinner(btnMinus, R.array.keymap_btn_labels, R.array.keymap_btn_values, settings.keymap, Settings.Keymap.KEYMAP_BTN_MINUS, Settings.Keymap.KEYMAP_BTN_MINUS_DEFAULT_VALUE);

        return view;
    }

    @Override
    public void onClick(View v) {
        Button b = (Button)v;
        currentButton_ = b;
        KeyMap c = (KeyMap) b.getTag();

        if (keyHolder_ != null) {
            Toast.makeText(getActivity(), "Press a key to assign to " + c.keyName(), Toast.LENGTH_SHORT).show();
            keyHolder_.setOnKeyListener(this);

            handler_.postDelayed(unsetKeyListener_, 5000);
        }

    }

    @Override
    public void onStop() {
        if (settings.advanced.hondaIntegrationEnabled()){
            HondaConnectManager.instance().endAudioBinding();
        }

        super.onStop();
    }

    @Override
    public boolean onLongClick(View v) {
        Button b = (Button)v;
        KeyMap c = (KeyMap) b.getTag();
        if (Log.isDebug()) Log.d(TAG, "Reset " + c.keyName() + "_code button");
        settings.keymap.key(c.keyName() + "_code", -1);
        b.setText("");

        return true;
    }

    @Override
    public boolean onKey(View view, int keyCode, KeyEvent event) {
        if (event.getAction() == KeyEvent.ACTION_DOWN && currentButton_ != null){
            KeyMap c = (KeyMap) currentButton_.getTag();
            if (Log.isDebug()) Log.d(TAG, "Set " + c.keyName() + "_code button to code " + keyCode);
            settings.keymap.key(c.keyName() + "_code", keyCode);

            Button b = getView().findViewById(c.id());
            b.setText(String.valueOf(keyCode));
        }

        unsetKeyListener_.run();

        return true;
    }

    @Override
    protected String tag() {
        return TAG;
    }

    public enum KeyMap {
        LEFT(KEYMAP_BTN_LEFT, R.id.keycode_left, -65532),
        RIGHT(KEYMAP_BTN_RIGHT, R.id.keycode_right, -65533),
        SOURCE(KEYMAP_BTN_SOURCE, R.id.keycode_source, -65531),
        PLUS(KEYMAP_BTN_PLUS, R.id.keycode_plus, -65535),
        MINUS(KEYMAP_BTN_MINUS, R.id.keycode_min, -65534);

        private final String keyName_;
        private final int id_;;
        private final int defaultValue_;;

        KeyMap(String keyName, int id, int defaultValue){
            keyName_ = keyName;
            id_ = id;
            defaultValue_ = defaultValue;
        }

        public String keyName(){
            return keyName_;
        }

        public int id(){
            return id_;
        }

        public int defaultValue(){
            return defaultValue_;
        }

    }

}