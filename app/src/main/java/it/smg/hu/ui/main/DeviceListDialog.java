package it.smg.hu.ui.main;

import android.app.AlertDialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import it.smg.hu.R;
import it.smg.hu.manager.USBManager;
import it.smg.libs.aasdk.usb.LibUsbDevice;
import it.smg.libs.common.Log;

public class DeviceListDialog extends AlertDialog {

    private static final String TAG = "DeviceListDialog";

    private USBManager usbManager_;

    protected DeviceListDialog(Context context) {
        super(context);

        setTitle("Select USB Device");

        LayoutInflater inflater = getLayoutInflater();
        View listView = inflater.inflate(R.layout.usb_devices_dialog, null);
        setView(listView);
        RadioGroup group = listView.findViewById(R.id.usb_devices_list);

        usbManager_ = USBManager.instance();

        int i = 0;
        for (LibUsbDevice d : usbManager_.connectedDevices()){
            RadioButton b = new RadioButton(getContext());
            b.setText(d.product);
            b.setId(i);
            LibUsbDevice currentAOAP = usbManager_.aoapDevice();
            if (currentAOAP != null && d.equals(currentAOAP)){
                b.setEnabled(false);
                b.setSelected(true);
            }
            b.setTag(d);
            b.setOnClickListener(v1 -> {
                LibUsbDevice dev = (LibUsbDevice) v1.getTag();
                if (Log.isDebug()) Log.d(TAG, "selected device: " + dev);
                if (currentAOAP != null && dev.equals(currentAOAP)) {
                    if (Log.isDebug()) Log.d(TAG, "the device selected is already the current device");
                } else {
                    usbManager_.checkDevice(dev.originalDevice());
                    dismiss();
                }
            });
            group.addView(b);
            i++;
        }
    }

}
