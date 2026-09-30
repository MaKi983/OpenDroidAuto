package it.smg.hu.manager;

import android.content.Context;
import android.content.Intent;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

/** Coordinates USB and Wi-Fi session state for the activities and foreground service. */
public final class ConnectionManager {
    public static final String ACTION_STATE_CHANGED = "it.smg.hu.CONNECTION_STATE_CHANGED";
    public static final String EXTRA_STATE = "state";
    public static final String EXTRA_MODE = "mode";
    public static final String EXTRA_MESSAGE = "message";

    private static ConnectionManager instance_;

    private final ConnectionPolicy policy_ = new ConnectionPolicy();
    private final LocalBroadcastManager broadcastManager_;
    private String lastMessage_;

    private ConnectionManager(Context context) {
        broadcastManager_ = LocalBroadcastManager.getInstance(context.getApplicationContext());
    }

    public static void init(Context context) {
        if (instance_ == null) {
            instance_ = new ConnectionManager(context);
        }
    }

    public static ConnectionManager instance() {
        return instance_;
    }

    public synchronized ConnectionState state() {
        return policy_.state();
    }

    public synchronized String mode() {
        return policy_.mode();
    }

    public synchronized boolean isManualStartAllowed() {
        return policy_.isManualStartAllowed();
    }

    public synchronized String lastMessage() {
        return lastMessage_;
    }


    public synchronized void transportAvailable(String mode, String message) {
        policy_.transportAvailable(mode);
        publish(message);
    }

    public synchronized void permissionPending(String message) {
        policy_.permissionPending();
        publish(message);
    }

    public synchronized void switchingToAoap(String message) {
        policy_.switchingToAoap();
        publish(message);
    }

    public synchronized void connecting(String mode, String message) {
        policy_.connecting(mode);
        publish(message);
    }

    public synchronized void active(String message) {
        policy_.active();
        publish(message);
    }

    public synchronized void disconnecting(String message) {
        policy_.disconnecting();
        publish(message);
    }

    public synchronized void userExited(String message) {
        policy_.userExited();
        publish(message);
    }

    public synchronized void detached(String message) {
        policy_.detached();
        publish(message);
    }

    public synchronized void detached(String mode, String message) {
        if (mode != null && policy_.mode() != null && !mode.equals(policy_.mode())) {
            return;
        }
        detached(message);
    }

    public synchronized void failed(String message) {
        policy_.failed();
        publish(message);
    }

    private void publish(String message) {
        lastMessage_ = message;
        Intent intent = new Intent(ACTION_STATE_CHANGED);
        intent.putExtra(EXTRA_STATE, policy_.state().name());
        intent.putExtra(EXTRA_MODE, policy_.mode());
        intent.putExtra(EXTRA_MESSAGE, message);
        broadcastManager_.sendBroadcast(intent);
    }
}
