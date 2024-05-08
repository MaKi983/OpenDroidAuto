package it.smg.libs.aasdk.projection;

public interface IBluetoothDevice {
    void stop();
    boolean isPaired(String address);
    boolean pair(String address);
    String getLocalAddress();
    boolean isAvailable();
    void delete();
}
