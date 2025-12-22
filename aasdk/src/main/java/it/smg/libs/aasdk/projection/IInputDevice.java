package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

public interface IInputDevice {
    void start();
    void stop();
    int[] getSupportedButtonCodes();
    boolean hasTouchscreen();
    Rect getTouchscreenGeometry();
    void gainFocus();
    void releaseFocus();
    void delete();

    @Keep
    class Rect {
        public final int width;
        public final int height;

        public Rect(int width, int height) {
            this.width = width;
            this.height = height;
        }
    }

    @Keep
    class TouchAction {
        public final int x;
        public final int y;
        public final int pointerId;

        public TouchAction(int x, int y, int pointerId){
            this.x = x;
            this.y = y;
            this.pointerId = pointerId;
        }
    }

    @Keep
    enum KeyCode {
        NONE(0, "none"),
        MICROPHONE_2(1, "mic2"),
        MENU(2,   "menu"),
        HOME(3,  "home"),
        BACK(4,  "back"),
        PHONE(5,  "phone"),
        CALL_END(6,  "call_end"),
        UP(19, "up"),
        DOWN(20,"down"),
        LEFT(21,"left"),
        RIGHT(22, "right"),
        ENTER(23, "enter"),
        MICROPHONE_1(84, "mic1"),
        TOGGLE_PLAY(85, "toggle_play"),
        NEXT(87, "next"),
        PREV(88, "prev"),
        PLAY(126, "play"),
        PAUSE(127,  "pause"),
        SCROLL_WHEEL(65536, "scroll_wheel");

        private final int code_;
        private final String keyName_;

        KeyCode(int code, String name){
            code_ = code;
            keyName_ = name;
        }

        public int value(){
            return code_;
        }

        public String keyName(){
            return keyName_;
        }

    }
}
