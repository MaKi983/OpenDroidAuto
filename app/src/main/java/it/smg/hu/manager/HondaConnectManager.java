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

import java.util.ArrayList;
import java.util.List;

import it.smg.hu.config.Settings;
import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.common.Log;

public class HondaConnectManager {

    public interface HondaListener {
        void onDayNightUpdate(boolean isNight);
        void onSteeringWheelKey(int keyType);
    }

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
        public static final int REQUEST_MODE = Integer.parseInt("011111", 2); //31
        public static final int CONFIRM_MODE = Integer.parseInt("111", 2); //7
        public static final int NOTIFY_MODE = Integer.parseInt("11", 2); //3
        public static final int AUDIO_MODE = Integer.parseInt("01", 2); //1
    }

    private static final String TAG = "HondaConnectManager";
    private static final String ModeMgrService = "ModeMgrService";
    private static final String StateMgrService = "StateMgrService";

    private static HondaConnectManager instance_;

    private ModeMgrManager modeMgrManager_;
    private StateMgrManager stateMgrManager_;

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
//    private CountDownLatch waitCond_;

    private final Handler mainHandler_;
    private final List<HondaListener> listeners_ = new ArrayList<>();

    private static Context appContext_;

    public static void init(Context context){
        appContext_ = context.getApplicationContext();
        instance_ = new HondaConnectManager(appContext_);
    }

    public static HondaConnectManager instance(){
        if (instance_ == null) {
            synchronized (HondaConnectManager.class) {
                if (instance_ == null && appContext_ != null) {
                    instance_ = new HondaConnectManager(appContext_);
                }
            }
        }
        return instance_;
    }

    public void addListener(HondaListener listener) {
        if (listener == null) return;
        synchronized (listeners_) {
            if (!listeners_.contains(listener)) {
                listeners_.add(listener);
            }
        }
    }

    public void removeListener(HondaListener listener) {
        if (listener == null) return;
        synchronized (listeners_) {
            listeners_.remove(listener);
        }
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

        modeMgrManager_ = null;
        stateMgrManager_ = null;

        try {
            modeMgrManager_ = (ModeMgrManager) context.getSystemService(ModeMgrService);
        } catch (Throwable t) {
            Log.e(TAG, "Could not get ModeMgrService", t);
        }

        try {
            stateMgrManager_ = (StateMgrManager) context.getSystemService(StateMgrService);
        } catch (Throwable t) {
            Log.e(TAG, "Could not get StateMgrService", t);
        }

        steeringMenuServiceConnection_ = new ServiceConnection() {
            private static final String TAG = "HondaConnectManager-steeringServiceConnection";

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                try {
                    if (Log.isVerbose()) Log.v(TAG, "Honda Wheel Service connected");
                    boundToSteeringMenuService_ = true;
                    steeringMenuServiceIface_ = ISteeringMenuService.Stub.asInterface(service);

                    int idx = settings_.advanced.steeringWheelIdx();
                    mainHandler_.post(() -> {
                        try {
                            Toast.makeText(context_.getApplicationContext(), "Wheel Service Connected (Idx: " + idx + ")", Toast.LENGTH_SHORT).show();
                        } catch (Throwable t) {}
                    });

                    registerSteeringMenuCallback();

                    if (pControl_ != null && pControl_.authType != Constants.AUTH_TYPE_PREINSTALL){
                        notifySteeringMenuDispMode(1);
                    }
                } catch (Throwable t) {
                    Log.e(TAG, "Error in onServiceConnected Wheel", t);
                }
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                if (Log.isVerbose()) Log.v(TAG, "Honda Wheel Service disconnected");
                try {
                    if (modeMgrManager_ != null) {
                        modeMgrManager_.setImidConnectStatus(0);
                    }
                } catch (Throwable t) {}
                boundToSteeringMenuService_ = false;
                steeringMenuServiceIface_ = null;
                steeringMenuServiceCallback_ = null;
            }
        };

        ecNcServiceConnection_ = new ServiceConnection() {
            private static final String TAG = "HondaConnectManager-ecNcServiceConnection";

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                try {
                    if (Log.isVerbose()) Log.v(TAG, "Honda EcNc Service connected");
                    boundToEcNcService_ = true;
                    ecNcServiceIface_ = IEcNcService.Stub.asInterface(service);
                } catch (Throwable t) {
                    Log.e(TAG, "Error in onServiceConnected EcNc", t);
                }
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                if (Log.isVerbose()) Log.v(TAG, "Honda EcNc Service disconnected");
                try {
                    if (modeMgrManager_ != null) {
                        modeMgrManager_.setImidConnectStatus(0);
                    }
                } catch (Throwable t) {}
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
        if (pControl_ != null && pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
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
        if (pControl_ == null) {
            if (Log.isWarn()) Log.w(TAG, "requestAudioFocus -> pControl_ is null");
            return;
        }
        if (modeMgrManager_ == null) {
            if (Log.isWarn()) Log.w(TAG, "requestAudioFocus -> modeMgrManager_ is null");
            return;
        }
        if (Log.isDebug()) Log.d(TAG, "requestAudioFocus -> app with auth " + pControl_.authType);
        if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus modeMgr audio hasAudioFocus= " + hasAudioFocus_);

        // If AUTH_TYPE <> preinstall the app has already audio focus
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && !hasAudioFocus_) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            int ret;

            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnReq idx= " + idx + ", mode= " + ModeMgrMode.REQUEST_MODE);
            ret = modeMgrManager_.sendModeMgrOnReq(idx, ModeMgrMode.REQUEST_MODE);
            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnReq result= " + ret);

            if (Log.isDebug()) Log.d(TAG, "requestAudioFocus notifySteeringMenuDispMode");
            notifySteeringMenuDispMode(1);

            try {
                modeMgrManager_.setImidConnectStatus(1);
            } catch (Throwable t) {
                Log.w(TAG, "Could not set i-MID connect status", t);
            }

            hasAudioFocus_ = true;
        }
    }

    public void releaseAudioFocus(){
        if (pControl_ == null) {
            if (Log.isWarn()) Log.w(TAG, "releaseAudioFocus -> pControl_ is null");
            return;
        }
        if (modeMgrManager_ == null) {
            if (Log.isWarn()) Log.w(TAG, "releaseAudioFocus -> modeMgrManager_ is null");
            return;
        }
        if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus -> app with auth " + pControl_.authType);
        if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus modeMgr audio hasAudioFocus= " + hasAudioFocus_);

        // If AUTH_TYPE <> preinstall the app has already audio focus
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL && hasAudioFocus_) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            int ret;

            try {
                if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus -> resetting i-MID and Steering Display");
                modeMgrManager_.setImidConnectStatus(0);
                notifySteeringMenuDispMode(0);
            } catch (Throwable t) {
                Log.w(TAG, "Could not reset i-MID/Steering status", t);
            }

            if (Log.isVerbose())  Log.v(TAG, "releaseAudioFocus sendModeMgrOffReq idx= " + idx + ", state = " + ModeMgrMode.REQUEST_MODE);
            ret = modeMgrManager_.sendModeMgrOffReq(idx, ModeMgrMode.REQUEST_MODE);
            if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus sendModeMgrOffReq ret= " + ret);

            hasAudioFocus_ = false;
        }
    }

    public void increaseVolume(){
        if (Log.isVerbose()) Log.v(TAG, "increaseVolume");
        if (modeMgrManager_ != null) {
            modeMgrManager_.reqModeMgrSteeringVolCmd(true);
        }
    }

    public void decreaseVolume(){
        if (Log.isVerbose()) Log.v(TAG, "decreaseVolume");
        if (modeMgrManager_ != null) {
            modeMgrManager_.reqModeMgrSteeringVolCmd(false);
        }
    }

    // Used in onCreate
    public void initialize(){
        if (pControl_ != null) {
            if (Log.isDebug()) Log.d(TAG, "initialize -> app with auth " + pControl_.authType);
        } else {
            if (Log.isDebug()) Log.d(TAG, "initialize -> pControl_ is null");
        }

        bindToEcNcService();
        bindToWheelService();

    }

    // Used in onResume
    public void initAudioBinding(){
        if (pControl_ == null) {
            if (Log.isWarn()) Log.w(TAG, "initAudioBinding -> pControl_ is null");
            return;
        }
        if (Log.isDebug()) Log.d(TAG, "initAudioBinding -> app with auth " + pControl_.authType);
        // if app has THIRD_PARTY auth will have exclusive audio focus, only bind wheel service
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> app auth = preinstall -> register ModeMgr and SW callback");

            registerModeMgrCallback();
            registerSteeringMenuCallback();
            registerStateMgrCallback();

            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> hasAudioFocus= " + hasAudioFocus_);
            if (hasAudioFocus_){
                notifySteeringMenuDispMode(1);
            }
        } else {
            // THIRD_PARTY
            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> using authType not PREINSTALL -> register SW callback and notify");
            registerSteeringMenuCallback();
            registerStateMgrCallback();
            notifySteeringMenuDispMode(1);
        }
    }

    // Used in onPause (app in background)
    public void sendToBackground(){
        if (pControl_ != null) {
            if (Log.isDebug()) Log.d(TAG, "sendToBackground -> app with auth " + pControl_.authType + " unregister SW callback");
        }
       unregisterSteeringMenuCallback();
       unregisterStateMgrCallback();
       notifySteeringMenuDispMode(0);
    }

    public void endAudioBinding(){
        if (pControl_ != null) {
            if (Log.isDebug()) Log.d(TAG, "endAudioBinding -> app with auth " + pControl_.authType);
        }

        stopMicSession();
        unbindFromEcNcService();

        if (pControl_ != null && pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            if (Log.isDebug()) Log.d(TAG, "endAudioBinding -> auth PREINSTALL -> release audio and unregister modemgr callback");
            releaseAudioFocus();
            unregisterModeMgrCallback();
        }

        unregisterSteeringMenuCallback();
        unregisterStateMgrCallback();
        unbindToWheelService();
    }

    private void notifySteeringMenuDispMode(int mode){
        if (Log.isDebug()) Log.d(TAG, "notifySteeringMenuDispMode -> boundToSteeringMenuService= " + boundToSteeringMenuService_);
        if (boundToSteeringMenuService_ && steeringMenuServiceIface_ != null) {
            try {
                if (settings_ != null && settings_.advanced != null) {
                    int idx = settings_.advanced.steeringWheelIdx();
                    if (idx > 0) {
                        if (Log.isVerbose()) Log.v(TAG, "notifySteeringMenuDispMode " + mode + " addr " + idx);
                        steeringMenuServiceIface_.notifySteeringMenuDispMode(idx, mode);
                    }
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
            if (boundToSteeringMenuService_ && steeringMenuServiceIface_ != null && steeringMenuServiceCallback_ != null) {
                int idx = settings_.advanced.steeringWheelIdx();
                if (Log.isVerbose()) Log.v(TAG, "unregisterCallbackEx swaddr " + idx);
                steeringMenuServiceIface_.unregisterCallbackEx(steeringMenuServiceCallback_, idx);
                steeringMenuServiceCallback_ = null;
            }
        } catch (Throwable t) {
            Log.e(TAG, "Error unregisterCallbackEx", t);
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
        if (modeMgrManager_ == null) return;

        int idx = settings_.advanced.modeMgrAudioIdx();
        if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback idx " + idx);
        int ret = modeMgrManager_.unregisterModeMgrCallback(idx);
        if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback ret " + ret);

        modeMgrServiceCallBack_ = null;
    }

    private void registerModeMgrCallback(){
        if (Log.isDebug()) Log.d(TAG, "registerModeMgrCallback");
        if (modeMgrManager_ != null) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isVerbose()) Log.v(TAG, "registerModeMgrCallback idx " + idx);
            modeMgrServiceCallBack_ = new ModeMgrServiceCallBack();
            int ret = modeMgrManager_.registerModeMgrCallback(idx, modeMgrServiceCallBack_);
            if (Log.isVerbose()) Log.v(TAG, "registerModeMgrCallback ret " + ret);
        } else {
            Log.w(TAG, "modeMgrManager_ null -> do nothing");
        }
    }

    private void registerStateMgrCallback() {
        if (Log.isDebug()) Log.d(TAG, "registerStateMgrCallback");
        if (stateMgrManager_ != null && stateMgrServiceCallBack_ == null) {
            stateMgrServiceCallBack_ = new StateMgrServiceCallBack();
            StateMgrChangeInfo info = new StateMgrChangeInfo();
            info.dayNightStateC = true;
            int ret = stateMgrManager_.registCallBack(stateMgrServiceCallBack_, info);
            if (Log.isVerbose()) Log.v(TAG, "registerStateMgrCallback ret " + ret);

            // Initial state check
            StateMgrInfo currentState = stateMgrManager_.getAllState();
            if (currentState != null) {
                boolean isNight = currentState.dayNightState == StateMgrServiceConst.STATE_NIGHT;
                List<HondaListener> listenersCopy;
                synchronized (listeners_) {
                    listenersCopy = new ArrayList<>(listeners_);
                }
                for (HondaListener l : listenersCopy) l.onDayNightUpdate(isNight);
            }
        }
    }

    private void unregisterStateMgrCallback() {
        if (Log.isDebug()) Log.d(TAG, "unregisterStateMgrCallback");
        if (stateMgrManager_ != null && stateMgrServiceCallBack_ != null) {
            stateMgrManager_.unRegistCallBack(stateMgrServiceCallBack_);
            stateMgrServiceCallBack_ = null;
        }
    }

//    private boolean waitForCond(int timeout){
//        boolean res = false;
//        try {
//            waitCond_ = new CountDownLatch(1);
//            res = waitCond_.await(timeout, TimeUnit.MILLISECONDS);
//            if (!res) {
//                if (Log.isWarn()) Log.w(TAG, "timeout in waiting condition");
//            }
//        } catch (InterruptedException e) {
//            Log.e(TAG, "error in wait condition", e);
//        }
//
//        if (Log.isVerbose()) Log.v(TAG, "received conf/notify condition");
//        waitCond_ = null;
//
//        return res;
//    }

    private class SteeringMenuServiceCallback extends ISteeringMenuServiceCallback.Stub {
        private static final String TAG = "HondaConnectManager-ISteeringMenuServiceCallback";

        @Override
        public void onShowView() throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "onShowView");
            notifySteeringMenuDispMode(1);
        }

        @Override
        public boolean onFinishView(boolean flg, boolean anime) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "onFinishView");
            return true;
        }

        @Override
        public boolean onSteeringSWDown(int keytype) throws RemoteException {
            if (Log.isDebug()) Log.d(TAG, "onSteeringSWDown " + keytype);
            
            List<HondaListener> listenersCopy;
            synchronized (listeners_) {
                listenersCopy = new ArrayList<>(listeners_);
            }
            for (HondaListener l : listenersCopy) {
                try {
                    l.onSteeringWheelKey(keytype);
                } catch (Throwable t) {
                    Log.e(TAG, "Error in listener callback", t);
                }
            }
            return true;
        }
    };

    private class StateMgrServiceCallBack extends IStateMgrServiceCallBack.Stub {
        private static final String TAG = "HondaConnectManager-StateMgrServiceCallBack";

        @Override
        public void onChangeState(StateMgrInfo stateMgrInfo) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "onChangeState");
            if (stateMgrInfo.updateState.dayNightStateC) {
                boolean isNight = stateMgrInfo.dayNightState == StateMgrServiceConst.STATE_NIGHT;
                if (Log.isDebug()) Log.d(TAG, "DayNight state changed, isNight: " + isNight);
                List<HondaListener> listenersCopy;
                synchronized (listeners_) {
                    listenersCopy = new ArrayList<>(listeners_);
                }
                for (HondaListener l : listenersCopy) {
                    l.onDayNightUpdate(isNight);
                }
            }
        }
    }

    private class ModeMgrServiceCallBack extends IModeMgrServiceCallBack.Stub {

        private static final String TAG = "HondaConnectManager-ModeMgrServiceCallBack";

        public void rcvOnInsCmd(int modestate) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvOnInsCmd modestate = " + modestate);
            int idx = settings_.advanced.modeMgrAudioIdx();

            if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOnCnf idx= " + idx + ",state = " + HondaConnectManager.ModeMgrMode.CONFIRM_MODE);
            int ret = modeMgrManager_.sendModeMgrOnCnf(idx, HondaConnectManager.ModeMgrMode.CONFIRM_MODE);
            if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOnCnf ret = " + ret);

            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus idx= " + idx + ", state = " + HondaConnectManager.ModeMgrMode.NOTIFY_MODE);
            ret = modeMgrManager_.notifyModeMgrStatus(idx, HondaConnectManager.ModeMgrMode.NOTIFY_MODE);
            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus ret = " + ret);

            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iAudioAddr = " + modeMgrManager_.getModeMgrOnAudioAddr());
            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iVideoAddr = " + modeMgrManager_.getModeMgrOnVideoAddr());

//            if (waitCond_ != null) {
//                waitCond_.countDown();
//            }

        }

        public void rcvOffInsCmd(int modestate) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd modestate = " + modestate);

            int sound_param = modestate & 1;
            int image_param = modestate & 2;
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd sound_param= " + sound_param + " image_param= " + image_param);

            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd sendModeMgrOffCnf idx= " + idx + ", state = " + HondaConnectManager.ModeMgrMode.CONFIRM_MODE);
            int ret = modeMgrManager_.sendModeMgrOffCnf(idx, HondaConnectManager.ModeMgrMode.CONFIRM_MODE);
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd sendModeMgrOffCnf ret = " + ret);

//            if (waitCond_ != null) {
//                waitCond_.countDown();
//            }

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

