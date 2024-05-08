package it.smg.libs.aasdk.messenger;

public enum ChannelId {
    CONTROL(0),
    INPUT(1),
    SENSOR(2),
    VIDEO(3),
    MEDIA_AUDIO(4),
    SPEECH_AUDIO(5),
    SYSTEM_AUDIO(6),
    AV_INPUT(7),
    BLUETOOTH(8),
    NAVIGATION(9),
    MEDIA_STATUS(10),
    NONE(255);

    ChannelId(int value){
        value_ = value;
    }

    private int value_;

    public int value(){
        return value_;
    }

    public byte asByte() {
        return (byte) value_;
    }

    public static ChannelId from(byte data){
        switch (new Byte(data).intValue()){
            case 0:
                return CONTROL;
            case 1:
                return INPUT;
            case 2:
                return SENSOR;
            case 3:
                return VIDEO;
            case 4:
                return MEDIA_AUDIO;
            case 5:
                return SPEECH_AUDIO;
            case 6:
                return SYSTEM_AUDIO;
            case 7:
                return AV_INPUT;
            case 8:
                return BLUETOOTH;
            case 9:
                return NAVIGATION;
            case 10:
                return MEDIA_STATUS;
            default:
                return NONE;
        }
    }

    public static String channelIdToString(ChannelId channelId){
        switch(channelId)
        {
            case CONTROL:
                return "CONTROL";
            case INPUT:
                return "INPUT";
            case SENSOR:
                return "SENSOR";
            case VIDEO:
                return "VIDEO";
            case MEDIA_AUDIO:
                return "MEDIA_AUDIO";
            case SPEECH_AUDIO:
                return "SPEECH_AUDIO";
            case SYSTEM_AUDIO:
                return "SYSTEM_AUDIO";
            case AV_INPUT:
                return "AV_INPUT";
            case BLUETOOTH:
                return "BLUETOOTH";
            case NAVIGATION:
                return "NAVIGATION";
            case NONE:
                return "NONE";
            default:
                return "(null)";
        }
    }
}
