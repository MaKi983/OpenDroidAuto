package it.smg.hu.service;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Read-only diagnostics endpoint for debugging phone-call and crash behaviour
 * on the head unit without adb.
 *
 * It runs in its own process (":debug") so it survives ODA crashes, and it does
 * NOT execute arbitrary input: a connecting client may only pick one of a fixed
 * set of predefined, read-only diagnostic actions by name. There is no shell.
 *
 * Protocol (one request per connection):
 *   line 1: access token
 *   line 2: action name (one of the keys in ACTIONS)
 * The output of the predefined command for that action is streamed back.
 */
public class DebugShellService extends Service {

    private static final String TAG = "ODA/DebugDiag";
    public static final int PORT = 5577;
    private static final String TOKEN = "qbavpmwxligf42sn3ek1";

    /** Fixed, read-only diagnostic actions. No caller input reaches a shell. */
    private static final Map<String, String[]> ACTIONS = new LinkedHashMap<>();
    static {
        // live log, drained and returned (client controls duration by disconnecting)
        ACTIONS.put("logcat",        new String[]{"logcat", "-v", "threadtime"});
        // snapshot of the log buffer, then exit
        ACTIONS.put("logcat-dump",   new String[]{"logcat", "-d", "-v", "threadtime"});
        ACTIONS.put("logcat-clear",  new String[]{"logcat", "-c"});
        // audio routing / focus state
        ACTIONS.put("audio",         new String[]{"dumpsys", "audio"});
        // telephony / call state
        ACTIONS.put("telephony",     new String[]{"dumpsys", "telephony.registry"});
        // bluetooth (HFP/A2DP) state
        ACTIONS.put("bluetooth",     new String[]{"dumpsys", "bluetooth_manager"});
        // media sessions
        ACTIONS.put("media",         new String[]{"dumpsys", "media_session"});
        // running processes (is ODA alive? did the native process die?)
        ACTIONS.put("processes",     new String[]{"ps"});
        // list available action names
        ACTIONS.put("help",          null);
    }

    private ServerSocket serverSocket_;
    private Thread acceptThread_;

    @Override
    public void onCreate() {
        super.onCreate();
        acceptThread_ = new Thread(this::acceptLoop, "DebugDiag-accept");
        acceptThread_.start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        try {
            if (serverSocket_ != null) serverSocket_.close();
        } catch (IOException ignored) {}
        super.onDestroy();
    }

    private void acceptLoop() {
        try {
            serverSocket_ = new ServerSocket(PORT);
            Log.i(TAG, "listening on port " + PORT);
            while (!serverSocket_.isClosed()) {
                final Socket client = serverSocket_.accept();
                new Thread(() -> handle(client), "DebugDiag-client").start();
            }
        } catch (IOException e) {
            Log.e(TAG, "server error", e);
        }
    }

    private void handle(Socket client) {
        Process process = null;
        try {
            client.setSoTimeout(10000);
            BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream(), "UTF-8"));
            OutputStream out = client.getOutputStream();

            String token = in.readLine();
            if (!TOKEN.equals(token)) {
                out.write("ERR token\n".getBytes("UTF-8"));
                return;
            }

            String action = in.readLine();
            if (action != null) action = action.trim();

            if (action == null || "help".equals(action) || !ACTIONS.containsKey(action)) {
                StringBuilder sb = new StringBuilder("available actions:\n");
                for (String key : ACTIONS.keySet()) sb.append("  ").append(key).append('\n');
                out.write(sb.toString().getBytes("UTF-8"));
                return;
            }

            String[] cmd = ACTIONS.get(action);
            client.setSoTimeout(0);
            Log.i(TAG, "diag: " + action);

            ProcessBuilder pb = new ProcessBuilder(cmd);
            pb.redirectErrorStream(true);
            process = pb.start();

            InputStream pin = process.getInputStream();
            byte[] buf = new byte[8192];
            int n;
            while ((n = pin.read(buf)) != -1) {
                out.write(buf, 0, n);
                out.flush();
            }
        } catch (IOException e) {
            Log.w(TAG, "client error: " + e.getMessage());
        } finally {
            if (process != null) process.destroy();
            try {
                client.close();
            } catch (IOException ignored) {}
        }
    }
}
