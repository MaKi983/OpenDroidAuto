package it.smg.hu.manager;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import it.smg.hu.R;
import it.smg.libs.common.Log;
import it.smg.libs.aasdk.usb.LibUsb;
import it.smg.libs.aasdk.usb.LibUsbDevice;


public class USBManager {

    private static final int ACC_REQ_GET_PROTOCOL = 51;
    private static final int ACC_REQ_SEND_STRING  = 52;
    private static final int ACC_REQ_START        = 53;
    private static final int AOAP_TRANSFER_TIMEOUT_MS = 3000;

    private static final String MANUFACTURER = "Android";
    private static final String MODEL = "Android Auto";
    private static final String DESC = "Android Auto";
    private static final String VERSION = "2.0.1";
    private static final String URI = "https://forum.xda-developers.com/t/honda-connect-android-system.3179549";
    private static final String SERIAL = "HU-AAAAAA001";

    public static final String ACTION_USB_PERMISSION =  "it.smg.hu.USB_PERMISSION";
    public static final String DETACH_AOAP_DEVICE = "it.smg.hu.DETACH_AOAP_DEVICE";
    public static final String ATTACH_AOAP_DEVICE = "it.smg.hu.ATTACH_AOAP_DEVICE";

    private static final String TAG = "USBManager";

    private final Context ctx_;
    private final UsbManager usbManager_;
    private final Object deviceLock_ = new Object();
    private final ExecutorService usbExecutor_;
    private final Handler mainHandler_;
    private volatile LibUsbDevice usbDevice_;
    private volatile UsbDevice lastDevice_;
    private int attachedDeviceId_ = -1;
    private int pendingPermissionDeviceId_ = -1;
    private int aoapSourceDeviceId_ = -1;
    private int aoapOperationId_;
    private boolean aoapSwitching_;
    private final LocalBroadcastManager localBroadcastManager_;

    private static USBManager instance_;
    private LibUsb libUsb_;

    public static void init(Context ctx){
        instance_ = new USBManager(ctx);
    }

    public static USBManager instance(){
        return instance_;
    }

    private USBManager(Context ctx){
        ctx_ = ctx.getApplicationContext();
        usbManager_ = (UsbManager) ctx_.getSystemService(Context.USB_SERVICE);
        localBroadcastManager_ = LocalBroadcastManager.getInstance(ctx_);
        mainHandler_ = new Handler(Looper.getMainLooper());
        usbExecutor_ = Executors.newSingleThreadExecutor(runnable -> {
            Thread thread = new Thread(runnable, "ODA-USB-worker");
            thread.setDaemon(true);
            return thread;
        });
        LibUsb.init();
        libUsb_ = LibUsb.instance();
        ConnectionManager.init(ctx_);
    }

    public boolean checkDevice(UsbDevice device){
        handleUsbAttached(device);
        return checkAOAPDevice(device);
    }

    /**
     * Records a newly attached device without opening Android's permission UI.
     * Permission is requested only after the user presses the USB action on the
     * dashboard, avoiding the legacy UsbConfirmActivity/UsbPermissionActivity
     * overlap that force-stops the selected package.
     */
    public void onUsbDeviceDetected(UsbDevice device) {
        if (device == null) {
            return;
        }
        if (usbManager_.hasPermission(device)) {
            handleUsbAttached(device);
            return;
        }
        if (!UsbAccessoryIds.isAoap(device.getVendorId(), device.getProductId())
                && !UsbAccessoryIds.maybeSupportsAoap(device)) {
            return;
        }
        synchronized (deviceLock_) {
            lastDevice_ = device;
        }
        ConnectionManager.instance().transportAvailable("modeUSB",
                ctx_.getString(R.string.connection_usb_detected));
    }

    public void handleUsbAttached(UsbDevice device){
        if (device == null) {
            return;
        }
        if (Log.isInfo()) Log.i(TAG, "handleUsbAttached " + device);
        boolean aoapDevice = checkAOAPDevice(device);
        synchronized (deviceLock_) {
            if (!aoapDevice && ((aoapSwitching_ && aoapSourceDeviceId_ != device.getDeviceId())
                    || (usbDevice_ != null && attachedDeviceId_ != device.getDeviceId()))) {
                if (Log.isInfo()) Log.i(TAG, "Ignoring unrelated USB device while Android Auto USB is active");
                return;
            }
            lastDevice_ = device;
        }

        boolean deviceHasPermission = usbManager_.hasPermission(device);
        if (Log.isInfo()) Log.i(TAG, "deviceHasPermission " + deviceHasPermission);
        if (!deviceHasPermission) {
            requestPermission(device);
            return;
        }

        if (aoapDevice) {
            attachAoapDevice(device);
            return;
        }

        synchronized (deviceLock_) {
            if (aoapSwitching_ && aoapSourceDeviceId_ == device.getDeviceId()) {
                return;
            }
        }

        if (!UsbAccessoryIds.maybeSupportsAoap(device)) {
            // Storage sticks, hubs and the like share this port. Negotiating AOAP
            // with them can only fail, and reporting that as a connection error
            // buried the real state of the Android Auto link.
            if (Log.isInfo()) Log.i(TAG, "Skipping AOAP switch for non-phone device " + device.getDeviceName());
            return;
        }

        ConnectionManager.instance().transportAvailable("modeUSB", ctx_.getString(R.string.connection_usb_detected));
        beginAoapSwitch(device);
    }

    public void onUsbPermissionResult(UsbDevice device, boolean granted) {
        synchronized (deviceLock_) {
            if (device == null || pendingPermissionDeviceId_ == device.getDeviceId()) {
                pendingPermissionDeviceId_ = -1;
            }
        }
        if (device == null) {
            ConnectionManager.instance().failed(ctx_.getString(R.string.connection_usb_permission_device_error));
            return;
        }
        if (!granted) {
            ConnectionManager.instance().failed(ctx_.getString(R.string.connection_usb_permission_denied));
            return;
        }
        handleUsbAttached(device);
    }

    public void onUsbDetached(UsbDevice device) {
        if (device == null) {
            return;
        }

        boolean expectedAoapReenumeration = false;
        boolean activeDeviceDetached = false;
        synchronized (deviceLock_) {
            int deviceId = device.getDeviceId();
            if (pendingPermissionDeviceId_ == deviceId) {
                pendingPermissionDeviceId_ = -1;
            }
            if (aoapSwitching_ && aoapSourceDeviceId_ == deviceId) {
                expectedAoapReenumeration = true;
                if (lastDevice_ != null && lastDevice_.getDeviceId() == deviceId) {
                    lastDevice_ = null;
                }
            } else if (attachedDeviceId_ == deviceId
                    || (lastDevice_ != null && lastDevice_.getDeviceId() == deviceId)) {
                invalidateAoapOperationLocked();
                closeUsbDeviceLocked();
                lastDevice_ = null;
                activeDeviceDetached = true;
            }
        }

        if (expectedAoapReenumeration) {
            if (Log.isInfo()) Log.i(TAG, "USB source detached while waiting for AOAP re-enumeration");
            return;
        }
        if (activeDeviceDetached) {
            ConnectionManager.instance().detached("modeUSB", ctx_.getString(R.string.connection_usb_disconnected));
            localBroadcastManager_.sendBroadcast(new Intent(DETACH_AOAP_DEVICE));
        }
    }

    private void requestPermission(UsbDevice device) {
        int deviceId = device.getDeviceId();
        synchronized (deviceLock_) {
            if (pendingPermissionDeviceId_ == deviceId) {
                return;
            }
            pendingPermissionDeviceId_ = deviceId;
        }

        if (Log.isDebug()) Log.d(TAG, "Request permission for USB device " + deviceId);
        Intent permissionIntent = new Intent(ctx_, UsbPermissionReceiver.class);
        permissionIntent.setAction(ACTION_USB_PERMISSION);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(ctx_, deviceId, permissionIntent,
                PendingIntent.FLAG_ONE_SHOT);
        ConnectionManager.instance().permissionPending(ctx_.getString(R.string.connection_usb_permission));
        try {
            usbManager_.requestPermission(device, pendingIntent);
        } catch (RuntimeException error) {
            synchronized (deviceLock_) {
                if (pendingPermissionDeviceId_ == deviceId) {
                    pendingPermissionDeviceId_ = -1;
                }
            }
            Log.e(TAG, "Unable to request USB permission", error);
            ConnectionManager.instance().failed(ctx_.getString(R.string.connection_usb_permission_device_error));
        }
    }

    private void beginAoapSwitch(UsbDevice device) {
        final int operationId;
        final int sourceDeviceId = device.getDeviceId();
        synchronized (deviceLock_) {
            if (aoapSwitching_ && aoapSourceDeviceId_ == sourceDeviceId) {
                return;
            }
            invalidateAoapOperationLocked();
            aoapSwitching_ = true;
            aoapSourceDeviceId_ = sourceDeviceId;
            operationId = aoapOperationId_;
        }

        ConnectionManager.instance().switchingToAoap(ctx_.getString(R.string.connection_usb_preparing));
        usbExecutor_.execute(() -> negotiateAoap(operationId, sourceDeviceId));
    }

    private void negotiateAoap(int operationId, int sourceDeviceId) {
        try {
            for (int attempt = 0; attempt < UsbRetryPolicy.NEGOTIATION_ATTEMPTS; attempt++) {
                if (!isAoapOperationCurrent(operationId)) {
                    return;
                }
                UsbDevice device = findDeviceById(sourceDeviceId);
                if (device == null) {
                    break;
                }
                if (checkAOAPDevice(device)) {
                    mainHandler_.post(() -> handleUsbAttached(device));
                    return;
                }
                if (requestAOAP(device)) {
                    break;
                }
                if (attempt + 1 < UsbRetryPolicy.NEGOTIATION_ATTEMPTS) {
                    SystemClock.sleep(UsbRetryPolicy.negotiationDelayMs(attempt));
                }
            }
        } catch (Throwable error) {
            Log.e(TAG, "AOAP negotiation failed", error);
            mainHandler_.post(() -> ConnectionManager.instance().failed(ctx_.getString(R.string.connection_usb_switch_error)));
            return;
        }
        scheduleAoapScan(operationId, 0);
    }

    private void scheduleAoapScan(int operationId, int scanAttempt) {
        if (!isAoapOperationCurrent(operationId)) {
            return;
        }
        mainHandler_.postDelayed(() -> scanForAoapDevice(operationId, scanAttempt),
                UsbRetryPolicy.reenumerationDelayMs(scanAttempt));
    }

    private void scanForAoapDevice(int operationId, int scanAttempt) {
        if (!isAoapOperationCurrent(operationId)) {
            return;
        }

        UsbDevice accessory = findAoapDevice();
        if (accessory != null) {
            synchronized (deviceLock_) {
                if (!isAoapOperationCurrentLocked(operationId)) {
                    return;
                }
                invalidateAoapOperationLocked();
            }
            handleUsbAttached(accessory);
            return;
        }

        if (scanAttempt + 1 < UsbRetryPolicy.REENUMERATION_ATTEMPTS) {
            scheduleAoapScan(operationId, scanAttempt + 1);
            return;
        }

        synchronized (deviceLock_) {
            if (!isAoapOperationCurrentLocked(operationId)) {
                return;
            }
            invalidateAoapOperationLocked();
        }
        ConnectionManager.instance().failed(ctx_.getString(R.string.connection_usb_switch_error));
    }

    private void attachAoapDevice(UsbDevice device) {
        LibUsbDevice attachedDevice;
        synchronized (deviceLock_) {
            if (attachedDeviceId_ == device.getDeviceId() && usbDevice_ != null) {
                return;
            }
            invalidateAoapOperationLocked();
            closeUsbDeviceLocked();
            lastDevice_ = device;
            pendingPermissionDeviceId_ = -1;
            attachedDevice = libUsb_.createDevice(device, usbManager_);
            usbDevice_ = attachedDevice;
            attachedDeviceId_ = attachedDevice == null ? -1 : device.getDeviceId();
        }

        if (attachedDevice == null) {
            ConnectionManager.instance().failed(ctx_.getString(R.string.connection_usb_switch_error));
            return;
        }
        ConnectionManager.instance().transportAvailable("modeUSB", ctx_.getString(R.string.connection_usb_ready));
        Intent aoapDeviceIntent = new Intent(ATTACH_AOAP_DEVICE);
        aoapDeviceIntent.putExtra(UsbManager.EXTRA_DEVICE, device);
        localBroadcastManager_.sendBroadcast(aoapDeviceIntent);
    }

    private UsbDevice findDeviceById(int deviceId) {
        for (UsbDevice device : usbManager_.getDeviceList().values()) {
            if (device.getDeviceId() == deviceId) {
                return device;
            }
        }
        return null;
    }

    private UsbDevice findAoapDevice() {
        for (UsbDevice device : usbManager_.getDeviceList().values()) {
            if (checkAOAPDevice(device)) {
                return device;
            }
        }
        return null;
    }

    private UsbDevice findPermittedAoapDevice() {
        for (UsbDevice device : usbManager_.getDeviceList().values()) {
            if (usbManager_.hasPermission(device) && checkAOAPDevice(device)) {
                return device;
            }
        }
        return null;
    }

    private boolean isAoapOperationCurrent(int operationId) {
        synchronized (deviceLock_) {
            return isAoapOperationCurrentLocked(operationId);
        }
    }

    private boolean isAoapOperationCurrentLocked(int operationId) {
        return aoapSwitching_ && aoapOperationId_ == operationId;
    }

    private void invalidateAoapOperationLocked() {
        aoapOperationId_++;
        aoapSwitching_ = false;
        aoapSourceDeviceId_ = -1;
    }

    private void closeUsbDeviceLocked() {
        LibUsbDevice device = usbDevice_;
        usbDevice_ = null;
        attachedDeviceId_ = -1;
        if (device == null) {
            return;
        }
        try {
            device.close();
        } catch (Throwable error) {
            Log.e(TAG, "Error closing stale USB device", error);
        }
    }

    /** Resets a session after its transfers have stopped but before it is deleted. */
    public void resetSession(LibUsbDevice sessionDevice) {
        if (sessionDevice == null) {
            return;
        }
        try {
            sessionDevice.reset();
        } catch (Throwable error) {
            Log.e(TAG, "Error resetting completed USB session", error);
        }
    }

    /**
     * Discards a completed transport. Delayed scans create a fresh wrapper only
     * after Android has processed a possible reset-driven re-enumeration.
     */
    public void finishSession(LibUsbDevice sessionDevice) {
        if (sessionDevice == null) {
            return;
        }
        synchronized (deviceLock_) {
            if (usbDevice_ == sessionDevice) {
                closeUsbDeviceLocked();
                lastDevice_ = null;
                pendingPermissionDeviceId_ = -1;
            }
        }
        mainHandler_.postDelayed(this::rescanAttachedDevices, 750L);
        mainHandler_.postDelayed(this::rescanAttachedDevices, 1500L);
    }

    public synchronized boolean recoverConnection() {
        if (Log.isInfo()) Log.i(TAG, "recoverConnection");
        // Publish the busy state before touching the device.  The dashboard
        // keeps its action disabled until a fresh AOAP accessory is attached.
        ConnectionManager.instance().switchingToAoap(ctx_.getString(R.string.connection_usb_preparing));
        synchronized (deviceLock_) {
            invalidateAoapOperationLocked();
            closeUsbDeviceLocked();
            lastDevice_ = null;
            pendingPermissionDeviceId_ = -1;
        }

        UsbDevice fallbackDevice = null;
        for (UsbDevice device : usbManager_.getDeviceList().values()) {
            if (checkAOAPDevice(device) && usbManager_.hasPermission(device)) {
                attachAoapDevice(device);
                return true;
            }
            if (fallbackDevice == null && UsbAccessoryIds.maybeSupportsAoap(device)) {
                fallbackDevice = device;
            }
        }

        if (fallbackDevice != null) {
            handleUsbAttached(fallbackDevice);
            return true;
        }
        ConnectionManager.instance().failed(ctx_.getString(R.string.connection_usb_unavailable));
        return false;
    }

    public LibUsbDevice aoapDevice(){
        return usbDevice_;
    }

    /** Recovers a missed attach broadcast without starting the Android Auto session. */
    public boolean rescanAttachedDevices() {
        UsbDevice accessory = findPermittedAoapDevice();
        if (accessory != null) {
            handleUsbAttached(accessory);
            return true;
        }

        UsbDevice previous = lastDevice_ == null ? null : findDeviceById(lastDevice_.getDeviceId());
        if (previous != null && usbManager_.hasPermission(previous)) {
            handleUsbAttached(previous);
            return true;
        }

        UsbDevice onlyDevice = null;
        for (UsbDevice device : usbManager_.getDeviceList().values()) {
            if (!UsbAccessoryIds.isAoap(device.getVendorId(), device.getProductId())
                    && !UsbAccessoryIds.maybeSupportsAoap(device)) {
                continue;
            }
            if (onlyDevice != null) {
                return false;
            }
            onlyDevice = device;
        }
        if (onlyDevice != null) {
            onUsbDeviceDetected(onlyDevice);
            return true;
        }
        return false;
    }

    /** Opens the current accessory with bounded recovery; call only from a worker thread. */
    public LibUsbDevice openAoapDeviceWithRetry() {
        for (int attempt = 0; attempt < UsbRetryPolicy.NEGOTIATION_ATTEMPTS; attempt++) {
            LibUsbDevice device = aoapDevice();
            if (device != null) {
                try {
                    if (device.open()) {
                        return device;
                    }
                } catch (Throwable error) {
                    Log.e(TAG, "Error opening AOAP device", error);
                }
            }
            if (attempt + 1 < UsbRetryPolicy.NEGOTIATION_ATTEMPTS) {
                recoverConnection();
                SystemClock.sleep(UsbRetryPolicy.negotiationDelayMs(attempt));
            }
        }
        return null;
    }

    private boolean checkAOAPDevice(UsbDevice device){
        if (device != null && UsbAccessoryIds.isAoap(device.getVendorId(), device.getProductId())) {
            if (Log.isInfo()) Log.i(TAG, "Found aop device");
            return true;
        }
        if (Log.isInfo()) Log.i(TAG, "Device no aop");
        return false;
    }

    public List<LibUsbDevice> connectedDevices(){
        if (usbManager_.getDeviceList() != null) {
            List<LibUsbDevice> attachedDevices = new ArrayList<>();
            for(UsbDevice dev : usbManager_.getDeviceList().values()){
                LibUsbDevice usbDevice = libUsb_.createDevice(dev, usbManager_);
                attachedDevices.add(usbDevice);
            }
            return attachedDevices;
        }
        return Collections.EMPTY_LIST;
    }

    public void searchForAoapDevice(){
        if (usbManager_.getDeviceList() != null) {
            for(UsbDevice dev : usbManager_.getDeviceList().values()){
                if (checkAOAPDevice(dev)){
                    handleUsbAttached(dev);
                    return;
                }
            }
        }
    }

    public boolean requestAOAP(UsbDevice device){
        UsbDeviceConnection usbConnection = usbManager_.openDevice(device);
        if (usbConnection != null){
            try {
                if (Log.isDebug()) Log.d(TAG, "switch to OAP");

                byte[] buffer = new byte[2];
                int len = usbConnection.controlTransfer(UsbConstants.USB_DIR_IN | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_GET_PROTOCOL, 0, 0, buffer, 2, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != 2) {
                    Log.e(TAG, "Error controlTransfer GET_PROTOCOL len: " + len);
                    return false;
                }
                int accVer = (buffer[1] << 8) | buffer[0];
                if (Log.isDebug()) Log.d(TAG, "accessory version: " + accVer);
                if (accVer < 1) {                                                  // If error or version too low...
                    Log.e(TAG, "No supported accessory");
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "Send Manufacter " + MANUFACTURER);
                buffer = (MANUFACTURER + "\0").getBytes();
                len = usbConnection.controlTransfer(UsbConstants.USB_DIR_OUT | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0, 0, buffer, buffer.length, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != buffer.length) {
                    Log.e(TAG, "Error sending Manufacter, len= " + len + " expected= " + buffer.length);
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "Send Model " + MODEL);
                buffer = (MODEL + "\0").getBytes();
                len = usbConnection.controlTransfer(UsbConstants.USB_DIR_OUT | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0, 1, buffer, buffer.length, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != buffer.length) {
                    Log.e(TAG, "Error sending Model, len= " + len + " expected= " + buffer.length);
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "Send Desc " + DESC);
                buffer = (DESC + "\0").getBytes();
                len = usbConnection.controlTransfer(UsbConstants.USB_DIR_OUT | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0, 2, buffer, buffer.length, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != buffer.length) {
                    Log.e(TAG, "Error sending Desc, len= " + len + " expected= " + buffer.length);
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "Send Version " + VERSION);
                buffer = (VERSION + "\0").getBytes();
                len = usbConnection.controlTransfer(UsbConstants.USB_DIR_OUT | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0, 3, buffer, buffer.length, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != buffer.length) {
                    Log.e(TAG, "Error sending Version, len= " + len + " expected= " + buffer.length);
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "Send URI " + URI);
                buffer = (URI + "\0").getBytes();
                len = usbConnection.controlTransfer(UsbConstants.USB_DIR_OUT | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0, 4, buffer, buffer.length, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != buffer.length) {
                    Log.e(TAG, "Error sending Manufacter, len= " + len + " expected= " + buffer.length);
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "Send Serial " + SERIAL);
                buffer = (SERIAL + "\0").getBytes();
                len = usbConnection.controlTransfer(UsbConstants.USB_DIR_OUT | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0, 5, buffer, buffer.length, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != buffer.length) {
                    Log.e(TAG, "Error sending Serial, len= " + len + " expected= " + buffer.length);
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "Sending acc start");           // Send accessory start request. Device should re-enumerate as an accessory.
                len = usbConnection.controlTransfer(UsbConstants.USB_DIR_OUT | UsbConstants.USB_TYPE_VENDOR, ACC_REQ_START, 0, 0, null, 0, AOAP_TRANSFER_TIMEOUT_MS);
                if (len != 0) {
                    Log.e(TAG, "Error in accessory start request");
                    return false;
                }

                if (Log.isDebug()) Log.d(TAG, "switchOAP everything OK");
                return true;
            } catch (Exception error) {
                Log.e(TAG, "controlTransfer failed during AOAP negotiation", error);
                return false;
            } finally {
                if (Log.isDebug()) Log.d(TAG, "Closing connection");
                usbConnection.close();
            }
        }
        return false;
    }

}
