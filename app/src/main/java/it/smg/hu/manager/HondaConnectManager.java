package it.smg.hu.manager;

import android.annotation.SuppressLint;
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

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import it.smg.hu.config.Settings;
import it.smg.hu.ui.PlayerActivity;
import it.smg.libs.aasdk.messenger.ChannelId;
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
//        public static final int REQUEST_MODE = Integer.parseInt("011111", 2); //31
//        public static final int CONFIRM_MODE = Integer.parseInt("111", 2); //7
//        public static final int NOTIFY_MODE = Integer.parseInt("11", 2); //3
//        public static final int AUDIO_MODE = Integer.parseInt("01", 2); //1
    }

    private static final String TAG = "HondaConnectManager";
    private static final String ModeMgrService = "ModeMgrService";
    private static final String StateMgrService = "StateMgrService";

    private static final long HEARTBEAT_INTERVAL_MS = 4000;
    private static final int BT_MODEMGR_ADDR = 197;

    private static HondaConnectManager instance_;

    private final ModeMgrManager modeMgrManager_;
    private final StateMgrManager stateMgrService_;

    private IModeMgrServiceCallBack modeMgrServiceCallBack_;
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

    private final Context context_;
    private final Settings settings_;

    private boolean hasAudioFocus_;
    private ProcessControl pControl_;
    private CountDownLatch waitCond_;

    private ScheduledExecutorService heartbeatExecutor_;
    private ScheduledFuture<?> heartbeatFuture_;
    private volatile int currentModeState_;

    private final Handler mainHandler_;

    public static void init(Context context){
        instance_ = new HondaConnectManager(context);
    }

    public static HondaConnectManager instance(){
        return instance_;
    }

    @SuppressLint("WrongConstant")
    private HondaConnectManager(Context context){
        if (Log.isInfo()) Log.i(TAG, "init");
        context_ = context;
        settings_ = Settings.instance();
        hasAudioFocus_ = false;
        boundToEcNcService_ = false;
        micVrStarted_ = false;
        mainHandler_ = new Handler(Looper.getMainLooper());
        currentModeState_ = 0;

        modeMgrManager_ = (ModeMgrManager) context.getSystemService(ModeMgrService);
        if (modeMgrManager_ == null){
            if (Log.isWarn()) Log.w(TAG, "modeMgrManager null");
        }

        stateMgrService_ = (StateMgrManager) context.getSystemService(StateMgrService);
        if (stateMgrService_ == null){
            Log.w(TAG, "stateMgrService null");
        }

        steeringMenuServiceConnection_ = new ServiceConnection() {
            private static final String TAG = "HondaConnectManager-steeringServiceConnection";

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                if (Log.isVerbose()) Log.v(TAG, "Honda Wheel Service connected");
                boundToSteeringMenuService_ = true;
                steeringMenuServiceIface_ = ISteeringMenuService.Stub.asInterface(service);

                registerSteeringMenuCallback();

                if (pControl_.authType != Constants.AUTH_TYPE_PREINSTALL){
                    notifySteeringMenuDispMode(1);
                }

            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                if (Log.isVerbose()) Log.v(TAG, "Honda Wheel Service disconnected");
                boundToSteeringMenuService_ = false;
                steeringMenuServiceIface_ = null;
            }
        };

        ecNcServiceConnection_ = new ServiceConnection() {
            private static final String TAG = "HondaConnectManager-ecNcServiceConnection";

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                if (Log.isVerbose()) Log.v(TAG, "Honda EcNc Service connected");
                boundToEcNcService_ = true;
                ecNcServiceIface_ = IEcNcService.Stub.asInterface(service);
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                if (Log.isVerbose()) Log.v(TAG, "Honda EcNc Service disconnected");

                boundToEcNcService_ = false;
                ecNcServiceIface_ = null;
                micVrStarted_ = false; // force to false
            }
        };

        try {
            pControl_ = IWhiteList.getProcessControl("it.smg.hu", null);
            if (pControl_ != null) {
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
        if (Log.isDebug()) Log.d(TAG, "requestAudioFocus -> app with auth " + pControl_.authType);
        if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus modeMgr audio hasAudioFocus= " + hasAudioFocus_);

        // If AUTH_TYPE <> preinstall the app has already audio focus
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && !hasAudioFocus_) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            int ret;

            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnReq idx= " + idx + ", mode= " + ModeMgrMode.AUDIO_MODE);
            ret = modeMgrManager_.sendModeMgrOnReq(idx, ModeMgrMode.AUDIO_MODE);
            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnReq result= " + ret);

//            try {
//                Thread.sleep(200);
//            } catch (InterruptedException ignored) {
//            }

            if (Log.isDebug()) Log.d(TAG, "requestAudioFocus -> wait for cond");
            waitForCond(500);

//            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnCnf idx= " + idx + ",state = " + ModeMgrMode.CONFIRM_MODE);
//            ret = modeMgrManager_.sendModeMgrOnCnf(idx, ModeMgrMode.CONFIRM_MODE);
//            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnCnf ret = " + ret);
//
//            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus notifyModeMgrStatus idx= " + idx + ", state = " + ModeMgrMode.NOTIFY_MODE);
//            ret = modeMgrManager_.notifyModeMgrStatus(idx, ModeMgrMode.NOTIFY_MODE);
//            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus notifyModeMgrStatus ret = " + ret);

            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iAudioAddr = " + modeMgrManager_.getModeMgrOnAudioAddr());
            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iVideoAddr = " + modeMgrManager_.getModeMgrOnVideoAddr());

            if (Log.isDebug()) Log.d(TAG, "requestAudioFocus notifySteeringMenuDispMode");
            notifySteeringMenuDispMode(1);

            hasAudioFocus_ = true;
        }
    }

    public void releaseAudioFocus(){
        if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus -> app with auth " + pControl_.authType);
        if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus modeMgr audio hasAudioFocus= " + hasAudioFocus_);

        // If AUTH_TYPE <> preinstall the app has already audio focus
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && hasAudioFocus_) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            int ret;

            if (Log.isVerbose())  Log.v(TAG, "releaseAudioFocus sendModeMgrOffReq idx= " + idx + ", state = " + ModeMgrMode.AUDIO_MODE);
            ret = modeMgrManager_.sendModeMgrOffReq(idx, ModeMgrMode.AUDIO_MODE);
            if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus sendModeMgrOffReq ret= " + ret);

            if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus -> wait for cond");
            waitForCond(500);

//            try {
//                Thread.sleep(200);
//            } catch (InterruptedException ignored) {
//            }

//            if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus sendModeMgrOffCnf idx= " + idx + ", state = " + ModeMgrMode.CONFIRM_MODE);
//            ret = modeMgrManager_.sendModeMgrOffCnf(idx, ModeMgrMode.CONFIRM_MODE);
//            if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus sendModeMgrOffCnf ret = " + ret);

            hasAudioFocus_ = false;
        }
    }

    public void increaseVolume(){
        if (Log.isVerbose()) Log.v(TAG, "increaseVolume");
        modeMgrManager_.reqModeMgrSteeringVolCmd(true);
    }

    public void decreaseVolume(){
        if (Log.isVerbose()) Log.v(TAG, "decreaseVolume");
        modeMgrManager_.reqModeMgrSteeringVolCmd(false);
    }

    // Used in onCreate
    public void initialize(){
        if (Log.isDebug()) Log.d(TAG, "initialize -> app with auth " + pControl_.authType);

        bindToEcNcService();
        bindToWheelService();

    }

    // Used in onResume
    public void initAudioBinding(){
        if (Log.isDebug()) Log.d(TAG, "initAudioBinding -> app with auth " + pControl_.authType);
        // if app has THIRD_PARTY auth will have exclusive audio focus, only bind wheel service
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> app auth = preinstall -> register ModeMgr and SW callback");

            registerModeMgrCallback();
            registerSteeringMenuCallback();

            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> hasAudioFocus= " + hasAudioFocus_);
            if (hasAudioFocus_){
                notifySteeringMenuDispMode(1);
            }
        } else {
            // THIRD_PARTY
            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> using authType not PREINSTALL -> register SW callback and notify");
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
            releaseAudioFocus();
            unregisterModeMgrCallback();
        }

        unregisterSteeringMenuCallback();
        unbindToWheelService();
    }

    private void notifySteeringMenuDispMode(int mode){
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && settings_.advanced.modeMgrAudioIdx() != BT_MODEMGR_ADDR) {
            if (Log.isDebug()) Log.d(TAG, "notifySteeringMenuDispMode -> modeMgrAudioIdx ( " + settings_.advanced.modeMgrAudioIdx() + " ) == BT_MODEMGR_ADDR => return");
            return;
        }

        if (Log.isDebug()) Log.d(TAG, "notifySteeringMenuDispMode -> boundToSteeringMenuService= " + boundToSteeringMenuService_);
        if (boundToSteeringMenuService_) {
            try {
                int idx = settings_.advanced.steeringWheelIdx();
                if (idx > 0) {
                    if (Log.isVerbose()) Log.v(TAG, "notifySteeringMenuDispMode " + mode + " addr " + idx);
                    steeringMenuServiceIface_.notifySteeringMenuDispMode(idx, mode);
                }
            } catch (RemoteException e) {
                Log.e(TAG, "Error registering", e);
            }
        }
    }

    private void registerSteeringMenuCallback(){
        if (Log.isDebug()) Log.d(TAG, "registerSteeringMenuCallback -> boundToSteeringMenuService= " + boundToSteeringMenuService_ + ", steeringMenuServiceCallback= " + (steeringMenuServiceCallback_ == null ? "null" : "not null"));
        try {
            if (boundToSteeringMenuService_ && steeringMenuServiceCallback_ == null) {
                int idx = settings_.advanced.steeringWheelIdx();
                if (Log.isVerbose()) Log.v(TAG, "registerCallbackEx swaddr " + idx);
                steeringMenuServiceCallback_ = new SteeringMenuServiceCallback();
                steeringMenuServiceIface_.registerCallbackEx(steeringMenuServiceCallback_, idx);
            }
        } catch (RemoteException e) {
            Log.e(TAG, "Error registerCallbackEx", e);
        }
    }

    private void unregisterSteeringMenuCallback(){
        if (Log.isDebug()) Log.d(TAG, "unregisterSteeringMenuCallback -> boundToSteeringMenuService= " + boundToSteeringMenuService_ + ", steeringMenuServiceCallback= " + (steeringMenuServiceCallback_ == null ? "null" : "not null"));
        try {
            if (boundToSteeringMenuService_ && steeringMenuServiceCallback_ != null) {
                int idx = settings_.advanced.steeringWheelIdx();
                if (Log.isVerbose()) Log.v(TAG, "unregisterCallbackEx swaddr " + idx);
                steeringMenuServiceIface_.unregisterCallbackEx(steeringMenuServiceCallback_, idx);
                steeringMenuServiceCallback_ = null;
            }
        } catch (RemoteException e) {
            Log.e(TAG, "Error unregisterCallbackEx", e);
        }
    }

    public void startMicSession() {
        if (Log.isDebug()) Log.d(TAG, "starting MicSession -> boundToEcNcService= " + boundToEcNcService_);
        if (!settings_.advanced.hondaMicVrEnabled()){
            if (Log.isDebug()) Log.d(TAG, "mic disabled");
            return;
        }

        if (boundToEcNcService_) {
            if (micVrStarted_){
                if (Log.isWarn()) Log.w(TAG, "mic session already started");
                return;
            }

            try {
                int ret = ecNcServiceIface_.startVR(true);
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
    }

    public void stopMicSession() {
        if (Log.isDebug()) Log.d(TAG, "stopping MicSession -> boundToEcNcService= " + boundToEcNcService_);
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

    private void unregisterModeMgrCallback() {
        if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback");

        if (modeMgrManager_ != null) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback idx " + idx);
            int ret = modeMgrManager_.unregisterModeMgrCallback(idx);
            if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback ret " + ret);
            modeMgrServiceCallBack_ = null;

            if (stateMgrService_ != null){
                if (Log.isVerbose()) Log.v(TAG, "stateMgrService_ unRegistCallBack");
                stateMgrService_.unRegistCallBack(stateMgrServiceCallBack_);
                stateMgrServiceCallBack_ = null;
            }
        }
    }

    private void registerModeMgrCallback(){
        if (Log.isDebug()) Log.d(TAG, "registerModeMgrCallback");
        if (modeMgrManager_ != null) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isVerbose()) Log.v(TAG, "registerModeMgrCallback idx " + idx);
            modeMgrServiceCallBack_ = new ModeMgrServiceCallBack();
            int ret = modeMgrManager_.registerModeMgrCallback(idx, modeMgrServiceCallBack_);
            if (Log.isVerbose()) Log.v(TAG, "registerModeMgrCallback ret " + ret);

            if (stateMgrService_ != null){
                StateMgrChangeInfo changeInfo = createStateMgrChangeInfo();
                stateMgrServiceCallBack_ = new StateMgrServiceCallBack();
                stateMgrService_.registCallBack(stateMgrServiceCallBack_, changeInfo);
            }
        } else {
            Log.w(TAG, "modeMgrManager_ null -> do nothing");
        }
    }

    private StateMgrChangeInfo createStateMgrChangeInfo(){
        StateMgrChangeInfo changeInfo = new StateMgrChangeInfo();
        changeInfo.audioAddressC = true;
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
        changeInfo.videoResumeCompletedC = true;
        changeInfo.lastAudioAddressC = false;
        changeInfo.audioResumeCompletedC = false;
        changeInfo.hftStateC = false;
        changeInfo.steeringDispKeyC = false;
        changeInfo.headerInterruptC = false;

        return changeInfo;
    }

    private boolean waitForCond(int timeout){
        boolean res = false;
        try {
            waitCond_ = new CountDownLatch(1);
            res = waitCond_.await(timeout, TimeUnit.MILLISECONDS);
            if (!res) {
                if (Log.isWarn()) Log.w(TAG, "timeout in waiting condition");
            }
        } catch (InterruptedException e) {
            Log.e(TAG, "error in wait condition", e);
        }

        if (Log.isVerbose()) Log.v(TAG, "received conf/notify condition");
        waitCond_ = null;

        return res;
    }

    private void startHeartbeat() {
        stopHeartbeat();

        heartbeatExecutor_ = Executors.newSingleThreadScheduledExecutor();

        heartbeatFuture_ = heartbeatExecutor_.scheduleWithFixedDelay(() -> {
            if (modeMgrManager_ != null && currentModeState_ != 0) {
                int idx = settings_.advanced.modeMgrAudioIdx();
                if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus idx= " + idx + ", state = " + currentModeState_);
                int ret = modeMgrManager_.notifyModeMgrStatus(idx, currentModeState_);
                if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus ret = " + ret);
            }
        }, HEARTBEAT_INTERVAL_MS, HEARTBEAT_INTERVAL_MS, TimeUnit.MILLISECONDS);
    }

    private void stopHeartbeat() {
        if (heartbeatExecutor_ != null) {
            heartbeatFuture_.cancel(false);
            heartbeatFuture_ = null;
        }
        if (heartbeatExecutor_ != null) {
            heartbeatExecutor_.shutdownNow();
            heartbeatExecutor_ = null;
        }
    }

    private void dumpStateMgr(StateMgrInfo info){
        if (info != null){
            if (info.updateState != null){
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.antitheftC " + info.updateState.antitheftC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.audioAddressC " + info.updateState.audioAddressC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.audioLauncherC " + info.updateState.audioLauncherC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.audioResumeCompletedC " + info.updateState.audioResumeCompletedC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.brightnessBarC " + info.updateState.brightnessBarC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.dayNightStateC " + info.updateState.dayNightStateC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.disclaimerC " + info.updateState.disclaimerC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.functionInfoC " + info.updateState.functionInfoC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.headerInterruptC " + info.updateState.headerInterruptC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.hftPopupC " + info.updateState.hftPopupC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.hftStateC " + info.updateState.hftStateC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.highTemperatureDetectionC " + info.updateState.highTemperatureDetectionC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.iMidC " + info.updateState.iMidC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.inlineDiagC " + info.updateState.inlineDiagC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.keyOffTimerAdvanceC " + info.updateState.keyOffTimerAdvanceC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.keyOffTimerExpirationC " + info.updateState.keyOffTimerExpirationC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.languageC " + info.updateState.languageC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.lastAudioAddressC " + info.updateState.lastAudioAddressC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.lastVideoAddressC " + info.updateState.lastVideoAddressC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.lcdAdjustStateC " + info.updateState.lcdAdjustStateC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.openingScreenC " + info.updateState.openingScreenC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.parkingSensorC " + info.updateState.parkingSensorC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.rdsAlarmInterruptC " + info.updateState.rdsAlarmInterruptC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.rdsInterruptC " + info.updateState.rdsInterruptC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.screenOffC " + info.updateState.screenOffC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.siriStateC " + info.updateState.siriStateC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.sourceFlowC " + info.updateState.sourceFlowC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.steeringDispKeyC " + info.updateState.steeringDispKeyC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.steeringMenuC " + info.updateState.steeringMenuC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.steeringPopUpC " + info.updateState.steeringPopUpC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.usbOvercurrentC " + info.updateState.usbOvercurrentC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.usbPopupC " + info.updateState.usbPopupC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.videoAddressC " + info.updateState.videoAddressC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.videoResumeCompletedC " + info.updateState.videoResumeCompletedC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.voiceTagStateC " + info.updateState.voiceTagStateC);
                if (Log.isVerbose()) Log.v(TAG, "StateMgrChangeInfo updateState.volumePanelC " + info.updateState.volumePanelC);
            }
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.antitheft " + info.antitheft);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.audioAddress " + info.audioAddress);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.audioLauncher " + info.audioLauncher);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.functionInfo " + info.functionInfo);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.brightnessBar " + info.brightnessBar);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.audioResumeCompleted " + info.audioResumeCompleted);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.dayNightState " + info.dayNightState);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.disclaimer " + info.disclaimer);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.headerInterrupt " + info.headerInterrupt);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.hftPopup " + info.hftPopup);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.hftState " + info.hftState);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.highTemperatureDetection " + info.highTemperatureDetection);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.iMid " + info.iMid);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.inlineDiag " + info.inlineDiag);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.keyOffTimerAdvance " + info.keyOffTimerAdvance);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.keyOffTimerExpiration " + info.keyOffTimerExpiration);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.language " + info.language);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.lastAudioAddress " + info.lastAudioAddress);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.lastVideoAddress " + info.lastVideoAddress);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.lcdAdjustState " + info.lcdAdjustState);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.openingScreen " + info.openingScreen);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.parkingSensor " + info.parkingSensor);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.rdsAlarmInterrupt " + info.rdsAlarmInterrupt);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.rdsInterrupt " + info.rdsInterrupt);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.screenOff " + info.screenOff);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.siriState " + info.siriState);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.sourceFlow " + info.sourceFlow);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.steeringDispKey " + info.steeringDispKey);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.steeringMenu " + info.steeringMenu);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.steeringPopUp " + info.steeringPopUp);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.usbOvercurrent " + info.usbOvercurrent);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.usbPopup " + info.usbPopup);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.videoAddress " + info.videoAddress);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.videoResumeCompleted " + info.videoResumeCompleted);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.voiceTagState " + info.voiceTagState);
            if (Log.isVerbose()) Log.v(TAG, "StateMgrInfo info.volumePanel " + info.volumePanel);
        }
    }

    private class SteeringMenuServiceCallback extends ISteeringMenuServiceCallback.Stub {
        private static final String TAG = "HondaConnectManager-ISteeringMenuServiceCallback";

        public void onShowView() {
            if (Log.isVerbose()) Log.v(TAG, "onShowView");
            notifySteeringMenuDispMode(1);
        }

        public boolean onFinishView(boolean flg, boolean anime) {
            if (Log.isVerbose()) Log.v(TAG, "onFinishView");
            return true;
        }

        public boolean onSteeringSWDown(int keytype) {
            if (Log.isVerbose()) Log.v(TAG, "onSteeringSWDown " + keytype);
            return true;
        }
    };

    private class StateMgrServiceCallBack extends IStateMgrServiceCallBack.Stub {
        private static final String TAG = "HondaConnectManager-IStateMgrServiceCallBack";

        public void onChangeState(StateMgrInfo info) {
            Log.v(TAG, "onChangeState");

            StateMgrChangeInfo changed = info.updateState;
            if (changed == null) return;

            dumpStateMgr(info);

            if (changed.videoResumeCompletedC) {
                Log.v(TAG, "videoResumeCompleted=" + info.videoResumeCompleted);

                if (info.videoResumeCompleted == StateMgrServiceConst.STATE_COMPLETED) {
                    Log.v(TAG, "videoResumeCompleted state completed");
                    if (info.lastVideoAddress == 92) {
                        Log.v(TAG, "videoResumeCompleted restore activity");
                        Intent i = new Intent(context_, PlayerActivity.class);
                        i.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_SINGLE_TOP);
                        context_.startActivity(i);
                    }
                }
            }
        }
    };

    private class ModeMgrServiceCallBack extends IModeMgrServiceCallBack.Stub {

        private static final String TAG = "HondaConnectManager-ModeMgrServiceCallBack";

        public void rcvOnInsCmd(int modestate) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvOnInsCmd modestate = " + modestate);
            int idx = settings_.advanced.modeMgrAudioIdx();

            if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOnCnf idx= " + idx + ",state = " + modestate);
            int ret = modeMgrManager_.sendModeMgrOnCnf(idx, modestate);
            if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOnCnf ret = " + ret);

//            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus idx= " + idx + ", state = " + HondaConnectManager.ModeMgrMode.NOTIFY_MODE);
//            ret = modeMgrManager_.notifyModeMgrStatus(idx, HondaConnectManager.ModeMgrMode.NOTIFY_MODE);
//            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus ret = " + ret);

            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iAudioAddr = " + modeMgrManager_.getModeMgrOnAudioAddr());
            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iVideoAddr = " + modeMgrManager_.getModeMgrOnVideoAddr());

            // Aggiorna il modestate e avvia/aggiorna il heartbeat
            currentModeState_ |= modestate;
            startHeartbeat();

            if (waitCond_ != null) {
                waitCond_.countDown();
            }

        }

        public void rcvOffInsCmd(int modestate) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd modestate = " + modestate);

            int sound_param = modestate & 1;
            int image_param = modestate & 2;
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd sound_param= " + sound_param + " image_param= " + image_param);

            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd sendModeMgrOffCnf idx= " + idx + ", state = " + modestate);
            int ret = modeMgrManager_.sendModeMgrOffCnf(idx, modestate);
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd sendModeMgrOffCnf ret = " + ret);

            // Rimuovi i bit revocati dallo stato corrente
            currentModeState_ &= ~modestate;

            if (currentModeState_ == 0) {
                stopHeartbeat();
            } else {
                startHeartbeat();
            }

            if (waitCond_ != null) {
                waitCond_.countDown();
            }

//            if (sound_param == 1 && image_param == 0) {
//                int ret = modeMgrManager_.sendModeMgrOffReq(settings_.advanced.modeMgrAudioIdx(), ModeMgrMode.REQUEST_MODE.mode);
//                if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOffReq ret = " + ret);
//            } else {
//                int ret = modeMgrManager_.sendModeMgrOffCnf(settings_.advanced.modeMgrAudioIdx(), ModeMgrMode.CONFIRM_MODE.mode);
//                if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOffCnf ret = " + ret);
////                ret = modeMgrManager_.unregisterModeMgrCallback(136);
////                if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback ret = " + ret);
//            }

        }

        public void rcvOnReqCmdFailed(int audioaddr, int videoaddr, int reason) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvOnReqCmdFailed audioaddr = " + audioaddr + " , videoaddr = " + videoaddr + " , reason = " + reason);
        }

        public void rcvVideoPwrCmd(int addr) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvVideoPwrCmd  addr = " + addr);
        }

        public void rcvAudioPwrONCmd(int addr) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvAudioPwrONCmd  addr = " + addr);
        }

        public void rcvAudioPwrOFFCmd() throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvAudioPwrOFFCmd -S");
        }

        public void insDispApl(int disp, int extInfo1, int extInfo2) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "insDispApl " + disp + "/" + extInfo1 + "/" + extInfo2);
        }
    };
}
