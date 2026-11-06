package it.smg.hu.manager;

/** Pure connection policy so manual start and exit behavior can be tested without Android hardware. */
public final class ConnectionPolicy {
    private ConnectionState state = ConnectionState.IDLE;
    private String mode;

    public ConnectionState state() {
        return state;
    }

    public String mode() {
        return mode;
    }

    public boolean isManualStartAllowed() {
        // A start request during permission, AOAP negotiation or native
        // teardown creates an overlapping session.  Only a prepared transport
        // (IDLE) or a deliberately stopped, still-prepared transport (EXITED)
        // may launch PlayerActivity.
        return state == ConnectionState.IDLE || state == ConnectionState.EXITED;
    }

    public void transportAvailable(String connectionMode) {
        mode = connectionMode;
        state = ConnectionState.IDLE;
    }

    public void permissionPending() {
        state = ConnectionState.PERMISSION_PENDING;
    }

    public void switchingToAoap() {
        state = ConnectionState.AOAP_SWITCHING;
    }

    public void connecting(String connectionMode) {
        if (connectionMode != null) {
            mode = connectionMode;
        }
        state = ConnectionState.CONNECTING;
    }

    public void active() {
        state = ConnectionState.ACTIVE;
    }

    public void disconnecting() {
        state = ConnectionState.DISCONNECTING;
    }

    public void userExited() {
        state = ConnectionState.EXITED;
    }

    public void detached() {
        mode = null;
        state = ConnectionState.IDLE;
    }

    public void failed() {
        state = ConnectionState.ERROR;
    }
}
