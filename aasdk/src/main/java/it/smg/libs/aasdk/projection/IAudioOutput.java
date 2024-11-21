package it.smg.libs.aasdk.projection;

import java.nio.ByteBuffer;

public interface IAudioOutput {
    boolean open();
    void write(long timestamp, ByteBuffer buffer);
//    void write(long timestamp, byte[] buffer);
    void start();
    void stop();
    void suspend();
    int getSampleSize();
    int getChannelCount();
    int getSampleRate();
    void delete();
}
