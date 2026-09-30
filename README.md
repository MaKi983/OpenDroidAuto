# OpenDroidAuto

Android Auto receiver for legacy Android ARM32 head units, including Honda Connect units and compatible generic multimedia systems.

Current hotfix version: **2.6.2**.

## Connection behavior

- USB is the primary path. When the phone reaches AOAP mode, use **START ANDROID AUTO** on the dashboard to begin the session.
- If the USB stack becomes stale, use **RECOVER USB**. It closes the stale handle, rescans the bus and recreates the AOAP device without resetting the physical USB port.
- Selecting **Exit** stops the session and keeps the dashboard open. The same cable can be used to press Start again.
- Wi-Fi is optional. Enable it in settings only after the phone hotspot is already saved by Android on the head unit. The app uses the hotspot gateway on TCP port 5277 and reports failures for manual recovery.
- The dashboard and Settings support a persistent light/dark selector. A fresh installation starts in light mode; Portuguese multimedia systems use Portuguese interface text and every other locale falls back to English.
- Android controls USB permission. A non-rooted head unit can still show the system dialog on a first connection or after a device is disconnected; the app does not bypass that permission.
- Version 2.6.1 introduces a stable release certificate. Uninstall the older debug-signed APK once before installing 2.6.1; future stable-signed updates can then preserve the app identity and USB default association.
- Version 2.6.2 restores the proven legacy projection lifecycle, adds bounded native USB transfer retries and simplifies the dashboard/Settings layout for the Honda display viewport.
- Stable release certificate SHA-256: `DD:F4:9D:72:6D:DB:B5:A7:FF:5F:EA:21:9D:AA:27:66:69:94:E2:F3:37:F7:E3:38:EE:CD:F9:C4:AB:B7:3C:B6`.

## Compatibility

- Android 4.x and `armeabi-v7a` remain the supported baseline.
- Honda integration is used only when Fujitsu/Honda system services are present. Generic head units fall back to standard Android audio and input behavior.
- Android Auto Wi-Fi Direct is not implemented. Use a saved phone hotspot.

## Build

Install JDK 17, Android SDK platform 34, and NDK `17.2.4988734`, then run:

```sh
./gradlew test
./gradlew assembleDebugAndroidTest
./gradlew assembleRelease
```

The APK is created at `app/build/outputs/apk/release/HondaAppCenter_A1.apk`.

For a production-signed APK, set these environment variables before the release build:

```text
ODA_RELEASE_STORE_FILE
ODA_RELEASE_STORE_PASSWORD
ODA_RELEASE_KEY_ALIAS
ODA_RELEASE_KEY_PASSWORD
```

Without them, the legacy debug-signing behavior is retained for local installation.
GitHub Actions decodes `ODA_RELEASE_KEYSTORE_BASE64` and supplies the other three signing values from repository secrets. Keep an offline backup of the release keystore and password; losing them prevents future in-place APK updates.

## Diagnostics

The home dashboard reports permission, AOAP switching, connection and native error details. Enable external logs only when diagnosing a hardware problem; protocol logging can contain sensitive connection data.

## License

GPL-3.0. See [LICENSE](LICENSE).
