#pragma once

#include <JNIBase.h>
#include <service/BluetoothService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JBluetoothService : public JService, JNIBase {
public:
    typedef JBluetoothService* Pointer;

    JBluetoothService(JNIEnv *env, jobject jbluetoothservice, jobject jmessenger, jobject jbluetoothdevice);
    ~JBluetoothService();

    static jfieldID handleId;
    static JBluetoothService::Pointer getJBluetoothService(JNIEnv* env, jobject jbluetoothservice);
    IService::Pointer getService() override;

    void start();
    void stop();
private:
    BluetoothService::Pointer bluetoothService_;

};