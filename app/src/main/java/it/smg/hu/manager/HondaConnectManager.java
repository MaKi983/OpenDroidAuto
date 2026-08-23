package it.smg.hu.manager;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.media.AudioTrack;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.RemoteException;
import android.widget.Toast;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHf;
import com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener;
import com.fujitsu_ten.displayaudio.bluetooth.handsfree.ConnectedPhoneInfo;
import com.fujitsu_ten.displayaudio.ecncservice.IEcNcService;
import com.fujitsu_ten.displayaudio.modemanagement.IModeMgrServiceCallBack;
import com.fujitsu_ten.displayaudio.modemanagement.IModeMgrServiceSWKeyEventCallBack;
import com.fujitsu_ten.displayaudio.modemanagement.ModeMgrManager;
import com.fujitsu_ten.displayaudio.oom.OomManager;
import com.fujitsu_ten.displayaudio.statemanagement.IStateMgrServiceCallBack;
import com.fujitsu_ten.displayaudio.statemanagement.StateMgrChangeInfo;
import com.fujitsu_ten.displayaudio.statemanagement.StateMgrInfo;
import com.fujitsu_ten.displayaudio.statemanagement.StateMgrManager;
import com.fujitsu_ten.displayaudio.statemanagement.StateMgrServiceConst;
import com.fujitsu_ten.displayaudio.steeringmenuservice.service.ISteeringMenuService;
import com.fujitsu_ten.displayaudio.steeringmenuservice.service.ISteeringMenuServiceCallback;
import com.fujitsu_ten.displayaudio.whitelist.common.Constants;
import com.fujitsu_ten.displayaudio.whitelist.common.IWhiteList;
import com.fujitsu_ten.displayaudio.whitelist.common.ProcessControl;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import it.smg.hu.config.Settings;
import it.smg.hu.ui.PlayerActivity;
import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.aasdk.projection.ISensor;
import it.smg.libs.common.Log;

public class HondaConnectManager {

    public static class SWMode {
        public static final String SW_SERVICE = "SW SERVICE";
//        public static final String MODEMGR_KEY_CALLBACK = "MODEMGR KEY CALLBACK";
    }

    public static class  AudioStreamType {
        public static final int ADA_NORMAL = 11;
        public static final int ADA_INTERRUPT = 12;
        public static final int ADA_INTERRUPT_LP = 13;
        public static final int ADA_INTERRUPT_VR = 14;
        public static final int ADA_INTERRUPT_NAVI = 15;
    }

    static class ModeMgrMode {
        public static final int AUDIO_MODE = 1;
        public static final int VIDEO_MODE = 2;
        public static final int AUDIO_VIDEO_MODE = 3;
    }

    private static final String TAG = "HondaConnectManager";
    private static final String ModeMgrService = "ModeMgrService";
    private static final String StateMgrService = "StateMgrService";

    private static final long HEARTBEAT_INTERVAL_MS = 4000;

    private static HondaConnectManager instance_;

    private final ModeMgrManager modeMgrManager_;
    private final StateMgrManager stateMgrManager_;

    private IModeMgrServiceCallBack modeMgrServiceAudioVideoCallBack_;
    private IModeMgrServiceSWKeyEventCallBack modeMgrServiceSWKeyEventCallBack_;

    private IStateMgrServiceCallBack stateMgrServiceCallBack_;

    // SteeringWheel service
    private ISteeringMenuService steeringMenuServiceIface_;
    private final ServiceConnection steeringMenuServiceConnection_;
    private ISteeringMenuServiceCallback steeringMenuServiceCallback_;
    private boolean boundToSteeringMenuService_;

    // EcNc service
    private IEcNcService ecNcServiceIface_;
    private final ServiceConnection ecNcServiceConnection_;
    private boolean boundToEcNcService_;
    private boolean micVrStarted_;

    // Bluetooth
    private BluetoothHfpHf bluetoothHfpHf_;
    private HfpHfProfileListener headsetlistener_;

    private final Context context_;
    private final Settings settings_;

    private boolean restoreActivity_;
    private boolean isRunning_;
    private boolean hasAudioFocus_;
    private boolean hasAudioFocusForMic_;
    private ProcessControl pControl_;
    private CountDownLatch waitCondOn_;
    private CountDownLatch waitCondOff_;

    private ScheduledExecutorService heartbeatExecutor_;
    private ScheduledFuture<?> heartbeatFuture_;
    private int currentModeState_;
    private int currentIdx_;

    private final LocalBroadcastManager localBroadcastManager_;

    private final Handler mainHandler_;
    private ISensor.Listener dayNightListener_;

    public static void init(Context context){
        instance_ = new HondaConnectManager(context);
    }

    public static HondaConnectManager instance(){
        return instance_;
    }

    @SuppressLint("WrongConstant")
    private HondaConnectManager(Context context){
        if (Log.isDebug()) Log.d(TAG, "init");
        context_ = context;
        settings_ = Settings.instance();
        hasAudioFocus_ = false;
        hasAudioFocusForMic_ = false;
        boundToEcNcService_ = false;
        micVrStarted_ = false;
        isRunning_ = false;
        restoreActivity_ = false;
        mainHandler_ = new Handler(Looper.getMainLooper());
        currentModeState_ = 0;
        currentIdx_ = 255;

        localBroadcastManager_ = LocalBroadcastManager.getInstance(context_);

        modeMgrManager_ = (ModeMgrManager) context.getSystemService(ModeMgrService);
        if (modeMgrManager_ == null){
            if (Log.isWarn()) Log.w(TAG, "modeMgrManager null");
        }

        stateMgrManager_ = (StateMgrManager) context.getSystemService(StateMgrService);
        if (stateMgrManager_ == null) {
            if (Log.isWarn()) Log.w(TAG, "stateMgrManager null");
        }

        steeringMenuServiceConnection_ = new ServiceConnection() {
            private static final String TAG = "HondaConnectManager-steeringServiceConnection";

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                if (Log.isDebug()) Log.d(TAG, "Honda Wheel Service connected");
                boundToSteeringMenuService_ = true;
                steeringMenuServiceIface_ = ISteeringMenuService.Stub.asInterface(service);

                registerSteeringMenuCallback();

                if (pControl_.authType != Constants.AUTH_TYPE_PREINSTALL){
                    notifySteeringMenuDispMode(1);
                }

            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                if (Log.isDebug()) Log.d(TAG, "Honda Wheel Service disconnected");
                boundToSteeringMenuService_ = false;
                steeringMenuServiceIface_ = null;
            }
        };

        ecNcServiceConnection_ = new ServiceConnection() {
            private static final String TAG = "HondaConnectManager-ecNcServiceConnection";

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                if (Log.isDebug()) Log.d(TAG, "Honda EcNc Service connected");
                boundToEcNcService_ = true;
                ecNcServiceIface_ = IEcNcService.Stub.asInterface(service);
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                if (Log.isDebug()) Log.d(TAG, "Honda EcNc Service disconnected");

                boundToEcNcService_ = false;
                ecNcServiceIface_ = null;
                micVrStarted_ = false; // force to false
            }
        };

        bluetoothHfpHf_ = BluetoothHfpHf.getInstance();
        if (bluetoothHfpHf_ != null) {
            if (Log.isDebug()) Log.d(TAG, "bluetoothHfpHf " + bluetoothHfpHf_);
            headsetlistener_ = new HfpHfProfileListener();
            bluetoothHfpHf_.addListener(this.headsetlistener_);
        }

        try {
            pControl_ = IWhiteList.getProcessControl("it.smg.hu", null);
            if (pControl_ != null) {
                if (Log.isDebug()) {
                    Log.d(TAG, "ProcessControl [ ");
                    Log.d(TAG, "appType= " + pControl_.appType);
                    Log.d(TAG, "authType= " + pControl_.authType);
                    Log.d(TAG, "lastMode= " + pControl_.lastMode);
                    Log.d(TAG, "oomSetPerm= " + pControl_.oomSetPerm);
                    Log.d(TAG, "result= " + pControl_.result);
                    Log.d(TAG, "soundInterrupt= " + pControl_.soundInterrupt);
                    Log.d(TAG, "soundInterruptMute= " + pControl_.soundInterruptMute);
                    Log.d(TAG, "soundOut= " + pControl_.soundOut);
                    Log.d(TAG, "videoOut= " + pControl_.videoOut);
                    Log.d(TAG, "]");
                }
            }
        } catch (Throwable t){
            Log.e(TAG, "process control error", t);
        }

    }

    public int mediaAudioStream(ChannelId audioChannel){
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            switch (audioChannel) {
                case SPEECH_AUDIO:
                case SYSTEM_AUDIO:
                    return AudioStreamType.ADA_INTERRUPT_NAVI;
                case MEDIA_AUDIO:
                default:
                    return AudioStreamType.ADA_NORMAL;
            }
        }

        return AudioTrack.MODE_STREAM;
    }

    public void adjustPermission(){
        try {
            int ret = OomManager.getOomAdjustment(context_);
            if (Log.isDebug()) Log.d(TAG, "getOomAdjustment " + ret);
            ret = OomManager.setOomAdjustmentCoreServer(context_);
            if (Log.isDebug()) Log.d(TAG, "setOomAdjustment " + ret);
        } catch (Exception e){
            Log.e(TAG, "error in adjustPermission", e);
        }
    }

    public void requestAudioFocus(){
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && !hasAudioFocus_) {
            requestFocus(settings_.advanced.modeMgrAudioVideoIdx(), ModeMgrMode.AUDIO_VIDEO_MODE);

            registerSteeringMenuCallback();
            notifySteeringMenuDispMode(1);
        }
    }

    public void releaseAudioFocus(){
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && hasAudioFocus_) {
            releaseFocus(settings_.advanced.modeMgrAudioVideoIdx(), ModeMgrMode.AUDIO_VIDEO_MODE);
            unregisterSteeringMenuCallback();
            notifySteeringMenuDispMode(0);
        }
    }

    private void requestFocus(int idx, int mode){
        if (Log.isDebug()) Log.d(TAG, "requestFocus -> app with auth " + pControl_.authType);
        if (Log.isDebug()) Log.d(TAG, "requestFocus modeMgr hasAudioFocus= " + hasAudioFocus_);
        if (Log.isDebug()) Log.d(TAG, "requestFocus modeMgr idx= " + idx + ", mode= " + mode);

        // If AUTH_TYPE <> preinstall the app has already audio focus
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL) {

            if (idx == currentIdx_ && mode == currentModeState_){
                if (Log.isWarn()) Log.w(TAG, "idx == currentIdx && mode == currentModeState -> do nothing");
                return;
            }

//            int idx = settings_.advanced.modeMgrAudioIdx();
            int ret;

            int sound_param = mode & ModeMgrMode.AUDIO_MODE;
            int image_param = mode & ModeMgrMode.VIDEO_MODE;
            if (Log.isDebug()) Log.d(TAG, "requestFocus sound_param= " + sound_param + ", image_param= " + image_param);

            if (image_param != 0){
                if (Log.isDebug()) Log.d(TAG, "requestFocus sendModeMgrOnReqForceVideo idx= " + idx + ", mode= " + mode);
                ret = modeMgrManager_.sendModeMgrOnReqForceVideo(idx, mode);
                if (Log.isDebug()) Log.d(TAG, "requestFocus sendModeMgrOnReqForceVideo result= " + ret);
            } else if (sound_param != 0) {
                if (Log.isDebug()) Log.d(TAG, "requestFocus sendModeMgrOnReq idx= " + idx + ", mode= " + mode);
                ret = modeMgrManager_.sendModeMgrOnReq(idx, mode);
                if (Log.isDebug()) Log.d(TAG, "requestFocus sendModeMgrOnReq result= " + ret);
            } else {
                if (Log.isWarn()) Log.w(TAG, "neither image nor sound -> do nothing");
                return;
            }

            if (Log.isDebug()) Log.d(TAG, "requestFocus -> wait for cond");

            waitCondOn_ = new CountDownLatch(1);
            waitForCond(1000, waitCondOn_);
            waitCondOn_ = null;
        }
    }

    private void releaseFocus(int idx, int mode){
        if (Log.isDebug()) Log.d(TAG, "releaseFocus -> app with auth " + pControl_.authType);
        if (Log.isDebug()) Log.d(TAG, "releaseFocus modeMgr audio hasAudioFocus= " + hasAudioFocus_);

        // If AUTH_TYPE <> preinstall the app has already audio focus
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL) {
            int ret;

            if (Log.isDebug()) Log.d(TAG, "releaseFocus sendModeMgrOffReq idx= " + idx + ", state = " + mode);
            ret = modeMgrManager_.sendModeMgrOffReq(idx, mode);
            if (Log.isDebug()) Log.d(TAG, "releaseFocus sendModeMgrOffReq ret= " + ret);

            if (Log.isDebug()) Log.d(TAG, "releaseFocus -> wait for cond");

            waitCondOff_ = new CountDownLatch(1);
            waitForCond(1000, waitCondOff_);
            waitCondOff_ = null;
        }
    }

    public void increaseVolume(){
        if (Log.isDebug()) Log.d(TAG, "increaseVolume");
        modeMgrManager_.reqModeMgrSteeringVolCmd(true);
    }

    public void decreaseVolume(){
        if (Log.isDebug()) Log.d(TAG, "decreaseVolume");
        modeMgrManager_.reqModeMgrSteeringVolCmd(false);
    }

    // Used in onCreate
    public void initialize(){
        if (Log.isDebug()) Log.d(TAG, "initialize -> app with auth " + pControl_.authType);

        bindToEcNcService();
        bindToWheelService();

        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL) {
            registerModeMgrVideoAudioCallback();
            registerStateMgrCallback();

            if (settings_.advanced.hondaImidEnabled()) {
                try {
                    if (Log.isDebug()) Log.d(TAG, "requestFocus setImidConnectStatus 1");
                    int ret = modeMgrManager_.setImidConnectStatus(1);
                    if (Log.isDebug()) Log.d(TAG, "requestFocus setImidConnectStatus ret= " + ret);
                } catch (Exception e) {
                    Log.w(TAG, "Could not set i-MID connect status", e);
                }
            }
        }
    }

    // Used in onResume
    public void initAudioBinding(){
        if (Log.isDebug()) Log.d(TAG, "initAudioBinding -> app with auth " + pControl_.authType);
        // if app has THIRD_PARTY auth will have exclusive audio focus, only bind wheel service
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            if (Log.isDebug()) Log.d(TAG, "initAudioBinding -> app auth = preinstall -> register ModeMgr and SW callback");

            isRunning_ = true;

            if (Log.isDebug()) Log.d(TAG, "initAudioBinding -> hasAudioFocus= " + hasAudioFocus_);
            if (hasAudioFocus_){
                requestFocus(settings_.advanced.modeMgrAudioVideoIdx(), ModeMgrMode.AUDIO_VIDEO_MODE);
                registerSteeringMenuCallback();
                notifySteeringMenuDispMode(1);
            }
        } else {
            // THIRD_PARTY
            if (Log.isDebug()) Log.d(TAG, "initAudioBinding -> using authType not PREINSTALL -> register SW callback and notify");
            registerSteeringMenuCallback();
            notifySteeringMenuDispMode(1);
        }
    }

    // Used in onPause (app in background)
    public void sendToBackground(){
        if (Log.isDebug()) Log.d(TAG, "sendToBackground -> app with auth " + pControl_.authType + " unregister SW callback");
        unregisterSteeringMenuCallback();
        notifySteeringMenuDispMode(0);
    }

    public void endAudioBinding(){
        if (Log.isDebug()) Log.d(TAG, "endAudioBinding -> app with auth " + pControl_.authType);

        stopMicSession();
        unbindFromEcNcService();

        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            if (Log.isDebug()) Log.d(TAG, "endAudioBinding -> auth PREINSTALL -> release audio and unregister modemgr callback");

            isRunning_ = false;
            int ret;

            if (hasAudioFocus_){
                releaseFocus(currentIdx_, currentModeState_);
                notifySteeringMenuDispMode(0);
            }

            unregisterModeMgrVideoAudioCallback();
            unregisterStateMgrCallback();

            if (settings_.advanced.hondaImidEnabled()) {
                try {
                    if (Log.isDebug()) Log.d(TAG, "releaseFocus setImidConnectStatus 0");
                    ret = modeMgrManager_.setImidConnectStatus(0);
                    if (Log.isDebug()) Log.d(TAG, "releaseFocus setImidConnectStatus ret= " + ret);
                } catch (Exception e) {
                    Log.w(TAG, "Could not set i-MID connect status", e);
                }
            }
        }

        unregisterSteeringMenuCallback();
        unbindToWheelService();
    }

    private void notifySteeringMenuDispMode(int mode){
        if (Log.isDebug()) Log.d(TAG, "notifySteeringMenuDispMode -> boundToSteeringMenuService= " + boundToSteeringMenuService_);
        if (boundToSteeringMenuService_) {
            try {
                int idx = settings_.advanced.steeringWheelIdx();
                if (idx > 0) {
                    if (Log.isDebug()) Log.d(TAG, "notifySteeringMenuDispMode " + mode + " addr " + idx);
                    steeringMenuServiceIface_.notifySteeringMenuDispMode(idx, mode);
                }
            } catch (RemoteException e) {
                Log.e(TAG, "Error registering", e);
            } catch (RuntimeException e) {
                Log.e(TAG, "Unexpected error notifySteeringMenuDispMode", e);
            }
        }
    }

    private void registerSteeringMenuCallback(){
        if (Log.isDebug()) Log.d(TAG, "registerSteeringMenuCallback -> boundToSteeringMenuService= " + boundToSteeringMenuService_ + ", steeringMenuServiceCallback= " + (steeringMenuServiceCallback_ == null ? "null" : "not null"));
        try {
            if (boundToSteeringMenuService_ && steeringMenuServiceCallback_ == null) {
                int idx = settings_.advanced.steeringWheelIdx();
                if (Log.isDebug()) Log.d(TAG, "registerCallbackEx swaddr " + idx);
                steeringMenuServiceCallback_ = new SteeringMenuServiceCallback();
                steeringMenuServiceIface_.registerCallbackEx(steeringMenuServiceCallback_, idx);
            }
        } catch (RemoteException e) {
            Log.e(TAG, "Error registerCallbackEx", e);
            steeringMenuServiceCallback_ = null;
        } catch (RuntimeException e) {
            Log.e(TAG, "Unexpected error registerCallbackEx", e);
            steeringMenuServiceCallback_ = null;
        }
    }

    private void unregisterSteeringMenuCallback(){
        if (Log.isDebug()) Log.d(TAG, "unregisterSteeringMenuCallback -> boundToSteeringMenuService= " + boundToSteeringMenuService_ + ", steeringMenuServiceCallback= " + (steeringMenuServiceCallback_ == null ? "null" : "not null"));
        try {
            if (boundToSteeringMenuService_ && steeringMenuServiceCallback_ != null) {
                int idx = settings_.advanced.steeringWheelIdx();
                if (Log.isDebug()) Log.d(TAG, "unregisterCallbackEx swaddr " + idx);
                steeringMenuServiceIface_.unregisterCallbackEx(steeringMenuServiceCallback_, idx);
                steeringMenuServiceCallback_ = null;
            }
        } catch (RemoteException e) {
            Log.e(TAG, "Error unregisterCallbackEx", e);
            steeringMenuServiceCallback_ = null;
        } catch (RuntimeException e) {
            Log.e(TAG, "Unexpected error unregisterCallbackEx", e);
            steeringMenuServiceCallback_ = null;
        }
    }

    public void startMicSession() {
        if (Log.isDebug()) Log.d(TAG, "starting MicSession -> boundToEcNcService= " + boundToEcNcService_ + " hasAudioFocus= " + hasAudioFocus_);
        if (!settings_.advanced.hondaMicVrEnabled()){
            if (Log.isDebug()) Log.d(TAG, "mic disabled");
            return;
        }

        int ret;
        if (boundToEcNcService_) {
            if (micVrStarted_){
                if (Log.isWarn()) Log.w(TAG, "mic session already started");
                return;
            }

            try {
                ret = ecNcServiceIface_.startVR(true);
                if (Log.isDebug()) {
                    Log.d(TAG, "ecNcServiceIface_ startVR ret= " + ret);
//                    mainHandler_.post(() -> {
//                        Toast.makeText(context_, "ecNcServiceIface_ startVR ret= " + ret, Toast.LENGTH_SHORT).show();
//                    });
                }
                if (ret == 0) {
                    micVrStarted_ = true;
                }
            } catch (RemoteException e) {
                Log.e(TAG, "startVR exception", e);
            }
        }

        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && !hasAudioFocus_) {
            if (Log.isDebug()) Log.d(TAG, "startMicSession hasAudioFocus_ false -> request audio focus");
            requestFocus(settings_.advanced.modeMgrAudioVideoIdx(), ModeMgrMode.AUDIO_VIDEO_MODE);
            hasAudioFocusForMic_ = true;
        }

        if (micVrStarted_){
            if (Log.isWarn()) Log.w(TAG, "mic session already started");
            return;
        }

//        if (Log.isDebug()) Log.d(TAG, "startMicSession getVoiceControlRights");
//        ret = bluetoothHfpHf_.getVoiceControlRights();
//        if (ret == 0){
//            micVrStarted_ = true;
//        }
//        if (Log.isDebug()) Log.d(TAG, "startMicSession getVoiceControlRights= " + ret);

    }

    public void stopMicSession() {
        if (Log.isDebug()) Log.d(TAG, "stopping MicSession -> boundToEcNcService= " + boundToEcNcService_ + " hasAudioFocus= " + hasAudioFocus_);
        if (!settings_.advanced.hondaMicVrEnabled()){
            if (Log.isDebug()) Log.d(TAG, "mic disabled");
            return;
        }

        if (boundToEcNcService_) {
            if (!micVrStarted_) {
                Log.w(TAG, "no mic session started, return");
                return;
            }

            try {
                int ret = ecNcServiceIface_.endVr();
                if (Log.isDebug()) {
                    Log.d(TAG, "ecNcServiceIface_ endVr ret= " + ret);
//                    mainHandler_.post(() -> {
//                        Toast.makeText(context_, "ecNcServiceIface_ endVr ret= " + ret, Toast.LENGTH_SHORT).show();
//                    });
                }
                if (ret == 0) {
                    micVrStarted_ = false;
                }
            } catch (RemoteException e) {
                Log.e(TAG, "endVr exception", e);
            }

        }

        if (Log.isDebug()) Log.d(TAG, "stopMicSession releaseVoiceControlRights");
        int ret = bluetoothHfpHf_.releaseVoiceControlRights();
        if (Log.isDebug()) Log.d(TAG, "stopMicSession releaseVoiceControlRights= " + ret);
        if (ret == 0) {
            micVrStarted_ = false;
        }

        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && hasAudioFocusForMic_) {
            if (Log.isDebug()) Log.d(TAG, "startMicSession hasAudioFocusForMic_ true -> release audio focus");
            releaseFocus(settings_.advanced.modeMgrAudioVideoIdx(), ModeMgrMode.AUDIO_VIDEO_MODE);
            hasAudioFocusForMic_ = false;
        }
    }

    private void bindToEcNcService() {
        if (!settings_.advanced.hondaMicVrEnabled()){
            if (Log.isDebug()) Log.d(TAG, "mic disabled");
            return;
        }

        if (!boundToEcNcService_) {
            if (Log.isDebug()) Log.d(TAG, "Request binding to service " + IEcNcService.class.getName());
            Intent intent = new Intent(IEcNcService.class.getName());
            context_.bindService(intent, ecNcServiceConnection_, Context.BIND_AUTO_CREATE);
        }
    }

    private void unbindFromEcNcService() {
        if (!settings_.advanced.hondaMicVrEnabled()){
            if (Log.isDebug()) Log.d(TAG, "mic disabled");
            return;
        }

        if (boundToEcNcService_) {
            if (Log.isDebug()) Log.d(TAG, "Request unbinding to service " + IEcNcService.class.getName());
            context_.unbindService(ecNcServiceConnection_);

            boundToEcNcService_ = false;
            ecNcServiceIface_ = null;
            micVrStarted_ = false; // force to false
        }
    }

    private void bindToWheelService(){
        if (!boundToSteeringMenuService_) {
            if (Log.isDebug()) Log.d(TAG, "Request binding to service " + ISteeringMenuService.class.getName());
            Intent intent = new Intent(ISteeringMenuService.class.getName());
            context_.bindService(intent, steeringMenuServiceConnection_, Context.BIND_AUTO_CREATE);
        }
    }

    private void unbindToWheelService(){
        if (boundToSteeringMenuService_) {
            if (Log.isDebug()) Log.d(TAG, "Request unbinding to service " + ISteeringMenuService.class.getName());
            context_.unbindService(steeringMenuServiceConnection_);

            boundToSteeringMenuService_ = false;
            steeringMenuServiceIface_ = null;
        }
    }

    private void unregisterModeMgrVideoAudioCallback() {
        if (Log.isDebug()) Log.d(TAG, "unregisterModeMgrVideoAudioCallback");

        if (modeMgrManager_ != null) {
            int idx = settings_.advanced.modeMgrAudioVideoIdx();
            if (Log.isDebug()) Log.d(TAG, "unregisterModeMgrVideoAudioCallback idx " + idx);
            int ret = modeMgrManager_.unregisterModeMgrCallback(idx);
            if (Log.isDebug()) Log.d(TAG, "unregisterModeMgrVideoAudioCallback ret " + ret);
            modeMgrServiceAudioVideoCallBack_ = null;
        }
    }

    private void registerModeMgrVideoAudioCallback(){
        if (Log.isDebug()) Log.d(TAG, "registerModeMgrVideoAudioCallback");
        if (modeMgrManager_ != null) {
            int idx = settings_.advanced.modeMgrAudioVideoIdx();
            if (Log.isDebug()) Log.d(TAG, "registerModeMgrVideoAudioCallback idx " + idx);
            modeMgrServiceAudioVideoCallBack_ = new ModeMgrServiceVideoAudioCallBack();
            int ret = modeMgrManager_.registerModeMgrCallback(idx, modeMgrServiceAudioVideoCallBack_);
            if (Log.isDebug()) Log.d(TAG, "registerModeMgrVideoAudioCallback ret " + ret);
        } else {
            Log.w(TAG, "modeMgrManager_ null -> do nothing");
        }
    }

    private void registerStateMgrCallback() {
        if (Log.isDebug()) Log.d(TAG, "registerStateMgrCallback");
        if (stateMgrManager_ != null){
            StateMgrChangeInfo changeInfo = createStateMgrChangeInfo();
            stateMgrServiceCallBack_ = new StateMgrServiceCallBack();
            int ret = stateMgrManager_.registCallBack(stateMgrServiceCallBack_, changeInfo);
            if (Log.isDebug()) Log.d(TAG, "registerStateMgrCallback ret " + ret);
        } else {
            Log.w(TAG, "stateMgrManager_ null -> do nothing");
        }
    }

    private void unregisterStateMgrCallback() {
        if (Log.isDebug()) Log.d(TAG, "registerStateMgrCallback");
        if (stateMgrManager_ != null){
            if (Log.isDebug()) Log.d(TAG, "stateMgrManager_ unRegistCallBack");
            int ret = stateMgrManager_.unRegistCallBack(stateMgrServiceCallBack_);
            if (Log.isDebug()) Log.d(TAG, "unRegistCallBack ret " + ret);
            stateMgrServiceCallBack_ = null;
        } else {
            Log.w(TAG, "stateMgrManager_ null -> do nothing");
        }
    }

    private StateMgrChangeInfo createStateMgrChangeInfo(){
        StateMgrChangeInfo changeInfo = new StateMgrChangeInfo();
        changeInfo.audioAddressC = true;
        changeInfo.dayNightStateC = true;
        changeInfo.rdsInterruptC = false;
        changeInfo.rdsAlarmInterruptC = false;
        changeInfo.videoAddressC = true;
        changeInfo.functionInfoC = false;
        changeInfo.openingScreenC = false;
        changeInfo.disclaimerC = false;
        changeInfo.languageC = false;
        changeInfo.antitheftC = false;
        changeInfo.inlineDiagC = false;
        changeInfo.volumePanelC = false;
        changeInfo.brightnessBarC = false;
        changeInfo.sourceFlowC = false;
        changeInfo.steeringMenuC = false;
        changeInfo.steeringPopUpC = false;
        changeInfo.audioLauncherC = false;
        changeInfo.iMidC = false;
        changeInfo.hftPopupC = false;
        changeInfo.screenOffC = true;
        changeInfo.usbPopupC = false;
        changeInfo.parkingSensorC = false;
        changeInfo.highTemperatureDetectionC = false;
        changeInfo.usbOvercurrentC = false;
        changeInfo.keyOffTimerAdvanceC = false;
        changeInfo.keyOffTimerExpirationC = false;
        changeInfo.lastVideoAddressC = true;
        changeInfo.videoResumeCompletedC = false;
        changeInfo.lastAudioAddressC = false;
        changeInfo.audioResumeCompletedC = false;
        changeInfo.hftStateC = false;
        changeInfo.steeringDispKeyC = false;
        changeInfo.headerInterruptC = false;

        return changeInfo;
    }

    private boolean waitForCond(int timeout, CountDownLatch waitCond){
        boolean res = false;
        try {
//            waitCond = new CountDownLatch(1);
            res = waitCond.await(timeout, TimeUnit.MILLISECONDS);
            if (!res) {
                if (Log.isWarn()) Log.w(TAG, "timeout in waiting condition");
            } else {
                if (Log.isDebug()) Log.d(TAG, "received conf/notify condition");
            }
        } catch (InterruptedException e) {
            Log.e(TAG, "error in wait condition", e);
        }

//        waitCond = null;

        return res;
    }

    private void startHeartbeat() {
//        stopHeartbeat();
        if (Log.isDebug()) Log.d(TAG, "startHeartbeat currentModeState_ " + currentModeState_);

        // Evita doppioni
        if (currentModeState_ == 0) {
            if (Log.isDebug()) Log.d(TAG, "startHeartbeat currentState 0 -> stop heartbeat");
            stopHeartbeat();
            return;
        }

        if (heartbeatExecutor_ != null){
            if (Log.isDebug()) Log.d(TAG, "startHeartbeat heartbeat already running -> do nothing");
            return;
        }

        heartbeatExecutor_ = Executors.newSingleThreadScheduledExecutor();

        heartbeatFuture_ = heartbeatExecutor_.scheduleWithFixedDelay(() -> {
            if (modeMgrManager_ != null && currentModeState_ != 0) {
//                int idx = settings_.advanced.modeMgrAudioIdx();
                int idx = currentIdx_;
                if (Log.isDebug()) Log.d(TAG, "heartbeat notifyModeMgrStatus idx= " + idx + ", state = " + currentModeState_);
                int ret = modeMgrManager_.notifyModeMgrStatus(idx, currentModeState_);
                if (Log.isDebug()) Log.d(TAG, "heartbeat notifyModeMgrStatus ret = " + ret);
            }
        }, HEARTBEAT_INTERVAL_MS, HEARTBEAT_INTERVAL_MS, TimeUnit.MILLISECONDS);
    }

    private void stopHeartbeat() {
        if (Log.isDebug()) Log.d(TAG, "stopHeartbeat currentModeState_ " + currentModeState_);

        if (heartbeatFuture_ != null) {
            if (Log.isDebug()) Log.d(TAG, "stopHeartbeat cancel future");
            heartbeatFuture_.cancel(false);
            heartbeatFuture_ = null;
        }
        if (heartbeatExecutor_ != null) {
            if (Log.isDebug()) Log.d(TAG, "stopHeartbeat shutdown executor");
            heartbeatExecutor_.shutdownNow();
            heartbeatExecutor_ = null;
        }
    }

    private void dumpStateMgr(StateMgrInfo info){
        if (Log.isDebug()) {
            if (info != null) {
                if (info.updateState != null) {
                    if (info.updateState.antitheftC) Log.d(TAG, "StateMgrChangeInfo updateState.antitheftC " + info.updateState.antitheftC);
                    if (info.updateState.audioAddressC) Log.d(TAG, "StateMgrChangeInfo updateState.audioAddressC " + info.updateState.audioAddressC);
                    if (info.updateState.audioLauncherC) Log.d(TAG, "StateMgrChangeInfo updateState.audioLauncherC " + info.updateState.audioLauncherC);
                    if (info.updateState.audioResumeCompletedC) Log.d(TAG, "StateMgrChangeInfo updateState.audioResumeCompletedC " + info.updateState.audioResumeCompletedC);
                    if (info.updateState.brightnessBarC) Log.d(TAG, "StateMgrChangeInfo updateState.brightnessBarC " + info.updateState.brightnessBarC);
                    if (info.updateState.dayNightStateC) Log.d(TAG, "StateMgrChangeInfo updateState.dayNightStateC " + info.updateState.dayNightStateC);
                    if (info.updateState.disclaimerC) Log.d(TAG, "StateMgrChangeInfo updateState.disclaimerC " + info.updateState.disclaimerC);
                    if (info.updateState.functionInfoC) Log.d(TAG, "StateMgrChangeInfo updateState.functionInfoC " + info.updateState.functionInfoC);
                    if (info.updateState.headerInterruptC) Log.d(TAG, "StateMgrChangeInfo updateState.headerInterruptC " + info.updateState.headerInterruptC);
                    if (info.updateState.hftPopupC) Log.d(TAG, "StateMgrChangeInfo updateState.hftPopupC " + info.updateState.hftPopupC);
                    if (info.updateState.hftStateC) Log.d(TAG, "StateMgrChangeInfo updateState.hftStateC " + info.updateState.hftStateC);
                    if (info.updateState.highTemperatureDetectionC) Log.d(TAG, "StateMgrChangeInfo updateState.highTemperatureDetectionC " + info.updateState.highTemperatureDetectionC);
                    if (info.updateState.iMidC) Log.d(TAG, "StateMgrChangeInfo updateState.iMidC " + info.updateState.iMidC);
                    if (info.updateState.inlineDiagC) Log.d(TAG, "StateMgrChangeInfo updateState.inlineDiagC " + info.updateState.inlineDiagC);
                    if (info.updateState.keyOffTimerAdvanceC) Log.d(TAG, "StateMgrChangeInfo updateState.keyOffTimerAdvanceC " + info.updateState.keyOffTimerAdvanceC);
                    if (info.updateState.keyOffTimerExpirationC) Log.d(TAG, "StateMgrChangeInfo updateState.keyOffTimerExpirationC " + info.updateState.keyOffTimerExpirationC);
                    if (info.updateState.languageC) Log.d(TAG, "StateMgrChangeInfo updateState.languageC " + info.updateState.languageC);
                    if (info.updateState.lastAudioAddressC) Log.d(TAG, "StateMgrChangeInfo updateState.lastAudioAddressC " + info.updateState.lastAudioAddressC);
                    if (info.updateState.lastVideoAddressC) Log.d(TAG, "StateMgrChangeInfo updateState.lastVideoAddressC " + info.updateState.lastVideoAddressC);
                    if (info.updateState.lcdAdjustStateC) Log.d(TAG, "StateMgrChangeInfo updateState.lcdAdjustStateC " + info.updateState.lcdAdjustStateC);
                    if (info.updateState.openingScreenC) Log.d(TAG, "StateMgrChangeInfo updateState.openingScreenC " + info.updateState.openingScreenC);
                    if (info.updateState.parkingSensorC) Log.d(TAG, "StateMgrChangeInfo updateState.parkingSensorC " + info.updateState.parkingSensorC);
                    if (info.updateState.rdsAlarmInterruptC) Log.d(TAG, "StateMgrChangeInfo updateState.rdsAlarmInterruptC " + info.updateState.rdsAlarmInterruptC);
                    if (info.updateState.rdsInterruptC) Log.d(TAG, "StateMgrChangeInfo updateState.rdsInterruptC " + info.updateState.rdsInterruptC);
                    if (info.updateState.screenOffC) Log.d(TAG, "StateMgrChangeInfo updateState.screenOffC " + info.updateState.screenOffC);
                    if (info.updateState.siriStateC) Log.d(TAG, "StateMgrChangeInfo updateState.siriStateC " + info.updateState.siriStateC);
                    if (info.updateState.sourceFlowC) Log.d(TAG, "StateMgrChangeInfo updateState.sourceFlowC " + info.updateState.sourceFlowC);
                    if (info.updateState.steeringDispKeyC) Log.d(TAG, "StateMgrChangeInfo updateState.steeringDispKeyC " + info.updateState.steeringDispKeyC);
                    if (info.updateState.steeringMenuC) Log.d(TAG, "StateMgrChangeInfo updateState.steeringMenuC " + info.updateState.steeringMenuC);
                    if (info.updateState.steeringPopUpC) Log.d(TAG, "StateMgrChangeInfo updateState.steeringPopUpC " + info.updateState.steeringPopUpC);
                    if (info.updateState.usbOvercurrentC) Log.d(TAG, "StateMgrChangeInfo updateState.usbOvercurrentC " + info.updateState.usbOvercurrentC);
                    if (info.updateState.usbPopupC) Log.d(TAG, "StateMgrChangeInfo updateState.usbPopupC " + info.updateState.usbPopupC);
                    if (info.updateState.videoAddressC) Log.d(TAG, "StateMgrChangeInfo updateState.videoAddressC " + info.updateState.videoAddressC);
                    if (info.updateState.videoResumeCompletedC) Log.d(TAG, "StateMgrChangeInfo updateState.videoResumeCompletedC " + info.updateState.videoResumeCompletedC);
                    if (info.updateState.voiceTagStateC) Log.d(TAG, "StateMgrChangeInfo updateState.voiceTagStateC " + info.updateState.voiceTagStateC);
                    if (info.updateState.volumePanelC) Log.d(TAG, "StateMgrChangeInfo updateState.volumePanelC " + info.updateState.volumePanelC);
                }
                Log.d(TAG, "StateMgrInfo info.antitheft " + info.antitheft);
                Log.d(TAG, "StateMgrInfo info.audioAddress " + info.audioAddress);
                Log.d(TAG, "StateMgrInfo info.audioLauncher " + info.audioLauncher);
                Log.d(TAG, "StateMgrInfo info.functionInfo " + info.functionInfo);
                Log.d(TAG, "StateMgrInfo info.brightnessBar " + info.brightnessBar);
                Log.d(TAG, "StateMgrInfo info.audioResumeCompleted " + info.audioResumeCompleted);
                Log.d(TAG, "StateMgrInfo info.dayNightState " + info.dayNightState);
                Log.d(TAG, "StateMgrInfo info.disclaimer " + info.disclaimer);
                Log.d(TAG, "StateMgrInfo info.headerInterrupt " + info.headerInterrupt);
                Log.d(TAG, "StateMgrInfo info.hftPopup " + info.hftPopup);
                Log.d(TAG, "StateMgrInfo info.hftState " + info.hftState);
                Log.d(TAG, "StateMgrInfo info.highTemperatureDetection " + info.highTemperatureDetection);
                Log.d(TAG, "StateMgrInfo info.iMid " + info.iMid);
                Log.d(TAG, "StateMgrInfo info.inlineDiag " + info.inlineDiag);
                Log.d(TAG, "StateMgrInfo info.keyOffTimerAdvance " + info.keyOffTimerAdvance);
                Log.d(TAG, "StateMgrInfo info.keyOffTimerExpiration " + info.keyOffTimerExpiration);
                Log.d(TAG, "StateMgrInfo info.language " + info.language);
                Log.d(TAG, "StateMgrInfo info.lastAudioAddress " + info.lastAudioAddress);
                Log.d(TAG, "StateMgrInfo info.lastVideoAddress " + info.lastVideoAddress);
                Log.d(TAG, "StateMgrInfo info.lcdAdjustState " + info.lcdAdjustState);
                Log.d(TAG, "StateMgrInfo info.openingScreen " + info.openingScreen);
                Log.d(TAG, "StateMgrInfo info.parkingSensor " + info.parkingSensor);
                Log.d(TAG, "StateMgrInfo info.rdsAlarmInterrupt " + info.rdsAlarmInterrupt);
                Log.d(TAG, "StateMgrInfo info.rdsInterrupt " + info.rdsInterrupt);
                Log.d(TAG, "StateMgrInfo info.screenOff " + info.screenOff);
                Log.d(TAG, "StateMgrInfo info.siriState " + info.siriState);
                Log.d(TAG, "StateMgrInfo info.sourceFlow " + info.sourceFlow);
                Log.d(TAG, "StateMgrInfo info.steeringDispKey " + info.steeringDispKey);
                Log.d(TAG, "StateMgrInfo info.steeringMenu " + info.steeringMenu);
                Log.d(TAG, "StateMgrInfo info.steeringPopUp " + info.steeringPopUp);
                Log.d(TAG, "StateMgrInfo info.usbOvercurrent " + info.usbOvercurrent);
                Log.d(TAG, "StateMgrInfo info.usbPopup " + info.usbPopup);
                Log.d(TAG, "StateMgrInfo info.videoAddress " + info.videoAddress);
                Log.d(TAG, "StateMgrInfo info.videoResumeCompleted " + info.videoResumeCompleted);
                Log.d(TAG, "StateMgrInfo info.voiceTagState " + info.voiceTagState);
                Log.d(TAG, "StateMgrInfo info.volumePanel " + info.volumePanel);
            }
        }
    }

    public boolean isNight(){
        StateMgrInfo currentState = stateMgrManager_.getAllState();
        if (currentState != null) {
            return currentState.dayNightState == StateMgrServiceConst.STATE_NIGHT;
        }

        return false;
    }

    public void setDayNightListener(ISensor.Listener dayNightListener){
        dayNightListener_ = dayNightListener;
    }

    private class SteeringMenuServiceCallback extends ISteeringMenuServiceCallback.Stub {
        private static final String TAG = "HondaConnectManager-SteeringMenuServiceCallback";

        public void onShowView() {
            if (Log.isDebug()) Log.d(TAG, "onShowView isRunning_ " + isRunning_);
            if (isRunning_) {
                notifySteeringMenuDispMode(1);
            }
        }

        public boolean onFinishView(boolean flg, boolean anime) {
            if (Log.isDebug()) Log.d(TAG, "onFinishView");
            return true;
        }

        public boolean onSteeringSWDown(int keytype) {
            if (Log.isDebug()) Log.d(TAG, "onSteeringSWDown " + keytype);
            if (Log.isVerbose()){
                mainHandler_.post(() -> {
                    Toast.makeText(context_, "onSteeringSWDown keytype= " + keytype, Toast.LENGTH_SHORT).show();
                });
            }

            return true;
        }
    };

    private class StateMgrServiceCallBack extends IStateMgrServiceCallBack.Stub {
        private static final String TAG = "HondaConnectManager-StateMgrServiceCallBack";

        public void onChangeState(StateMgrInfo info) {
            if (Log.isDebug()) Log.d(TAG, "onChangeState");

            StateMgrChangeInfo changed = info.updateState;
            if (changed == null) return;

            dumpStateMgr(info);

            if (changed.videoAddressC){
                if (Log.isDebug()) Log.d(TAG, "videoAddress=" + info.videoAddress);
                if (Log.isDebug()) Log.d(TAG, "lastVideoAddress=" + info.lastVideoAddress);

                if (info.videoAddress == 92 && isRunning_) {
                    if (Log.isDebug()) {
                        Log.d(TAG, "activating rear camera -> restore activity after");
                        mainHandler_.post(() -> {
                            Toast.makeText(context_, "videoAddress " + info.videoAddress + " -> activating rear camera", Toast.LENGTH_SHORT).show();
                        });
                        restoreActivity_ = true;

//                        if (Log.isVerbose()) Log.v(TAG, "stop video indication");
//                        Intent stopIntent = new Intent(ODAService.STOP_VIDEO_INDICATION);
//                        localBroadcastManager_.sendBroadcast(stopIntent);
                    }
                } else if (info.videoAddress == 120 && isRunning_ && restoreActivity_) { // check info.videoResumeCompleted ?
                    if (Log.isDebug()){
                        Log.d(TAG, "coming from reverse camera -> restore activity");
                        mainHandler_.post(() -> {
                            Toast.makeText(context_, "videoAddress " + info.videoAddress + " -> restore activity", Toast.LENGTH_SHORT).show();
                        });
                        restoreActivity_ = false;
//                        Intent i = new Intent(context_, PlayerActivity.class);
//                        i.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_SINGLE_TOP);
//                        context_.startActivity(i);
                        Intent i = new Intent(context_, PlayerActivity.class);
                        i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                        context_.startActivity(i);
                    }
                }
            }

            if (changed.dayNightStateC) {
                boolean isNight = info.dayNightState == StateMgrServiceConst.STATE_NIGHT;
                if (Log.isDebug()) Log.d(TAG, "DayNight state changed, isNight: " + isNight);
                if (dayNightListener_ != null){
                    dayNightListener_.onDayNightUpdate(isNight);
                }
            }
            
//            if (changed.videoResumeCompletedC) {
//                Log.v(TAG, "videoResumeCompleted=" + info.videoResumeCompleted);
//
//                if (info.videoResumeCompleted == StateMgrServiceConst.STATE_COMPLETED) {
//                    Log.v(TAG, "videoResumeCompleted state completed");
//                    if (info.lastVideoAddress == 92) {
//                        Log.v(TAG, "videoResumeCompleted restore activity");
//                        Intent i = new Intent(context_, PlayerActivity.class);
//                        i.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_SINGLE_TOP);
//                        context_.startActivity(i);
//                    }
//                }
//            }
        }
    };

    private class ModeMgrServiceVideoAudioCallBack extends IModeMgrServiceCallBack.Stub {

        private static final String TAG = "HondaConnectManager-ModeMgrServiceVideoAudioCallBack";

        public void rcvOnInsCmd(int modestate) throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd modestate = " + modestate);
            int idx = settings_.advanced.modeMgrAudioVideoIdx();

            int sound_param = modestate & ModeMgrMode.AUDIO_MODE;
            int image_param = modestate & ModeMgrMode.VIDEO_MODE;
            if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd sound_param= " + sound_param + " image_param= " + image_param);

            if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd sendModeMgrOnCnf idx= " + idx + ",state = " + modestate);
            int ret = modeMgrManager_.sendModeMgrOnCnf(idx, modestate);
            if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd sendModeMgrOnCnf ret = " + ret);

            if (image_param != 0){
                if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd sendModeMgrCompDisp idx= " + idx);
                modeMgrManager_.sendModeMgrCompDisp(idx, 1);
            }

            if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd iAudioAddr = " + modeMgrManager_.getModeMgrOnAudioAddr());
            if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd iVideoAddr = " + modeMgrManager_.getModeMgrOnVideoAddr());

            // Aggiorna il modestate e avvia/aggiorna il heartbeat
            currentModeState_ |= modestate;
            if (sound_param != 0 && image_param != 0){
                hasAudioFocus_ = true;
            } else if (sound_param != 0) {
                hasAudioFocus_ = true;
            }
            currentIdx_ = idx;
            if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd currentModeState_ = " + currentModeState_ + ", currentIdx_= " + currentIdx_);

            startHeartbeat();

            if (waitCondOn_ != null) {
                if (Log.isDebug()) Log.d(TAG, "rcvOnInsCmd notify waitCondOn");
                waitCondOn_.countDown();
            }

        }

        public void rcvOffInsCmd(int modestate) throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd modestate = " + modestate);

            int sound_param = modestate & ModeMgrMode.AUDIO_MODE;
            int image_param = modestate & ModeMgrMode.VIDEO_MODE;
            if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd sound_param= " + sound_param + " image_param= " + image_param);

            int idx = settings_.advanced.modeMgrAudioVideoIdx();
            if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd sendModeMgrOffCnf idx= " + idx + ", state = " + modestate);
            int ret = modeMgrManager_.sendModeMgrOffCnf(idx, modestate);
            if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd sendModeMgrOffCnf ret = " + ret);

            // Rimuovi i bit revocati dallo stato corrente
            if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd currentIdx_ = " + currentIdx_ + ", idx= " + idx);
            if (currentIdx_ == idx) {
                currentModeState_ &= ~modestate;
                if (sound_param != 0 && image_param != 0) {
                    hasAudioFocus_ = false;
                } else if (sound_param != 0) {
                    hasAudioFocus_ = false;
                }
            } else {
                if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd currentIdx_ <> idx");
            }

            if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd currentModeState_ = " + currentModeState_ + ", currentIdx_= " + currentIdx_);

            if (currentModeState_ == 0) {
                if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd currentModeState_ == 0");
                stopHeartbeat();
                currentIdx_ = 255;
            } else {
                startHeartbeat();
            }

            if (waitCondOff_ != null) {
                if (Log.isDebug()) Log.d(TAG, "rcvOffInsCmd notify waitCondOff");
                waitCondOff_.countDown();
            }
        }

        public void rcvOnReqCmdFailed(int audioaddr, int videoaddr, int reason) throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "rcvOnReqCmdFailed audioaddr = " + audioaddr + " , videoaddr = " + videoaddr + " , reason = " + reason);
        }

        public void rcvVideoPwrCmd(int addr) throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "rcvVideoPwrCmd  addr = " + addr);
        }

        public void rcvAudioPwrONCmd(int addr) throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "rcvAudioPwrONCmd  addr = " + addr);
        }

        public void rcvAudioPwrOFFCmd() throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "rcvAudioPwrOFFCmd -S");
        }

        public void insDispApl(int disp, int extInfo1, int extInfo2) throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "insDispApl " + disp + "/" + extInfo1 + "/" + extInfo2);
        }
    };

    private final class HfpHfProfileListener implements BluetoothHfpHfListener {
        private static final String TAG = "HondaConnectManager-HfpHfProfileListener";

        private HfpHfProfileListener() {
        }

        @Override
        public void onAgPhoneNumNotified(String str) {
            if (Log.isDebug()) Log.d(TAG, "onAgPhoneNumNotified " + str);
        }

        @Override
        public void onCallStatusNotified(int i, int i2, int i3, int i4) {
            if (Log.isDebug()) Log.d(TAG, "onCallStatusNotified " + i + "/" + i2 + "/" + i3 + "/" + i4);
        }

        @Override
        public void onConnectedPhoneInfoNotified(List<ConnectedPhoneInfo> list) {
            if (Log.isDebug()) Log.d(TAG, "onConnectedPhoneInfoNotified " + Arrays.toString(list.toArray()));
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onDtmfSendResult() {
            if (Log.isDebug()) Log.d(TAG, "onDtmfSendResult");
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onErrorOccurred(int i, int i2) {
            if (Log.isDebug()) Log.d(TAG, "onErrorOccurred");
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onHfpConnected() {
            if (bluetoothHfpHf_ == null){
                Log.w(TAG, "bluetoothHfpHf_ null");
                return;
            }

            BluetoothDevice connectedDevice = bluetoothHfpHf_.getConnectedDevice();
            if (connectedDevice == null) {
                Log.w(TAG, "onHfpConnected() No Device");
                return;
            }

            String address = connectedDevice.getAddress();
            if (Log.isDebug()) Log.d(TAG, "onHfpConnected connectedDevice address: " + address);
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onHfpConnectionFailureNotified(boolean z) {
            if (Log.isDebug()) Log.d(TAG, "onHfpConnectionFailureNotified: " + z);
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onHfpDisconnected(int i) {
            if (Log.isDebug()) Log.d(TAG, "onHfpDisconnected: " + i);
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onHfpDisconnectionFailureNotified() {
            if (Log.isDebug()) Log.d(TAG, "onHfpDisconnectionFailureNotified");
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onHfpFirstAutoConnectEndNotified() {
            if (Log.isDebug()) Log.d(TAG, "onHfpFirstAutoConnectEndNotified");
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onIncomingPhoneNumNotified(int i, String str) {
            if (Log.isDebug()) Log.d(TAG, "onIncomingPhoneNumNotified " + i + "/" + str);
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onRequestSamplingRateChange(int i) {
            if (Log.isDebug()) Log.d(TAG, "onRequestSamplingRateChange " + i);
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onScoChanged(int i, int i2) {
            if (Log.isDebug()) Log.d(TAG, "onScoChanged " + i + "/" + i2);
        }

        @Override // com.fujitsu_ten.displayaudio.bluetooth.handsfree.BluetoothHfpHfListener
        public void onSwitchCallNotified() {
            if (Log.isDebug()) Log.d(TAG, "onSwitchCallNotified");
        }
    }
}
