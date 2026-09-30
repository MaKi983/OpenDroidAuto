package it.smg.hu.manager;

public enum ConnectionState {
    IDLE,
    PERMISSION_PENDING,
    AOAP_SWITCHING,
    CONNECTING,
    DISCONNECTING,
    ACTIVE,
    EXITED,
    ERROR
}
