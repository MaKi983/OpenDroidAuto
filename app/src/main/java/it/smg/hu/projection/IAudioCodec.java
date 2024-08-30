package it.smg.hu.projection;

import java.nio.ByteBuffer;

public interface IAudioCodec {
    void write(ByteBuffer buffer, long timestamp);
    void start();
    void stop();
    void delete();
}
