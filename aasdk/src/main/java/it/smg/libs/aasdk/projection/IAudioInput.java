package it.smg.libs.aasdk.projection;

import java.nio.ByteBuffer;

public interface IAudioInput {
    boolean open();
    boolean isActive();
    int read(ByteBuffer buffer);
    boolean start();
    void stop();
    int getSampleSize();
    int getChannelCount();
    int getSampleRate();
    int getBufferSize();
    void delete();
}