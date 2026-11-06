package it.smg.hu.ui;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.projection.InputDevice;
import it.smg.hu.ui.settings.AdvancedFragment;
import it.smg.hu.ui.settings.CarFragment;
import it.smg.hu.ui.settings.ConnectivityFragment;
import it.smg.hu.ui.settings.KeymapFragment;
import it.smg.hu.ui.settings.VideoFragment;
import it.smg.libs.common.Log;

public class SettingsActivity extends FragmentActivity implements InputDevice.OnKeyHolder {
    private static final String TAG = "SettingsActivity";
    private static final String SELECTED_NAVIGATION_ID = "selectedNavigationId";
    private static final int[] NAVIGATION_IDS = {
            R.id.car_settings, R.id.video_settings, R.id.conn_settings,
            R.id.keymap_settings, R.id.advanced_settings
    };
    private View.OnKeyListener keyListener_;
    private int selectedNavigationId_ = R.id.car_settings;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        findViewById(R.id.settingsBackBtn).setOnClickListener(v -> onBackPressed());
        findViewById(R.id.settingsThemeBtn).setOnClickListener(v -> {
            v.setEnabled(false);
            Settings settings = Settings.instance();
            settings.appearance.darkTheme(!settings.appearance.darkTheme());
            applyTheme();
            showSettings(fragmentForNavigation(selectedNavigationId_),
                    titleForNavigation(selectedNavigationId_), selectedNavigationId_);
            v.postDelayed(() -> v.setEnabled(true), 350);
        });
        findViewById(R.id.car_settings).setOnClickListener(v -> showSettings(new CarFragment(), R.string.car_settings, R.id.car_settings));
        findViewById(R.id.video_settings).setOnClickListener(v -> showSettings(new VideoFragment(), R.string.video_settings, R.id.video_settings));
        findViewById(R.id.conn_settings).setOnClickListener(v -> showSettings(new ConnectivityFragment(), R.string.conn_settings, R.id.conn_settings));
        findViewById(R.id.keymap_settings).setOnClickListener(v -> showSettings(new KeymapFragment(), R.string.keymap_settings, R.id.keymap_settings));
        findViewById(R.id.advanced_settings).setOnClickListener(v -> showSettings(new AdvancedFragment(), R.string.advanced_settings, R.id.advanced_settings));

        if (savedInstanceState != null) {
            selectedNavigationId_ = savedInstanceState.getInt(SELECTED_NAVIGATION_ID, R.id.car_settings);
        }
        applyTheme();
        if (savedInstanceState == null) {
            showSettings(new CarFragment(), R.string.car_settings, R.id.car_settings);
        } else {
            ((TextView) findViewById(R.id.settingsTitle)).setText(titleForNavigation(selectedNavigationId_));
            renderNavigation();
        }
    }

    private void showSettings(Fragment fragment, int titleResource, int selectedNavigationId) {
        selectedNavigationId_ = selectedNavigationId;
        ((TextView) findViewById(R.id.settingsTitle)).setText(titleResource);
        renderNavigation();
        findViewById(R.id.settingsScroll).scrollTo(0, 0);
        getSupportFragmentManager().beginTransaction()
                .replace(R.id.main_content, fragment)
                .commit();
    }

    private void applyTheme() {
        boolean dark = Settings.instance().appearance.darkTheme();
        int surfaceColor = dark ? R.color.oda_surface : R.color.settings_surface;
        int elevatedColor = dark ? R.color.oda_surface_elevated : R.color.home_light_surface_elevated;
        int primaryColor = dark ? R.color.oda_text_primary : R.color.settings_text_primary;
        int secondaryColor = dark ? R.color.oda_text_secondary : R.color.settings_text_secondary;
        int secondaryButton = dark ? R.drawable.button_secondary : R.drawable.button_secondary_light;

        findViewById(R.id.settingsRoot).setBackgroundColor(getResources().getColor(surfaceColor));
        findViewById(R.id.settingsHeader).setBackgroundColor(getResources().getColor(elevatedColor));
        findViewById(R.id.settingsScroll).setBackgroundColor(getResources().getColor(surfaceColor));
        findViewById(R.id.main_content).setBackgroundColor(getResources().getColor(surfaceColor));
        findViewById(R.id.settingsNav).setBackgroundColor(getResources().getColor(elevatedColor));
        ((TextView) findViewById(R.id.settingsTitle)).setTextColor(getResources().getColor(primaryColor));
        ((TextView) findViewById(R.id.settingsSubtitle)).setTextColor(getResources().getColor(secondaryColor));

        Button backButton = findViewById(R.id.settingsBackBtn);
        backButton.setBackgroundResource(secondaryButton);
        backButton.setTextColor(getResources().getColor(primaryColor));
        Button themeButton = findViewById(R.id.settingsThemeBtn);
        themeButton.setBackgroundResource(secondaryButton);
        themeButton.setTextColor(getResources().getColor(primaryColor));
        themeButton.setText(dark ? R.string.theme_dark : R.string.theme_light);
        themeButton.setSelected(dark);
        renderNavigation();
    }

    private void renderNavigation() {
        boolean dark = Settings.instance().appearance.darkTheme();
        int unselectedBackground = dark ? R.drawable.button_secondary : R.drawable.button_secondary_light;
        int unselectedTint = dark ? R.color.oda_text_primary : R.color.settings_text_primary;
        for (int id : NAVIGATION_IDS) {
            boolean selected = id == selectedNavigationId_;
            ImageButton button = findViewById(id);
            button.setBackgroundResource(selected ? R.drawable.button_primary : unselectedBackground);
            button.setColorFilter(getResources().getColor(selected ? R.color.oda_on_accent : unselectedTint));
            button.setSelected(selected);
        }
    }

    private int titleForNavigation(int navigationId) {
        if (navigationId == R.id.video_settings) return R.string.video_settings;
        if (navigationId == R.id.conn_settings) return R.string.conn_settings;
        if (navigationId == R.id.keymap_settings) return R.string.keymap_settings;
        if (navigationId == R.id.advanced_settings) return R.string.advanced_settings;
        return R.string.car_settings;
    }

    private Fragment fragmentForNavigation(int navigationId) {
        if (navigationId == R.id.video_settings) return new VideoFragment();
        if (navigationId == R.id.conn_settings) return new ConnectivityFragment();
        if (navigationId == R.id.keymap_settings) return new KeymapFragment();
        if (navigationId == R.id.advanced_settings) return new AdvancedFragment();
        return new CarFragment();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        outState.putInt(SELECTED_NAVIGATION_ID, selectedNavigationId_);
        super.onSaveInstanceState(outState);
    }

    @Override
    public void onBackPressed() {
        setResult(Activity.RESULT_OK, new Intent());
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
