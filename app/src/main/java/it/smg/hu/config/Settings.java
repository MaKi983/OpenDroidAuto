package it.smg.hu.config;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import androidx.annotation.Keep;

import it.smg.libs.common.ILog;
import it.smg.libs.aasdk.configuration.ICarConfiguration;

//import it.smg.hu.oda.service.input.KeyCode;
//import it.smg.hu.oda.service.sensor.IDayNightSensor;

public class Settings {

    private static final String TAG = "Settings";

    private SharedPreferences SP;

    public Car car;
    public Keymap keymap;
    public Advanced advanced;
    public Video video;
    public Audio audio;
    public Connectivity connectivity;

    private static Settings settings = null;

    public static Settings instance(){
        return settings;
    }

    public static void build(Context context){
        settings = new Settings(context);
    }

    private Settings(Context context) {
        SP = PreferenceManager.getDefaultSharedPreferences(context);

        car = new Car();
        keymap = new Keymap();
        connectivity = new Connectivity();
        advanced = new Advanced();
        video = new Video();
        audio = new Audio();
    }

    public class Car extends Base implements ICarConfiguration {
        public final static String CAR_SETTINGS_HU_NAME = "hu_name";
        public final static String CAR_SETTINGS_MODEL = "car_model";
        public final static String CAR_SETTINGS_YEAR = "car_year";
        public final static String CAR_SETTINGS_HU_MAKE = "hu_make";
        public final static String CAR_SETTINGS_HU_MODEL = "hu_model";
        public final static String CAR_SETTINGS_SW_VERSION = "sw_version";
        public final static String CAR_SETTINGS_SW_BUILD = "sw_build";
        public final static String CAR_SETTINGS_PLAY_MEDIA_DURING_VR = "play_media_during_vr";
        public final static String CAR_SETTINGS_HIDE_CLOCK = "hide_clock";
        public final static String CAR_SETTINGS_LEFT_HAND_DRIVE = "left_drive";

        public final static String CAR_SETTINGS_HU_NAME_DEFAULT_VALUE = "Honda";
        public final static String CAR_SETTINGS_MODEL_DEFAULT_VALUE = "HRV";
        public final static String CAR_SETTINGS_YEAR_DEFAULT_VALUE = "2017";
        public final static String CAR_SETTINGS_HU_MAKE_DEFAULT_VALUE = "Honda";
        public final static String CAR_SETTINGS_HU_MODEL_DEFAULT_VALUE = "Honda Connect";
        public final static String CAR_SETTINGS_SW_VERSION_DEFAULT_VALUE = "SWB1";
        public final static String CAR_SETTINGS_SW_BUILD_DEFAULT_VALUE = "SWV1";
        public final static boolean CAR_SETTINGS_PLAY_MEDIA_DURING_VR_DEFAULT_VALUE = true;
        public final static boolean CAR_SETTINGS_HIDE_CLOCK_DEFAULT_VALUE = false;
        public final static boolean CAR_SETTINGS_LEFT_HAND_DRIVE_DEFAULT_VALUE = true;

        @Keep
        public String huName() {
            return get(CAR_SETTINGS_HU_NAME, CAR_SETTINGS_HU_NAME_DEFAULT_VALUE);
        }

        public void huName(String value){
            set(CAR_SETTINGS_HU_NAME, value);
        }

        @Keep
        public String model() {
            return get(CAR_SETTINGS_MODEL, CAR_SETTINGS_MODEL_DEFAULT_VALUE);
        }

        public void model(String value){
            set(CAR_SETTINGS_MODEL, value);
        }

        @Keep
        public String year() {
            return get(CAR_SETTINGS_YEAR, CAR_SETTINGS_YEAR_DEFAULT_VALUE);
        }

        public void year(String value){
            set(CAR_SETTINGS_YEAR, value);
        }

        @Keep
        public String huMake() {
            return get(CAR_SETTINGS_HU_MAKE, CAR_SETTINGS_HU_MAKE_DEFAULT_VALUE);
        }

        public void huMake(String value){
            set(CAR_SETTINGS_HU_MAKE, value);
        }

        @Keep
        public String huModel() {
            return get(CAR_SETTINGS_HU_MODEL, CAR_SETTINGS_HU_MODEL_DEFAULT_VALUE);
        }

        public void huModel(String value){
            set(CAR_SETTINGS_HU_MODEL, value);
        }

        @Keep
        public String swVersion() {
            return get(CAR_SETTINGS_SW_VERSION, CAR_SETTINGS_SW_VERSION_DEFAULT_VALUE);
        }

        public void swVersion(String value){
            set(CAR_SETTINGS_SW_VERSION, value);
        }

        @Keep
        public String swBuild() {
            return get(CAR_SETTINGS_SW_BUILD, CAR_SETTINGS_SW_BUILD_DEFAULT_VALUE);
        }

        public void swBuild(String value){
            set(CAR_SETTINGS_SW_BUILD, value);
        }

        @Keep
        public boolean playMediaDuringVr() {
            return get(CAR_SETTINGS_PLAY_MEDIA_DURING_VR, CAR_SETTINGS_PLAY_MEDIA_DURING_VR_DEFAULT_VALUE);
        }

        public void playMediaDuringVr(boolean value){
            set(CAR_SETTINGS_PLAY_MEDIA_DURING_VR, value);
        }

        @Keep
        public boolean hideClock() {
            return get(CAR_SETTINGS_HIDE_CLOCK, CAR_SETTINGS_HIDE_CLOCK_DEFAULT_VALUE);
        }

        public void hideClock(boolean value){
            set(CAR_SETTINGS_HIDE_CLOCK, value);
        }

        @Keep
        public boolean leftHandDrive() {
            return get(CAR_SETTINGS_LEFT_HAND_DRIVE, CAR_SETTINGS_LEFT_HAND_DRIVE_DEFAULT_VALUE);
        }

        public void leftHandDrive(boolean value){
            set(CAR_SETTINGS_LEFT_HAND_DRIVE, value);
        }
    }

    public class Keymap extends Base {

        public final static String KEYMAP_BTN_LEFT = "keymap_btn_left";
        public final static String KEYMAP_BTN_RIGHT = "keymap_btn_right";
        public final static String KEYMAP_BTN_SOURCE = "keymap_btn_source";
        public final static String KEYMAP_BTN_PLUS = "keymap_btn_plus";
        public final static String KEYMAP_BTN_MINUS = "keymap_btn_minus";

        public final static int KEYMAP_BTN_LEFT_DEFAULT_VALUE = 88; // PREV
        public final static int KEYMAP_BTN_RIGHT_DEFAULT_VALUE = 87; // NEXT
        public final static int KEYMAP_BTN_SOURCE_DEFAULT_VALUE = 85; // TOGGLE_PLAY
        public final static int KEYMAP_BTN_PLUS_DEFAULT_VALUE = 19; // UP
        public final static int KEYMAP_BTN_MINUS_DEFAULT_VALUE = 20; // DOWN

        private Keymap(){}

//        public void left(int code){
//            set(KEYMAP_BTN_LEFT, code);
//        }
//
//        public int left(){
//            return get(KEYMAP_BTN_LEFT, KEYMAP_BTN_LEFT_DEFAULT_VALUE);
//        }
//        public void right(int code){
//            set(KEYMAP_BTN_RIGHT, code);
//        }
//
//        public int right(String key){
//            return get(KEYMAP_BTN_RIGHT, KEYMAP_BTN_RIGHT_DEFAULT_VALUE);
//        }
//
//        public void source(int code){
//            set(KEYMAP_BTN_SOURCE, code);
//        }
//
//        public int source(String key){
//            return get(KEYMAP_BTN_SOURCE, KEYMAP_BTN_SOURCE_DEFAULT_VALUE);
//        }

//        public void plus(int code){
//            set(KEYMAP_BTN_PLUS, code);
//        }
//
//        public int plus(){
//            return get(KEYMAP_BTN_PLUS, KEYMAP_BTN_PLUS_DEFAULT_VALUE);
//        }
//
//        public void minus(int code){
//            set(KEYMAP_BTN_MINUS, code);
//        }
//
//        public int minus(){
//            return get(KEYMAP_BTN_MINUS, KEYMAP_BTN_MINUS_DEFAULT_VALUE);
//        }

        public void key(String key, int code){
            set(key, code);
        }

        public int key(String key){
            return get(key, 0);
        }

    }

    public class Video extends Base {
        public final static String VIDEO_RESOLUTION = "video_resolution";
        public final static String VIDEO_FPS = "video_fps";
        public final static String VIDEO_DPI = "video_dpi";
        public final static String VIDEO_MARGIN = "video_margin";
        public final static String VIDEO_NIGHT_MODE = "video_night_mode";
        public final static String VIDEO_SHOW_MEDIA_NOTIFICATION = "video_show_media_notification";
        public final static String VIDEO_SHOW_NAVIGATION_NOTIFICATION = "video_show_navigation_notification";
        public final static String VIDEO_SHOW_APP_BADGE = "video_show_app_badge";

        public final static int VIDEO_RESOLUTION_DEFAULT_VALUE = 1; // 480p
        public final static int VIDEO_FPS_DEFAULT_VALUE = 1; // 30 fps
        public final static int VIDEO_DPI_DEFAULT_VALUE = 140;
        public final static String VIDEO_MARGIN_DEFAULT_VALUE = "0,0";
        public final static String VIDEO_NIGHT_MODE_DEFAULT_VALUE = ""; //IDayNightSensor.TIME_GPS;
        public final static boolean VIDEO_SHOW_MEDIA_NOTIFICATION_DEFAULT_VALUE = true;
        public final static boolean VIDEO_SHOW_NAVIGATION_NOTIFICATION_DEFAULT_VALUE = true;
        public final static boolean VIDEO_SHOW_APP_BADGE_DEFAULT_VALUE = true;

        public int resolution(){
            return SP.getInt(VIDEO_RESOLUTION,VIDEO_RESOLUTION_DEFAULT_VALUE);
        }
        public void resolution(int res){
            SP.edit().putInt(VIDEO_RESOLUTION, res).apply();
        }

        public int fps(){
            return SP.getInt(VIDEO_FPS,VIDEO_FPS_DEFAULT_VALUE);
        }
        public void fps(int fps){
            SP.edit().putInt(VIDEO_FPS, fps).apply();
        }

        public int dpi() {
            return get(VIDEO_DPI, VIDEO_DPI_DEFAULT_VALUE);
        }
        public void dpi(int value){
            set(VIDEO_DPI, value);
        }

        public String margin(){
            return get(VIDEO_MARGIN,VIDEO_MARGIN_DEFAULT_VALUE);
        }
        public void margin(String value){
            set(VIDEO_MARGIN, value);
        }

        public String nightMode(){
            return get(VIDEO_NIGHT_MODE,VIDEO_NIGHT_MODE_DEFAULT_VALUE);
        }
        public void nightMode(String value){
            set(VIDEO_NIGHT_MODE, value);
        }

        public boolean showMediaNotification() {
            return get(VIDEO_SHOW_MEDIA_NOTIFICATION, VIDEO_SHOW_MEDIA_NOTIFICATION_DEFAULT_VALUE);
        }
        public void showMediaNotification(boolean value){
            set(VIDEO_SHOW_MEDIA_NOTIFICATION, value);
        }

        public boolean showNavigationNotification() {
            return get(VIDEO_SHOW_NAVIGATION_NOTIFICATION, VIDEO_SHOW_NAVIGATION_NOTIFICATION_DEFAULT_VALUE);
        }
        public void showNavigationNotification(boolean value){
            set(VIDEO_SHOW_NAVIGATION_NOTIFICATION, value);
        }

        public boolean showAppBadge() {
            return get(VIDEO_SHOW_APP_BADGE, VIDEO_SHOW_APP_BADGE_DEFAULT_VALUE);
        }
        public void showAppBadge(boolean value){
            set(VIDEO_SHOW_APP_BADGE, value);
        }
    }

    public class Connectivity extends Base {
        public final static String CONN_ENABLED_AD2P = "audio_enable_ad2p";
        public final static String CONN_ENABLED_HFP = "audio_enable_hfp";

        public final static boolean CONN_ENABLED_AD2P_DEFAULT_VALUE = false;
        public final static boolean CONN_ENABLED_HFP_DEFAULT_VALUE = true;


        public boolean enableA2dp(){
            return SP.getBoolean(CONN_ENABLED_AD2P, CONN_ENABLED_AD2P_DEFAULT_VALUE);
        }
        public void enableA2dp(boolean value){
            SP.edit().putBoolean(CONN_ENABLED_AD2P, value).apply();
        }

        public boolean enableHfp(){
            return SP.getBoolean(CONN_ENABLED_HFP, CONN_ENABLED_HFP_DEFAULT_VALUE);
        }
        public void enableHfp(boolean value){
            SP.edit().putBoolean(CONN_ENABLED_HFP, value).apply();
        }


    }
    public class Audio extends Base {
        public final static String MEDIA_ENABLE_CHANNEL = "media_enable_channel";
        public final static String MEDIA_SAMPLERATE = "media_samplerate";
        public final static String MEDIA_SAMPLESIZE = "media_samplesize";
        public final static String MEDIA_CHANNELCOUNT = "media_channelcount";

        public final static String SPEECH_ENABLE_CHANNEL = "speech_enable_channel";
        public final static String SPEECH_SAMPLERATE = "speech_samplerate";
        public final static String SPEECH_SAMPLESIZE = "speech_samplesize";
        public final static String SPEECH_CHANNELCOUNT = "speech_channelcount";

        public final static String SYSTEM_SAMPLERATE = "system_samplerate";
        public final static String SYSTEM_SAMPLESIZE = "system_samplesize";
        public final static String SYSTEM_CHANNELCOUNT = "system_channelcount";

        public final static String MIC_SAMPLERATE = "mic_samplerate";
        public final static String MIC_SAMPLESIZE = "mic_samplesize";
        public final static String MIC_CHANNELCOUNT = "mic_channelcount";
        public final static String MIC_SOURCE = "mic_source";

        public final static boolean MEDIA_ENABLE_CHANNEL_DEFAULT_VALUE = true;
        public final static int MEDIA_SAMPLERATE_DEFAULT_VALUE = 48000;
        public final static int MEDIA_SAMPLESIZE_DEFAULT_VALUE = 16; // 16bit
        public final static int MEDIA_CHANNELCOUNT_DEFAULT_VALUE = 2; //AudioFormat.CHANNEL_OUT_STEREO;

        public final static boolean SPEECH_ENABLE_CHANNEL_DEFAULT_VALUE = true;
        public final static int SPEECH_SAMPLERATE_DEFAULT_VALUE = 16000;
        public final static int SPEECH_SAMPLESIZE_DEFAULT_VALUE = 16; // 16bit
        public final static int SPEECH_CHANNELCOUNT_DEFAULT_VALUE = 1; //AudioFormat.CHANNEL_OUT_MONO;

        public final static int SYSTEM_SAMPLERATE_DEFAULT_VALUE = 16000;
        public final static int SYSTEM_SAMPLESIZE_DEFAULT_VALUE = 16; // 16bit
        public final static int SYSTEM_CHANNELCOUNT_DEFAULT_VALUE = 1; //AudioFormat.CHANNEL_OUT_MONO;

        public final static int MIC_SAMPLERATE_DEFAULT_VALUE = 16000;
        public final static int MIC_SAMPLESIZE_DEFAULT_VALUE = 16; // 16bit
        public final static int MIC_CHANNELCOUNT_DEFAULT_VALUE = 1; //AudioFormat.CHANNEL_OUT_MONO;
        public final static int MIC_SOURCE_DEFAULT_VALUE = 1; // 1 = MIC

        public boolean musicChannelEnabled(){
            return SP.getBoolean(MEDIA_ENABLE_CHANNEL, MEDIA_ENABLE_CHANNEL_DEFAULT_VALUE);
        }

        public void musicChannelEnabled(boolean value){
            SP.edit().putBoolean(MEDIA_ENABLE_CHANNEL, value).apply();
        }

        public int mediaSampleRate(){
            return SP.getInt(MEDIA_SAMPLERATE, MEDIA_SAMPLERATE_DEFAULT_VALUE);
        }

        public void mediaSampleRate(int value){
            SP.edit().putInt(MEDIA_SAMPLERATE, value).apply();
        }

        public int mediaSampleSize(){
            return SP.getInt(MEDIA_SAMPLESIZE, MEDIA_SAMPLESIZE_DEFAULT_VALUE);
        }

        public void mediaSampleSize(int value){
            SP.edit().putInt(MEDIA_SAMPLESIZE, value).apply();
        }

        public int mediaChannelCount(){
            return SP.getInt(MEDIA_CHANNELCOUNT, MEDIA_CHANNELCOUNT_DEFAULT_VALUE);
        }

        public void mediaChannelCount(int value){
            SP.edit().putInt(MEDIA_CHANNELCOUNT, value).apply();
        }

        public boolean speechAudioChannelEnabled(){
            return SP.getBoolean(SPEECH_ENABLE_CHANNEL, SPEECH_ENABLE_CHANNEL_DEFAULT_VALUE);
        }

        public void speechAudioChannelEnabled(boolean value){
            SP.edit().putBoolean(SPEECH_ENABLE_CHANNEL, value).apply();
        }

        public int speechSampleRate(){
            return SP.getInt(SPEECH_SAMPLERATE, SPEECH_SAMPLERATE_DEFAULT_VALUE);
        }

        public void speechSampleRate(int value){
            SP.edit().putInt(SPEECH_SAMPLERATE, value).apply();
        }

        public int speechSampleSize(){
            return SP.getInt(SPEECH_SAMPLESIZE, SPEECH_SAMPLESIZE_DEFAULT_VALUE);
        }

        public void speechSampleSize(int value){
            SP.edit().putInt(SPEECH_SAMPLESIZE, value).apply();
        }

        public int speechChannelCount(){
            return SP.getInt(SPEECH_CHANNELCOUNT, SPEECH_CHANNELCOUNT_DEFAULT_VALUE);
        }

        public void speechChannelCount(int value){
            SP.edit().putInt(SPEECH_CHANNELCOUNT, value).apply();
        }

        public int systemSampleRate(){
            return SP.getInt(SYSTEM_SAMPLERATE, SYSTEM_SAMPLERATE_DEFAULT_VALUE);
        }

        public void systemSampleRate(int value){
            SP.edit().putInt(SYSTEM_SAMPLERATE, value).apply();
        }

        public int systemSampleSize(){
            return SP.getInt(SYSTEM_SAMPLESIZE, SYSTEM_SAMPLESIZE_DEFAULT_VALUE);
        }

        public void systemSampleSize(int value){
            SP.edit().putInt(SYSTEM_SAMPLESIZE, value).apply();
        }

        public int systemChannelCount(){
            return SP.getInt(SYSTEM_CHANNELCOUNT, SYSTEM_CHANNELCOUNT_DEFAULT_VALUE);
        }

        public void systemChannelCount(int value){
            SP.edit().putInt(SYSTEM_CHANNELCOUNT, value).apply();
        }

        public int micSampleRate(){
            return SP.getInt(MIC_SAMPLERATE, MIC_SAMPLERATE_DEFAULT_VALUE);
        }

        public void micSampleRate(int value){
            SP.edit().putInt(MIC_SAMPLERATE, value).apply();
        }

        public int micSampleSize(){
            return SP.getInt(MIC_SAMPLESIZE, MIC_SAMPLESIZE_DEFAULT_VALUE);
        }

        public void micSampleSize(int value){
            SP.edit().putInt(MIC_SAMPLESIZE, value).apply();
        }

        public int micChannelCount(){
            return SP.getInt(MIC_CHANNELCOUNT, MIC_CHANNELCOUNT_DEFAULT_VALUE);
        }

        public void micChannelCount(int value){
            SP.edit().putInt(MIC_CHANNELCOUNT, value).apply();
        }

        public int micSource(){
            return SP.getInt(MIC_SOURCE, MIC_SOURCE_DEFAULT_VALUE);
        }

        public void micSource(int value){
            SP.edit().putInt(MIC_SOURCE, value).apply();
        }
    }

    public class Advanced extends Base{

        public final static String ADVANCED_ENABLED_DEBUG = "enabledebug";
        public final static String ADVANCED_LOG_DIR = "logDir";
        public final static String ADVANCED_ENABLE_WIFI = "enableWiFi";
        public final static String ADVANCED_LOG_LEVEL = "logLevel";
        public final static String ADVANCED_LOG_PROTOCOL = "logProtocol";
        public final static String ADVANCED_THREADS_NUM = "threads";
        public final static String ADVANCED_ENABLE_HONDA_INTEGRATION = "enablehondaintegration";
        public final static String ADVANCED_SW_MODE = "swmode";
        public final static String ADVANCED_MODEMGRAUDIO_IDX = "modemgridx";
        public final static String ADVANCED_SW_IDX = "steeringwheelidx";

        public final static boolean ADVANCED_ENABLED_DEBUG_DEFAULT_VALUE = false;
        public final static String ADVANCED_LOG_DIR_DEFAULT_VALUE = "/mnt/sdcard/usbdrive2/ODA/";
        public final static boolean ADVANCED_ENABLE_WIFI_DEFAULT_VALUE = false;
        public final static int ADVANCED_LOG_LEVEL_DEFAULT_VALUE = ILog.DEFAULT_LOG_LEVEL;
        public final static boolean ADVANCED_LOG_PROTOCOL_DEFAULT_VALUE = false;
        public final static int ADVANCED_THREADS_NUM_DEFAULT_VALUE = 4;
        public final static boolean ADVANCED_ENABLE_HONDA_INTEGRATION_DEFAULT_VALUE = true;
        public final static String ADVANCED_SW_MODE_DEFAULT_VALUE = "SW SERVICE";
        public final static int ADVANCED_MODEMGRAUDIO_IDX_DEFAULT_VALUE = 213; // 223 appmode, 220 appmodepic, 213 3rd party, 214 3rd party pic, 197 bt, 92 cam, 98 cd, 196 dab, 216 hdmi, 201 ipod, 222 mirrolink, 221 mirrorlink pic, 255 off, 87 phone, 198 tel rcv, 199 usb audio, 133 voicetag, 136 siri, 96 radio
        public final static int ADVANCED_SW_IDX_DEFAULT_VALUE = 260; // 258 source, 263 source header, 259 disp, 260 menu, 265 launcher, 266 imid

        public void logDir(String logDir){
            SP.edit().putString(ADVANCED_LOG_DIR, logDir).apply();
        }

        public String logDir(){
            return SP.getString(ADVANCED_LOG_DIR, ADVANCED_LOG_DIR_DEFAULT_VALUE);
        }

        public void enabledDebug(boolean enabled){
            SP.edit().putBoolean(ADVANCED_ENABLED_DEBUG, enabled).apply();
        }

        public boolean enabledDebug(){
            return SP.getBoolean(ADVANCED_ENABLED_DEBUG, ADVANCED_ENABLED_DEBUG_DEFAULT_VALUE);
        }

        public void enableWiFi(boolean enabled){
            SP.edit().putBoolean(ADVANCED_ENABLE_WIFI, enabled).apply();
        }

        public boolean enableWiFi(){
            return SP.getBoolean(ADVANCED_ENABLE_WIFI, ADVANCED_ENABLE_WIFI_DEFAULT_VALUE);
        }

        public int logLevel(){
            return SP.getInt(ADVANCED_LOG_LEVEL, ADVANCED_LOG_LEVEL_DEFAULT_VALUE);
        }

        public void logLevel(int logLevel){
            SP.edit().putInt(ADVANCED_LOG_LEVEL, logLevel).apply();
        }

        public boolean logProtocol(){
            return SP.getBoolean(ADVANCED_LOG_PROTOCOL, ADVANCED_LOG_PROTOCOL_DEFAULT_VALUE);
        }

        public void logProtocol(boolean logProtocol){
            SP.edit().putBoolean(ADVANCED_LOG_PROTOCOL, logProtocol).apply();
        }

        public int threads(){
            return SP.getInt(ADVANCED_THREADS_NUM, ADVANCED_THREADS_NUM_DEFAULT_VALUE);
        }

        public void threads(int threads){
            SP.edit().putInt(ADVANCED_THREADS_NUM, threads).apply();
        }

        public boolean hondaIntegrationEnabled(){
            return SP.getBoolean(ADVANCED_ENABLE_HONDA_INTEGRATION, ADVANCED_ENABLE_HONDA_INTEGRATION_DEFAULT_VALUE);
        }

        public void hondaIntegrationEnabled(boolean integration){
            SP.edit().putBoolean(ADVANCED_ENABLE_HONDA_INTEGRATION, integration).apply();
        }

        public String swMode(){
            return SP.getString(ADVANCED_SW_MODE, ADVANCED_SW_MODE_DEFAULT_VALUE);
        }

        public void swMode(String mode){
            SP.edit().putString(ADVANCED_SW_MODE, mode).apply();
        }

        public int modeMgrAudioIdx(){
            return SP.getInt(ADVANCED_MODEMGRAUDIO_IDX, ADVANCED_MODEMGRAUDIO_IDX_DEFAULT_VALUE);
        }

        public void modeMgrAudioIdx(int idx){
            SP.edit().putInt(ADVANCED_MODEMGRAUDIO_IDX, idx).apply();
        }

        public int steeringWheelIdx(){
            return SP.getInt(ADVANCED_SW_IDX, ADVANCED_SW_IDX_DEFAULT_VALUE);
        }

        public void steeringWheelIdx(int idx){
            SP.edit().putInt(ADVANCED_SW_IDX, idx).apply();
        }
    }

    public class Base {
        private Base(){}

        public String get(String key, String defaultValue){
            return SP.getString(key, defaultValue);
        }

        public void set(String key, String value){
            SP.edit().putString(key, value).apply();
        }

        public boolean get(String key, boolean defaultValue) {
            return SP.getBoolean(key, defaultValue);
        }

        public void set(String key, boolean value) {
            SP.edit().putBoolean(key, value).apply();
        }

        public int get(String key, int defaultValue){
            return SP.getInt(key, defaultValue);
        }

        public void set(String key, int value){
            SP.edit().putInt(key, value).apply();
        }
    }
}
