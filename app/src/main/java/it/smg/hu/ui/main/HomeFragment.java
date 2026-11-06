package it.smg.hu.ui.main;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.fragment.app.Fragment;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.manager.ConnectionManager;
import it.smg.hu.manager.ConnectionState;
import it.smg.hu.manager.USBManager;
import it.smg.hu.manager.WIFIManager;
import it.smg.hu.service.ODAService;
import it.smg.hu.ui.MainActivity;
import it.smg.hu.ui.SettingsActivity;

/** The stationary connection dashboard. Starting a session always remains a deliberate action. */
public class HomeFragment extends Fragment {
    private View root_;
    private View statusPanel_;
    private TextView homeTitle_;
    private TextView homeSubtitle_;
    private TextView statusBadge_;
    private TextView statusTitle_;
    private TextView statusDetail_;
    private TextView usbStatus_;
    private TextView wifiStatus_;
    private ImageView indicator_;
    private Button retryButton_;
    private Button settingsButton_;
    private Button themeButton_;
    private Button exitButton_;
    private USBManager usbManager_;
    private WIFIManager wifiManager_;
    private LocalBroadcastManager broadcasts_;
    private ConnectionState renderedState_;
    private boolean darkTheme_;

    private final BroadcastReceiver receiver_ = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ConnectionManager.ACTION_STATE_CHANGED.equals(action)) {
                String name = intent.getStringExtra(ConnectionManager.EXTRA_STATE);
                ConnectionState state = name == null ? ConnectionState.IDLE : ConnectionState.valueOf(name);
                renderState(state, intent.getStringExtra(ConnectionManager.EXTRA_MESSAGE),
                        intent.getStringExtra(ConnectionManager.EXTRA_MODE));
            } else if (USBManager.ATTACH_AOAP_DEVICE.equals(action)) {
                usbStatus_.setText(R.string.home_usb_ready);
            } else if (USBManager.DETACH_AOAP_DEVICE.equals(action)) {
                usbStatus_.setText(R.string.home_usb_waiting);
            } else if (WIFIManager.CONNECT_WIFI.equals(action)) {
                String ssid = intent.getStringExtra(WIFIManager.EXTRA_SSID);
                wifiStatus_.setText(getString(R.string.home_wifi_ready, ssid == null ? "" : ssid));
            } else if (WIFIManager.DISCONNECT_WIFI.equals(action)) {
                renderWifiAvailability();
            }
        }
    };

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.home_fragment, container, false);
        usbManager_ = USBManager.instance();
        wifiManager_ = WIFIManager.instance();
        broadcasts_ = LocalBroadcastManager.getInstance(requireContext());

        root_ = view.findViewById(R.id.homeRoot);
        statusPanel_ = view.findViewById(R.id.statusPanel);
        homeTitle_ = view.findViewById(R.id.homeTitle);
        homeSubtitle_ = view.findViewById(R.id.homeSubtitle);
        statusBadge_ = view.findViewById(R.id.statusBadge);
        statusTitle_ = view.findViewById(R.id.statusTitle);
        statusDetail_ = view.findViewById(R.id.statusDetail);
        usbStatus_ = view.findViewById(R.id.usbStatus);
        wifiStatus_ = view.findViewById(R.id.wifiStatus);
        indicator_ = view.findViewById(R.id.connectionIndicator);
        retryButton_ = view.findViewById(R.id.retryBtn);
        settingsButton_ = view.findViewById(R.id.settingsBtn);
        themeButton_ = view.findViewById(R.id.themeBtn);
        exitButton_ = view.findViewById(R.id.exitBtn);

        settingsButton_.setOnClickListener(v ->
                startActivityForResult(new Intent(getContext(), SettingsActivity.class), MainActivity.SETTINGS_ACTIVITY_REQUEST));
        themeButton_.setOnClickListener(v -> toggleTheme());
        exitButton_.setOnClickListener(v -> ((MainActivity) requireActivity()).exitApplication());
        retryButton_.setOnClickListener(v -> retryConnection());

        applyTheme();
        renderWifiAvailability();
        renderState(ConnectionManager.instance().state(), ConnectionManager.instance().lastMessage(),
                ConnectionManager.instance().mode());
        return view;
    }

    @Override
    public void onStart() {
        super.onStart();
        IntentFilter filter = new IntentFilter();
        filter.addAction(ConnectionManager.ACTION_STATE_CHANGED);
        filter.addAction(USBManager.ATTACH_AOAP_DEVICE);
        filter.addAction(USBManager.DETACH_AOAP_DEVICE);
        filter.addAction(WIFIManager.CONNECT_WIFI);
        filter.addAction(WIFIManager.DISCONNECT_WIFI);
        broadcasts_.registerReceiver(receiver_, filter);
        // State changes can happen while PlayerActivity covers this fragment.
        // Render the current snapshot on return so DISCONNECTING and its
        // completion are visible even if their broadcasts were missed.
        renderWifiAvailability();
        renderState(ConnectionManager.instance().state(), ConnectionManager.instance().lastMessage(),
                ConnectionManager.instance().mode());
    }

    @Override
    public void onStop() {
        broadcasts_.unregisterReceiver(receiver_);
        super.onStop();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == MainActivity.SETTINGS_ACTIVITY_REQUEST) {
            applyTheme();
            renderWifiAvailability();
            if (wifiManager_ != null) {
                wifiManager_.checkNetwork();
            }
        }
    }

    private void retryConnection() {
        String mode = ConnectionManager.instance().mode();
        if (ODAService.MODE_USB.equals(mode) && usbManager_ != null
                && (renderedState_ == ConnectionState.ERROR || usbManager_.aoapDevice() == null)) {
            // Recovery only prepares a fresh USB transport.  Starting a new
            // PlayerActivity before that preparation completes races the prior
            // TLS teardown and produces SSL_READ(5).
            usbManager_.recoverConnection();
        } else if (renderedState_ == ConnectionState.ERROR && mode != null) {
            // Move a Wi-Fi retry back to a startable state explicitly. ERROR is
            // otherwise blocked by ConnectionPolicy to prevent accidental
            // overlapping sessions from widgets or duplicate taps.
            ConnectionManager.instance().transportAvailable(mode,
                    ConnectionManager.instance().lastMessage());
            ((MainActivity) requireActivity()).startConnectionManually(mode);
        } else if (mode != null && (!ODAService.MODE_USB.equals(mode)
                || (usbManager_ != null && usbManager_.aoapDevice() != null))) {
            ((MainActivity) requireActivity()).startConnectionManually(mode);
        }
    }

    private void renderWifiAvailability() {
        if (Settings.instance().advanced.enableWiFi()) {
            wifiStatus_.setText(R.string.home_wifi_waiting);
        } else {
            wifiStatus_.setText(R.string.home_wifi_off);
        }
    }

    private void toggleTheme() {
        Settings.instance().appearance.darkTheme(!Settings.instance().appearance.darkTheme());
        applyTheme();
        renderState(ConnectionManager.instance().state(), ConnectionManager.instance().lastMessage(),
                ConnectionManager.instance().mode());
    }

    private void applyTheme() {
        darkTheme_ = Settings.instance().appearance.darkTheme();
        int surfaceColor = darkTheme_ ? R.color.oda_surface : R.color.home_light_surface;
        int primaryColor = darkTheme_ ? R.color.oda_text_primary : R.color.home_light_text_primary;
        int secondaryColor = darkTheme_ ? R.color.oda_text_secondary : R.color.home_light_text_secondary;
        int secondaryButton = darkTheme_ ? R.drawable.button_secondary : R.drawable.button_secondary_light;

        root_.setBackgroundColor(getResources().getColor(surfaceColor));
        statusPanel_.setBackgroundResource(darkTheme_ ? R.drawable.panel_surface : R.drawable.panel_surface_light);
        homeTitle_.setTextColor(getResources().getColor(primaryColor));
        homeSubtitle_.setTextColor(getResources().getColor(secondaryColor));
        statusTitle_.setTextColor(getResources().getColor(primaryColor));
        statusDetail_.setTextColor(getResources().getColor(secondaryColor));
        usbStatus_.setTextColor(getResources().getColor(secondaryColor));
        wifiStatus_.setTextColor(getResources().getColor(secondaryColor));
        usbStatus_.setBackgroundResource(darkTheme_ ? R.drawable.status_badge_idle : R.drawable.status_badge_idle_light);
        wifiStatus_.setBackgroundResource(darkTheme_ ? R.drawable.status_badge_idle : R.drawable.status_badge_idle_light);

        styleSecondaryButton(settingsButton_, secondaryButton, primaryColor);
        styleSecondaryButton(themeButton_, secondaryButton, primaryColor);
        styleSecondaryButton(exitButton_, secondaryButton, primaryColor);
        themeButton_.setText(darkTheme_ ? R.string.theme_dark : R.string.theme_light);
        themeButton_.setSelected(darkTheme_);
    }

    private void styleSecondaryButton(Button button, int background, int textColor) {
        button.setBackgroundResource(background);
        button.setTextColor(getResources().getColor(textColor));
    }

    private void renderState(ConnectionState state, String message, String mode) {
        renderedState_ = state;
        boolean loading = state == ConnectionState.PERMISSION_PENDING
                || state == ConnectionState.AOAP_SWITCHING || state == ConnectionState.CONNECTING
                || state == ConnectionState.DISCONNECTING;
        boolean manualStartAvailable = state != ConnectionState.ERROR && mode != null
                && ConnectionManager.instance().isManualStartAllowed();
        retryButton_.setVisibility(state == ConnectionState.ERROR || manualStartAvailable || loading
                ? View.VISIBLE : View.GONE);
        retryButton_.setEnabled(!loading);
        retryButton_.setText(loading ? R.string.home_loading
                : (state == ConnectionState.ERROR && ODAService.MODE_USB.equals(mode)
                ? R.string.home_recover_usb : (state == ConnectionState.ERROR ? R.string.home_retry : R.string.home_start)));
        String detail = state == ConnectionState.ERROR ? message : null;
        int badge = darkTheme_ ? R.drawable.status_badge_idle : R.drawable.status_badge_idle_light;
        int badgeTextColor = darkTheme_ ? R.color.oda_text_primary : R.color.home_light_text_primary;
        int icon = ODAService.MODE_WIFI.equals(mode) ? R.drawable.wifi_dis : R.drawable.usb_dis;

        switch (state) {
            case PERMISSION_PENDING:
                statusBadge_.setText(R.string.home_status_permission);
                statusTitle_.setText(R.string.home_permission_title);
                detail = fallback(detail, R.string.home_permission_detail);
                break;
            case AOAP_SWITCHING:
                statusBadge_.setText(R.string.home_status_connecting);
                statusTitle_.setText(R.string.home_connecting_title);
                detail = fallback(detail, R.string.home_aoap_detail);
                break;
            case CONNECTING:
                statusBadge_.setText(R.string.home_status_connecting);
                statusTitle_.setText(R.string.home_connecting_title);
                detail = fallback(detail, R.string.home_connecting_detail);
                break;
            case DISCONNECTING:
                statusBadge_.setText(R.string.home_status_stopping);
                statusTitle_.setText(R.string.home_stopping_title);
                detail = fallback(detail, R.string.home_stopping_detail);
                break;
            case ACTIVE:
                badge = R.drawable.status_badge_active;
                badgeTextColor = R.color.oda_text_primary;
                icon = ODAService.MODE_WIFI.equals(mode) ? R.drawable.wifi : R.drawable.usb;
                statusBadge_.setText(R.string.home_status_connected);
                statusTitle_.setText(R.string.home_connected_title);
                detail = fallback(detail, R.string.home_connected_detail);
                break;
            case EXITED:
                statusBadge_.setText(R.string.home_status_stopped);
                statusTitle_.setText(R.string.home_stopped_title);
                detail = fallback(detail, R.string.home_stopped_detail);
                break;
            case ERROR:
                badge = R.drawable.status_badge_error;
                badgeTextColor = R.color.oda_text_primary;
                statusBadge_.setText(R.string.home_status_error);
                statusTitle_.setText(R.string.home_error_title);
                detail = fallback(detail, R.string.home_error_detail);
                break;
            case IDLE:
            default:
                statusBadge_.setText(manualStartAvailable
                        ? R.string.home_status_ready : R.string.home_status_waiting);
                statusTitle_.setText(manualStartAvailable
                        ? R.string.home_ready_title : R.string.home_waiting_title);
                detail = fallback(detail, manualStartAvailable
                        ? R.string.home_ready_detail : R.string.home_waiting_detail);
                break;
        }
        statusBadge_.setBackgroundResource(badge);
        statusBadge_.setTextColor(getResources().getColor(badgeTextColor));
        indicator_.setImageResource(icon);
        statusDetail_.setText(detail);
        if (ODAService.MODE_USB.equals(mode) && usbManager_ != null && usbManager_.aoapDevice() != null) {
            usbStatus_.setText(R.string.home_usb_ready);
        }
    }

    private String fallback(String detail, int fallbackResource) {
        return detail == null || detail.trim().isEmpty() ? getString(fallbackResource) : detail;
    }

}
