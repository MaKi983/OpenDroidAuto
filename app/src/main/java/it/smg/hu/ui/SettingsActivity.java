package it.smg.hu.ui;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageButton;

import androidx.fragment.app.FragmentActivity;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.hu.projection.InputDevice;
import it.smg.hu.ui.settings.AdvancedFragment;
import it.smg.hu.ui.settings.CarFragment;
import it.smg.hu.ui.settings.ConnectivityFragment;
import it.smg.hu.ui.settings.KeymapFragment;
import it.smg.hu.ui.settings.VideoFragment;
import it.smg.libs.common.Log;

public class SettingsActivity extends FragmentActivity implements InputDevice.OnKeyHolder {

    private static final String TAG = "SettingsActivity";
    private View.OnKeyListener keyListener_;
    private Settings settings_;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        settings_ = Settings.instance();

        setContentView(R.layout.activity_settings);

        ImageButton carImage = findViewById(R.id.car_settings);
        carImage.setOnClickListener(
                  listener -> getSupportFragmentManager()
                              .beginTransaction()
                              .replace(R.id.main_content, new CarFragment())
                              .commit()
        );

        ImageButton advancedImage = findViewById(R.id.advanced_settings);
        advancedImage.setOnClickListener(
                    listener -> getSupportFragmentManager()
                                .beginTransaction()
                                .replace(R.id.main_content, new AdvancedFragment())
                                .commit()
        );

        ImageButton videoImage = findViewById(R.id.video_settings);
        videoImage.setOnClickListener(
                listener -> getSupportFragmentManager()
                        .beginTransaction()
                        .replace(R.id.main_content, new VideoFragment())
                        .commit()
        );

        ImageButton keymapImage = findViewById(R.id.keymap_settings);
        keymapImage.setOnClickListener(
                listener -> getSupportFragmentManager()
                        .beginTransaction()
                        .replace(R.id.main_content, new KeymapFragment())
                        .commit()
        );

//        audioImage = findViewById(R.id.audio_settings);
//        audioImage.setOnClickListener(
//                listener -> getSupportFragmentManager()
//                        .beginTransaction()
//                        .replace(R.id.main_content, new AudioFragment())
//                        .commit()
//        );

        ImageButton connImage = findViewById(R.id.conn_settings);
        connImage.setOnClickListener(
                listener -> getSupportFragmentManager()
                        .beginTransaction()
                        .replace(R.id.main_content, new ConnectivityFragment())
                        .commit()
        );

        getSupportFragmentManager().beginTransaction().replace(R.id.main_content, new CarFragment()).commit();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (Log.isDebug()) Log.d(TAG, "onResume");
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (Log.isDebug()) Log.d(TAG, "onPause");
    }

    @Override
    public void onBackPressed() {
        Intent i = new Intent();
        setResult(Activity.RESULT_OK, i);
        super.onBackPressed();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (Log.isDebug()) Log.d(TAG, "onKeyDown: " + keyCode);
        return keyListener_ != null ? keyListener_.onKey(null, keyCode, event) : super.onKeyDown(keyCode, event);
    }

    @Override
    public void setOnKeyListener(View.OnKeyListener listener) {
        keyListener_ = listener;
    }

}
