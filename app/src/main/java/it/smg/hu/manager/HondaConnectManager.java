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

import com.fujitsu_ten.displayaudio.modemanagement.IModeMgrManager;
import com.fujitsu_ten.displayaudio.modemanagement.IModeMgrServiceCallBack;
import com.fujitsu_ten.displayaudio.modemanagement.IModeMgrServiceSWKeyEventCallBack;
import com.fujitsu_ten.displayaudio.modemanagement.ModeMgrManager;
import com.fujitsu_ten.displayaudio.oom.OomManager;
import com.fujitsu_ten.displayaudio.steeringmenuservice.service.ISteeringMenuService;
import com.fujitsu_ten.displayaudio.steeringmenuservice.service.ISteeringMenuServiceCallback;
import com.fujitsu_ten.displayaudio.whitelist.common.Constants;
import com.fujitsu_ten.displayaudio.whitelist.common.IWhiteList;
import com.fujitsu_ten.displayaudio.whitelist.common.ProcessControl;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import it.smg.hu.config.Settings;
//import it.smg.hu.manager.steeringmenu.SteeringMenuServiceCallback;
import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.common.Log;

public class HondaConnectManager {

    public static class SWMode {
        public static final String SW_SERVICE = "SW SERVICE";
        public static final String MODEMGR_KEY_CALLBACK = "MODEMGR KEY CALLBACK";
    }

    public static class  AudioStreamType {
        public static final int ADA_NORMAL = 11;
        public static final int ADA_INTERRUPT = 12;
        public static final int ADA_INTERRUPT_LP = 13;
        public static final int ADA_INTERRUPT_VR = 14;
        public static final int ADA_INTERRUPT_NAVI = 15;
    }

    private static class ModeMgrMode {
        public static final int REQUEST_MODE = Integer.parseInt("011111", 2); //31
        public static final int CONFIRM_MODE = Integer.parseInt("111", 2); //7
        public static final int NOTIFY_MODE = Integer.parseInt("11", 2); //3
        public static final int AUDIO_MODE = Integer.parseInt("01", 2); //1
    }

    private static final String TAG = "HondaConnectManager";
    private static final String ModeMgrService = "ModeMgrService";
    private static final String StateMgrService = "StateMgrService";

    private static HondaConnectManager instance_;

    private final ModeMgrManager modeMgrManager_;

    // SteeringWheel service
    private ISteeringMenuService steeringMenuServiceIface_;
    private final ServiceConnection steeringMenuServiceConnection_;
//    private SteeringMenuServiceCallback steeringMenuServiceCallback_;
    private boolean boundToWheelService_;

    private final Context context_;
    private final Settings settings_;

    private boolean hasAudioFocus_;
    private boolean boundToModeMgrService_;
    private ProcessControl pControl_;
//    private CountDownLatch waitCond_;

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
        boundToModeMgrService_ = false;
        mainHandler_ = new Handler(Looper.getMainLooper());

        modeMgrManager_ = (ModeMgrManager) context.getSystemService(ModeMgrService);
        if (modeMgrManager_ == null){
            if (Log.isWarn()) Log.w(TAG, "modeMgrManager null");
        }

        steeringMenuServiceConnection_ = new ServiceConnection() {
            private static final String TAG = "HondaConnectManager-steeringServiceConnection";

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                if (Log.isVerbose()) Log.v(TAG, "Honda Wheel Service connected");
                boundToWheelService_ = true;
                steeringMenuServiceIface_ = ISteeringMenuService.Stub.asInterface(service);
                try {
//                    steeringMenuServiceCallback_ = new SteeringMenuServiceCallback();

                    int idx = settings_.advanced.steeringWheelIdx();
                    if (idx > 0){
                        if (Log.isVerbose()) Log.v(TAG, "notifySteeringMenuDispMode 1 addr " + idx);
                        steeringMenuServiceIface_.notifySteeringMenuDispMode(idx, 1);
                        if (Log.isVerbose()) Log.v(TAG, "registerCallbackEx swaddr " + idx);
                        steeringMenuServiceIface_.registerCallbackEx(steeringMenuServiceCallback_, idx);
                    }

                } catch (RemoteException e) {
                    Log.e(TAG, "Error registering", e);
                    boundToWheelService_ = false;
                }
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                if (Log.isVerbose()) Log.v(TAG, "Honda Wheel Service disconnected");
                try {
                    if (boundToWheelService_) {
                        int idx = settings_.advanced.steeringWheelIdx();
                        if (idx > 0) {
                            if (Log.isVerbose())  Log.v(TAG, "notifySteeringMenuDispMode 0 addr " + idx);
                            steeringMenuServiceIface_.notifySteeringMenuDispMode(idx, 0);
                            if (Log.isVerbose()) Log.v(TAG, "unregisterCallbackEx swaddr " + idx);
                            steeringMenuServiceIface_.unregisterCallbackEx(steeringMenuServiceCallback_, idx);
//                            steeringMenuServiceCallback_ = null;
                        }
                    }
                } catch (RemoteException e) {
                    Log.e(TAG, "Error unregistering ", e);
                }
                boundToWheelService_ = false;
                steeringMenuServiceIface_ = null;
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

            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnReq idx= " + idx + ", mode= " + ModeMgrMode.REQUEST_MODE);
            ret = modeMgrManager_.sendModeMgrOnReq(idx, ModeMgrMode.REQUEST_MODE);
            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnReq result= " + ret);

            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
            }

            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnCnf idx= " + idx + ",state = " + ModeMgrMode.CONFIRM_MODE);
            ret = modeMgrManager_.sendModeMgrOnCnf(idx, ModeMgrMode.CONFIRM_MODE);
            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus sendModeMgrOnCnf ret = " + ret);

            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus notifyModeMgrStatus idx= " + idx + ", state = " + ModeMgrMode.NOTIFY_MODE);
            ret = modeMgrManager_.notifyModeMgrStatus(idx, ModeMgrMode.NOTIFY_MODE);
            if (Log.isVerbose()) Log.v(TAG, "requestAudioFocus notifyModeMgrStatus ret = " + ret);

            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iAudioAddr = " + modeMgrManager_.getModeMgrOnAudioAddr());
            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus iVideoAddr = " + modeMgrManager_.getModeMgrOnVideoAddr());

//            if (Log.isDebug()) Log.d(TAG, "requestAudioFocus -> wait for cond");
//            waitForCond(500);

            if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.SW_SERVICE)) {
                if (Log.isDebug()) Log.d(TAG, "requestAudioFocus -> using authType PREINSTALL and swMode SW_SERVICE -> bind to sw service");
                bindToWheelService();
            } else if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.MODEMGR_KEY_CALLBACK)) {
                if (Log.isDebug()) Log.d(TAG, "requestAudioFocus -> using authType PREINSTALL and swMode MODEMGR_KEY_CALLBACK -> register ModeMgr key event");
                registerModeMgrSWEvent();
            } else {
                if (Log.isWarn()) Log.w(TAG, "requestAudioFocus -> wrong steeringwheel mode " + settings_.advanced.swMode());
            }

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

            if (Log.isVerbose())  Log.v(TAG, "releaseAudioFocus sendModeMgrOffReq idx= " + idx + ", state = " + ModeMgrMode.REQUEST_MODE);
            ret = modeMgrManager_.sendModeMgrOffReq(idx, ModeMgrMode.REQUEST_MODE);
            if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus sendModeMgrOffReq ret= " + ret);

//            if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus -> wait for cond");
//            waitForCond(500);

            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
            }

            if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus sendModeMgrOffCnf idx= " + idx + ", state = " + ModeMgrMode.CONFIRM_MODE);
            ret = modeMgrManager_.sendModeMgrOffCnf(idx, ModeMgrMode.CONFIRM_MODE);
            if (Log.isVerbose()) Log.v(TAG, "releaseAudioFocus sendModeMgrOffCnf ret = " + ret);

            if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.SW_SERVICE)) {
                if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus -> using authType PREINSTALL and swMode SW_SERVICE -> unbind to sw service");
                unbindToWheelService();
            } else if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.MODEMGR_KEY_CALLBACK)) {
                if (Log.isDebug()) Log.d(TAG, "releaseAudioFocus -> using authType PREINSTALL and swMode MODEMGR_KEY_CALLBACK -> unregister ModeMgr key event");
                unregisterModeMgrSWEvent();
            } else {
                if (Log.isWarn()) Log.w(TAG, "releaseAudioFocus -> wrong steeringwheel mode " + settings_.advanced.swMode());
            }

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

    // Used in onResume
    public void initAudioBinding(){
        if (Log.isDebug()) Log.d(TAG, "initAudioBinding -> app with auth " + pControl_.authType);
        // if app has THIRD_PARTY auth will have exclusive audio focus, only bind wheel service
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> app auth = preinstall");
            bindToModeMgrService();
        } else {
            if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> app auth not preinstall");

            if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.SW_SERVICE)) {
                if (Log.isVerbose()) Log.v(TAG, "initAudioBinding -> using authType not PREINSTALL and swMode SW_SERVICE -> bind HC sw service");
                bindToWheelService();
            } else if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.MODEMGR_KEY_CALLBACK)) {
                if (Log.isWarn()) Log.w(TAG, "initAudioBinding -> using authType not PREINSTALL and swMode MODEMGR_KEY_CALLBACK -> USE MODEMGR_KEY_CALLBACK ONLY WITH PREINSTALL");
            } else {
                if (Log.isWarn()) Log.w(TAG, "initAudioBinding -> wrong steeringwheel mode " + settings_.advanced.swMode());
            }
        }
    }

    // Used in onPause (app in background)
    public void sendToBackground(){
        if (Log.isDebug()) Log.d(TAG, "sendToBackground -> app with auth " + pControl_.authType);
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.SW_SERVICE)) {
                if (Log.isVerbose()) Log.v(TAG, "sendToBackground -> using authType PREINSTALL and swMode SW_SERVICE -> unbind HC sw service");
                unbindToWheelService();
            } else if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.MODEMGR_KEY_CALLBACK)) {
                if (Log.isVerbose()) Log.v(TAG, "sendToBackground -> using authType PREINSTALL and swMode MODEMGR_KEY_CALLBACK -> do nothing");
            } else {
                if (Log.isWarn()) Log.w(TAG, "sendToBackground -> wrong steeringwheel mode " + settings_.advanced.swMode());
            }
        } else {
            if (Log.isVerbose()) Log.v(TAG, "sendToBackground -> app auth not preinstall");
            if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.SW_SERVICE)) {
                if (Log.isVerbose()) Log.d(TAG, "sendToBackground -> using authType not PREINSTALL and swMode SW_SERVICE -> unbind HC sw service");
                unbindToWheelService();
            } else if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.MODEMGR_KEY_CALLBACK)) {
                if (Log.isWarn()) Log.w(TAG, "sendToBackground -> using authType not PREINSTALL and swMode MODEMGR_KEY_CALLBACK -> USE MODEMGR_KEY_CALLBACK ONLY WITH PREINSTALL");
            } else {
                if (Log.isWarn()) Log.w(TAG, "sendToBackground -> wrong steeringwheel mode " + settings_.advanced.swMode());
            }
        }
    }

    public void endAudioBinding(){
        if (Log.isDebug()) Log.d(TAG, "endAudioBinding -> app with auth " + pControl_.authType);
        if (pControl_.authType == Constants.AUTH_TYPE_PREINSTALL){
            releaseAudioFocus();
            unbindToModeMgrService();
        } else {
            if (Log.isVerbose()) Log.v(TAG, "endAudioBinding -> app auth not preinstall");
            if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.SW_SERVICE)) {
                if (Log.isVerbose()) Log.v(TAG, "endAudioBinding -> using authType not PREINSTALL and swMode SW_SERVICE -> unbind HC sw service");
                unbindToWheelService();
            } else if (settings_.advanced.swMode().equalsIgnoreCase(SWMode.MODEMGR_KEY_CALLBACK)) {
                if (Log.isWarn()) Log.w(TAG, "endAudioBinding -> using authType not PREINSTALL and swMode MODEMGR_KEY_CALLBACK -> USE MODEMGR_KEY_CALLBACK ONLY WITH PREINSTALL");
            } else {
                if (Log.isWarn()) Log.w(TAG, "endAudioBinding -> wrong steeringwheel mode " + settings_.advanced.swMode());
            }
        }
    }


    private void bindToWheelService(){
        if (!boundToWheelService_) {
            if (Log.isDebug()) Log.d(TAG, "Request binding to service " + ISteeringMenuService.class.getName());
            Intent intent = new Intent(ISteeringMenuService.class.getName());
            context_.bindService(intent, steeringMenuServiceConnection_, Context.BIND_AUTO_CREATE);
        }
    }

    private void registerModeMgrSWEvent(){
        if (!boundToWheelService_) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isDebug()) Log.d(TAG, "registerModeMgrSWKeyEventCallback idx " + idx);
            int ret = modeMgrManager_.registerModeMgrSWKeyEventCallback(idx, modeMgrServiceSWKeyEventCallBack_);
            if (Log.isDebug()) Log.d(TAG, "registerModeMgrSWKeyEventCallback ret " + ret);
            boundToWheelService_ = true;
        }
    }

    private void unregisterModeMgrSWEvent(){
        if (boundToWheelService_) {
            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isDebug()) Log.d(TAG, "unregisterModeMgrSWKeyEventCallback idx " + idx);
            int ret = modeMgrManager_.unregisterModeMgrSWKeyEventCallback(idx);
            if (Log.isDebug()) Log.d(TAG, "unregisterModeMgrSWKeyEventCallback ret " + ret);
            boundToWheelService_ = false;
        }
    }

    private void unbindToWheelService(){
        if (boundToWheelService_) {
            if (Log.isDebug()) Log.d(TAG, "Request unbinding to service " + ISteeringMenuService.class.getName());
            context_.unbindService(steeringMenuServiceConnection_);

            boundToWheelService_ = false;
            try {
                int idx = settings_.advanced.steeringWheelIdx();
                Log.v(TAG, "idx " + idx);
                if (idx > 0){
                    Log.v(TAG, "notifySteeringMenuDispMode 0 addr " + idx);
                    steeringMenuServiceIface_.notifySteeringMenuDispMode(idx, 0);
                    Log.v(TAG, "unregisterCallbackEx swaddr " + idx);
                    steeringMenuServiceIface_.unregisterCallbackEx(steeringMenuServiceCallback_, idx);
                }
            } catch (RemoteException e) {
                Log.e(TAG, "errore", e);
            }
            steeringMenuServiceIface_ = null;
        }
    }

    private void unbindToModeMgrService() {
        if (boundToModeMgrService_) {
            if (Log.isVerbose()) Log.v(TAG, "Request unbinding to service " + IModeMgrManager.class.getName());

            int idx = settings_.advanced.modeMgrAudioIdx();
            if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback idx " + idx);
            int ret = modeMgrManager_.unregisterModeMgrCallback(idx);
            if (Log.isVerbose()) Log.v(TAG, "unregisterModeMgrCallback ret " + ret);

            boundToModeMgrService_ = false;
        }
    }

    private void bindToModeMgrService(){
        if (!boundToModeMgrService_) {
            if (Log.isDebug()) Log.d(TAG, "Request binding to service " + IModeMgrManager.class.getName());
            if (modeMgrManager_ != null) {
                boundToModeMgrService_ = true;
                int idx = settings_.advanced.modeMgrAudioIdx();
                if (Log.isVerbose()) Log.v(TAG, "registerModeMgrCallback idx " + idx);
                int ret = modeMgrManager_.registerModeMgrCallback(idx, modeMgrServiceCallBack_);
                if (Log.isVerbose()) Log.v(TAG, "registerModeMgrCallback ret " + ret);
            } else {
                Log.w(TAG, "modeMgrManager_ null -> do nothing");
            }
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

    private final IModeMgrServiceSWKeyEventCallBack.Stub modeMgrServiceSWKeyEventCallBack_ = new IModeMgrServiceSWKeyEventCallBack.Stub() {
        private static final String TAG = "HondaConnectManager-IModeMgrServiceSWKeyEventCallBack";

        @Override
        public void rcvStrgKeyEvent(int key, int action) throws RemoteException {
            Log.v(TAG, "rcvStrgKeyEvent " + key + "/" + action);

            if (Log.isDebug()) {
                mainHandler_.post(() -> {
                    Toast.makeText(context_, "rcvStrgKeyEvent key: " + key + " action: " + action, Toast.LENGTH_SHORT).show();
                });
            }
        }
    };

    private final IModeMgrServiceCallBack modeMgrServiceCallBack_ = new IModeMgrServiceCallBack.Stub() {
        private static final String TAG = "HondaConnectManager-IModeMgrServiceCallBack";
        public void rcvOnInsCmd(int modestate) throws RemoteException {
            if (Log.isVerbose()) Log.v(TAG, "rcvOnInsCmd modestate = " + modestate);
            int idx = settings_.advanced.modeMgrAudioIdx();

            if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOnCnf idx= " + idx + ",state = " + ModeMgrMode.CONFIRM_MODE);
            int ret = modeMgrManager_.sendModeMgrOnCnf(idx, ModeMgrMode.CONFIRM_MODE);
            if (Log.isVerbose()) Log.v(TAG, "sendModeMgrOnCnf ret = " + ret);

            if (Log.isVerbose()) Log.v(TAG, "notifyModeMgrStatus idx= " + idx + ", state = " + ModeMgrMode.NOTIFY_MODE);
            ret = modeMgrManager_.notifyModeMgrStatus(idx, ModeMgrMode.NOTIFY_MODE);
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
            if (Log.isVerbose()) Log.v(TAG, "rcvOffInsCmd sendModeMgrOffCnf idx= " + idx + ", state = " + ModeMgrMode.CONFIRM_MODE);
            int ret = modeMgrManager_.sendModeMgrOffCnf(idx, ModeMgrMode.CONFIRM_MODE);
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

    private final ISteeringMenuServiceCallback steeringMenuServiceCallback_ = new ISteeringMenuServiceCallback.Stub() {
        private static final String TAG = "HondaConnectManager-ISteeringMenuServiceCallback";

        public void onShowView() {
            Log.v(TAG, "onShowView");
        }

        public boolean onFinishView(boolean flg, boolean anime) {
            Log.v(TAG, "onFinishView");
            return true;
        }

        public boolean onSteeringSWDown(int keytype) {
            Log.v(TAG, "onSteeringSWDown " + keytype);
            return true;
        }
    };
}
