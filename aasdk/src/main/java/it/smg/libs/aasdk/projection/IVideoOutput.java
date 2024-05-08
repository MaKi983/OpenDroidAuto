package it.smg.libs.aasdk.projection;

import java.nio.ByteBuffer;

public interface IVideoOutput {
    boolean open();
    boolean init();
    void write(long timestamp, ByteBuffer buffer);
    void stop();
    int getVideoFPS();
    int getVideoResolution();
    int getScreenDPI();
    Rect getVideoMargins();
    void delete();

    class Rect {
        public final int width;
        public final int height;

        public Rect(int width, int height) {
            this.width = width;
            this.height = height;
        }
    }
}
