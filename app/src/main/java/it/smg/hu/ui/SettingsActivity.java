package it.smg.hu.ui;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageButton;

import androidx.fragment.app.FragmentActivity;

import it.smg.hu.R;
import it.smg.hu.ui.settings.AdvancedFragment;
import it.smg.hu.ui.settings.AudioFragment;
import it.smg.hu.ui.settings.CarFragment;
import it.smg.hu.ui.settings.VideoFragment;

public class SettingsActivity extends FragmentActivity {

//    private ImageButton keymapImage;
    private ImageButton carImage;
    private ImageButton advancedImage;
    private ImageButton videoImage;
    private ImageButton audioImage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_settings);

        carImage = findViewById(R.id.car_settings);
        carImage.setOnClickListener(
                  listener -> getSupportFragmentManager()
                              .beginTransaction()
                              .replace(R.id.main_content, new CarFragment())
                              .commit()
        );

        advancedImage = findViewById(R.id.advanced_settings);
        advancedImage.setOnClickListener(
                    listener -> getSupportFragmentManager()
                                .beginTransaction()
                                .replace(R.id.main_content, new AdvancedFragment())
                                .commit()
        );

        videoImage = findViewById(R.id.video_settings);
        videoImage.setOnClickListener(
                listener -> getSupportFragmentManager()
                        .beginTransaction()
                        .replace(R.id.main_content, new VideoFragment())
                        .commit()
        );

        audioImage = findViewById(R.id.audio_settings);
        audioImage.setOnClickListener(
                listener -> getSupportFragmentManager()
                        .beginTransaction()
                        .replace(R.id.main_content, new AudioFragment())
                        .commit()
        );

        getSupportFragmentManager().beginTransaction().replace(R.id.main_content, new CarFragment()).commit();
    }

    @Override
    public void onBackPressed() {
        Intent i = new Intent();
        setResult(Activity.RESULT_OK, i);
        super.onBackPressed();
    }
}
