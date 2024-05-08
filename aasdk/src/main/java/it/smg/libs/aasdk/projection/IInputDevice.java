package it.smg.libs.aasdk.projection;

public interface IInputDevice {
    void start();
    void stop();
    int[] getSupportedButtonCodes();
    boolean hasTouchscreen();
    Rect getTouchscreenGeometry();
    void gainFocus();
    void releaseFocus();
    void delete();

    class Rect {
        public final int width;
        public final int height;

        public Rect(int width, int height) {
            this.width = width;
            this.height = height;
        }
    }

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
}
