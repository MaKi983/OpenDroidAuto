package it.smg.hu.manager;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.DhcpInfo;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import java.net.InetAddress;
import java.net.UnknownHostException;

import it.smg.libs.common.Log;

public class WIFIManager {

    private static final String TAG = "WiFiConnectionManager";

    public static final String DISCONNECT_WIFI = "it.smg.hu.DISCONNECT_WIFI";
    public static final String CONNECT_WIFI = "it.smg.hu.CONNECT_WIFI";
    public static final String EXTRA_SSID = "ssid";

    private static WIFIManager instance_;
    private LocalBroadcastManager localBroadcastManager_;

    private Context ctx_;
    private String ipAddress_;
    private String networkSSID_;

    private Thread serverThread;

    private WIFIManager(Context ctx){
        ctx_ = ctx;
        localBroadcastManager_ = LocalBroadcastManager.getInstance(ctx_);
    }

    public static void init(Context ctx){
        if (instance_ == null){
            instance_ = new WIFIManager(ctx);
        }
    }

    public static WIFIManager instance() {
        return instance_;
    }

    public String getIpAddress(){
        return ipAddress_;
    }

    private String networkSSID(){
        WifiManager wifi = (WifiManager) ctx_.getSystemService(Context.WIFI_SERVICE);
        return wifi.getConnectionInfo().getSSID();
    }

    private String ipAddress(){
        WifiManager wifi = (WifiManager) ctx_.getSystemService(Context.WIFI_SERVICE);
        DhcpInfo d = wifi.getDhcpInfo();

        byte[] addressBytes = {(byte) (0xff & d.gateway),
                (byte) (0xff & (d.gateway >> 8)),
                (byte) (0xff & (d.gateway >> 16)),
                (byte) (0xff & (d.gateway >> 24))};
        try {
            return InetAddress.getByAddress(addressBytes).getHostAddress();
        } catch (UnknownHostException e) {
            return null;
        }

    }

    private boolean isConnected(){
        ConnectivityManager connManager = (ConnectivityManager) ctx_.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo mWifi = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        return mWifi.isConnected();
    }

    public void checkNetwork(){
        synchronized (this) {

            boolean isConnected = isConnected();
            if (isConnected) {
                ipAddress_ = ipAddress();
                networkSSID_ = networkSSID();
                if (Log.isDebug()) Log.d(TAG, "Network with ip: " + ipAddress_);
                if (Log.isDebug()) Log.d(TAG, "Network with SSID: " + networkSSID_);

                Intent connectWifiIntent = new Intent(CONNECT_WIFI);
                connectWifiIntent.putExtra(EXTRA_SSID, networkSSID_);
                localBroadcastManager_.sendBroadcast(connectWifiIntent);

                return;
            }

            if (Log.isDebug()) Log.d(TAG, "isConnected: " + isConnected);

            if (!isConnected) {
                ipAddress_ = null;
                networkSSID_ = null;
            }

            Intent disconnectWifiIntent = new Intent(DISCONNECT_WIFI);
            localBroadcastManager_.sendBroadcast(disconnectWifiIntent);
        }
    }
}
