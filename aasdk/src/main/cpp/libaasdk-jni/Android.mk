LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := aasdk-jni
AASDK_SRC_FILES := channel/av/AudioServiceChannel.cpp \
                   channel/av/AVInputServiceChannel.cpp \
                   channel/av/MediaAudioServiceChannel.cpp \
                   channel/av/MediaStatusServiceChannel.cpp \
                   channel/av/SpeechAudioServiceChannel.cpp \
                   channel/av/SystemAudioServiceChannel.cpp \
                   channel/av/VideoServiceChannel.cpp \
                   channel/bluetooth/BluetoothServiceChannel.cpp \
                   channel/control/ControlServiceChannel.cpp \
                   channel/input/InputServiceChannel.cpp \
                   channel/navigation/NavigationStatusServiceChannel.cpp \
                   channel/sensor/SensorServiceChannel.cpp \
                   channel/ServiceChannel.cpp \
                   common/Data.cpp \
                   configuration/Configuration.cpp \
                   error/Error.cpp \
                   io/IOContextWrapper.cpp \
                   messenger/ChannelId.cpp \
                   messenger/ChannelReceiveMessageQueue.cpp \
                   messenger/ChannelReceivePromiseQueue.cpp \
                   messenger/Cryptor.cpp \
                   messenger/EncryptionType.cpp \
                   messenger/FrameHeader.cpp \
                   messenger/FrameSize.cpp \
                   messenger/FrameSizeType.cpp \
                   messenger/FrameType.cpp \
                   messenger/Message.cpp \
                   messenger/MessageId.cpp \
                   messenger/MessageInStream.cpp \
                   messenger/MessageOutStream.cpp \
                   messenger/MessageType.cpp \
                   messenger/Messenger.cpp \
                   messenger/Timestamp.cpp \
                   service/AndroidAutoEntity.cpp \
                   service/AudioInputService.cpp \
                   service/AudioService.cpp \
                   service/BluetoothService.cpp \
                   service/InputService.cpp \
                   service/MediaAudioService.cpp \
                   service/MediaStatusService.cpp \
                   service/NavigationStatusService.cpp \
                   service/Pinger.cpp \
                   service/SensorService.cpp \
                   service/SpeechAudioService.cpp \
                   service/SystemAudioService.cpp \
                   service/VideoService.cpp \
                   tcp/TCPEndpoint.cpp \
                   tcp/TCPWrapper.cpp \
                   transport/DataSink.cpp \
                   transport/SSLWrapper.cpp \
                   transport/TCPTransport.cpp \
                   transport/Transport.cpp \
                   transport/USBTransport.cpp \
                   usb/AOAPDevice.cpp \
                   usb/LibUsb.cpp \
                   usb/LibUsbDevice.cpp \
                   usb/LibUsbEndpoint.cpp \
                   jni/it_smg_libs_aasdk_messenger_Cryptor.cpp \
                   jni/it_smg_libs_aasdk_messenger_Messenger.cpp \
                   jni/it_smg_libs_aasdk_projection_AudioInput.cpp \
                   jni/it_smg_libs_aasdk_projection_AudioOutput.cpp \
                   jni/it_smg_libs_aasdk_projection_BluetoothDevice.cpp \
                   jni/it_smg_libs_aasdk_projection_InputDevice.cpp \
                   jni/it_smg_libs_aasdk_projection_MediaStatusEvent.cpp \
                   jni/it_smg_libs_aasdk_projection_NavigationStatusEvent.cpp \
                   jni/it_smg_libs_aasdk_projection_VideoOutput.cpp \
                   jni/it_smg_libs_aasdk_Runtime.cpp \
                   jni/it_smg_libs_aasdk_service_AndroidAutoEntity.cpp \
                   jni/it_smg_libs_aasdk_service_AudioInputService.cpp \
                   jni/it_smg_libs_aasdk_service_BluetoothService.cpp \
                   jni/it_smg_libs_aasdk_service_InputService.cpp \
                   jni/it_smg_libs_aasdk_service_MediaAudioService.cpp \
                   jni/it_smg_libs_aasdk_service_MediaStatusService.cpp \
                   jni/it_smg_libs_aasdk_service_NavigationStatusService.cpp \
                   jni/it_smg_libs_aasdk_service_Pinger.cpp \
                   jni/it_smg_libs_aasdk_service_SensorService.cpp \
                   jni/it_smg_libs_aasdk_service_SpeechAudioService.cpp \
                   jni/it_smg_libs_aasdk_service_SystemAudioService.cpp \
                   jni/it_smg_libs_aasdk_service_VideoService.cpp \
                   jni/it_smg_libs_aasdk_tcp_TCPEndpoint.cpp \
                   jni/it_smg_libs_aasdk_transport_TCPTransport.cpp \
                   jni/it_smg_libs_aasdk_transport_Transport.cpp \
                   jni/it_smg_libs_aasdk_transport_USBTransport.cpp \
                   jni/it_smg_libs_aasdk_usb_AOAPDevice.cpp \
                   jni/it_smg_libs_aasdk_usb_LibUsb.cpp \
                   jni/it_smg_libs_aasdk_usb_LibUsbDevice.cpp

PROTO_SRC_FILES := proto/TouchLocationData.pb.cc \
                   proto/GearEnum.pb.cc \
                   proto/ChannelOpenResponseMessage.pb.cc \
                   proto/AbsoluteInputEventData.pb.cc \
                   proto/AVChannelSetupResponseMessage.pb.cc \
                   proto/AVChannelStopIndicationMessage.pb.cc \
                   proto/AudioConfigData.pb.cc \
                   proto/AVInputChannelData.pb.cc \
                   proto/AudioFocusStateEnum.pb.cc \
                   proto/AbsoluteInputEventsData.pb.cc \
                   proto/VideoFocusRequestMessage.pb.cc \
                   proto/ShutdownResponseMessage.pb.cc \
                   proto/ButtonEventsData.pb.cc \
                   proto/DrivingStatusData.pb.cc \
                   proto/AudioFocusTypeEnum.pb.cc \
                   proto/ButtonEventData.pb.cc \
                   proto/VideoResolutionEnum.pb.cc \
                   proto/VideoFocusReasonEnum.pb.cc \
                   proto/AudioFocusRequestMessage.pb.cc \
                   proto/NavigationTurnTypeEnum.pb.cc \
                   proto/RelativeInputEventData.pb.cc \
                   proto/ShutdownReasonEnum.pb.cc \
                   proto/ManeuverTypeEnum.pb.cc \
                   proto/MediaInfoChannelMessageIdsEnum.pb.cc \
                   proto/VersionResponseStatusEnum.pb.cc \
                   proto/OdometerData.pb.cc \
                   proto/BluetoothPairingStatusEnum.pb.cc \
                   proto/RPMData.pb.cc \
                   proto/BluetoothPairingRequestMessage.pb.cc \
                   proto/ChannelOpenRequestMessage.pb.cc \
                   proto/DoorData.pb.cc \
                   proto/ShutdownRequestMessage.pb.cc \
                   proto/ServiceDiscoveryRequestMessage.pb.cc \
                   proto/AVChannelStartIndicationMessage.pb.cc \
                   proto/MediaInfoChannelMetadataData.pb.cc \
                   proto/FuelLevelData.pb.cc \
                   proto/NavigationChannelData.pb.cc \
                   proto/SteeringWheelData.pb.cc \
                   proto/InputChannelMessageIdsEnum.pb.cc \
                   proto/ChannelDescriptorData.pb.cc \
                   proto/AVInputOpenRequestMessage.pb.cc \
                   proto/SensorChannelData.pb.cc \
                   proto/NavigationTurnEventMessage.pb.cc \
                   proto/AVMediaAckIndicationMessage.pb.cc \
                   proto/PassengerData.pb.cc \
                   proto/SensorData.pb.cc \
                   proto/AudioFocusResponseMessage.pb.cc \
                   proto/DistanceUnitEnum.pb.cc \
                   proto/AVChannelSetupStatusEnum.pb.cc \
                   proto/TouchEventData.pb.cc \
                   proto/InputChannelData.pb.cc \
                   proto/AuthCompleteIndicationMessage.pb.cc \
                   proto/ButtonCodeEnum.pb.cc \
                   proto/SensorStartRequestMessage.pb.cc \
                   proto/SensorTypeEnum.pb.cc \
                   proto/GyroData.pb.cc \
                   proto/MediaInfoChannelPlaybackData.pb.cc \
                   proto/NavigationStatusMessage.pb.cc \
                   proto/VideoFPSEnum.pb.cc \
                   proto/GearData.pb.cc \
                   proto/VideoFocusModeEnum.pb.cc \
                   proto/ServiceDiscoveryResponseMessage.pb.cc \
                   proto/AVChannelMessageIdsEnum.pb.cc \
                   proto/AVStreamTypeEnum.pb.cc \
                   proto/EnvironmentData.pb.cc \
                   proto/BluetoothPairingResponseMessage.pb.cc \
                   proto/DrivingStatusEnum.pb.cc \
                   proto/PingRequestMessage.pb.cc \
                   proto/NavigationDistanceEventMessage.pb.cc \
                   proto/NavigationImageOptionsData.pb.cc \
                   proto/AVChannelSetupRequestMessage.pb.cc \
                   proto/IndicatorStatusEnum.pb.cc \
                   proto/VendorExtensionChannelData.pb.cc \
                   proto/NavigationFocusResponseMessage.pb.cc \
                   proto/SensorStartResponseMessage.pb.cc \
                   proto/StatusEnum.pb.cc \
                   proto/InputEventIndicationMessage.pb.cc \
                   proto/GPSLocationData.pb.cc \
                   proto/BluetoothChannelData.pb.cc \
                   proto/PingResponseMessage.pb.cc \
                   proto/NavigationChannelMessageIdsEnum.pb.cc \
                   proto/AccelData.pb.cc \
                   proto/LightData.pb.cc \
                   proto/BluetoothChannelMessageIdsEnum.pb.cc \
                   proto/CompassData.pb.cc \
                   proto/DiagnosticsData.pb.cc \
                   proto/SpeedData.pb.cc \
                   proto/ManeuverDirectionEnum.pb.cc \
                   proto/ParkingBrakeData.pb.cc \
                   proto/NavigationFocusRequestMessage.pb.cc \
                   proto/BindingRequestMessage.pb.cc \
                   proto/RelativeInputEventsData.pb.cc \
                   proto/VideoFocusIndicationMessage.pb.cc \
                   proto/BindingResponseMessage.pb.cc \
                   proto/SensorChannelMessageIdsEnum.pb.cc \
                   proto/SensorEventIndicationMessage.pb.cc \
                   proto/AVInputOpenResponseMessage.pb.cc \
                   proto/NightModeData.pb.cc \
                   proto/TouchConfigData.pb.cc \
                   proto/AudioTypeEnum.pb.cc \
                   proto/ControlMessageIdsEnum.pb.cc \
                   proto/MediaInfoChannelData.pb.cc \
                   proto/WifiChannelData.pb.cc \
                   proto/HVACData.pb.cc \
                   proto/TouchActionEnum.pb.cc \
                   proto/VideoConfigData.pb.cc \
                   proto/VoiceSessionRequestMessage.pb.cc \
                   proto/BluetoothPairingMethodEnum.pb.cc \
                   proto/AVChannelData.pb.cc \
                   proto/HeadlightStatusEnum.pb.cc

LOCAL_SRC_FILES := $(AASDK_SRC_FILES) \
                   $(PROTO_SRC_FILES)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_STATIC_LIBRARIES := libusb-1.0 libssl libcrypto libboost_system libboost_thread protobuf
LOCAL_SHARED_LIBRARIES := common

LOCAL_LDLIBS += -lz -llog -landroid
LOCAL_LDFLAGS += -rdynamic

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CFLAGS := -funwind-tables -Wl,--no-merge-exidx-entries -fno-omit-frame-pointer
LOCAL_ARM_NEON := true

include $(BUILD_SHARED_LIBRARY)

# If you don't need your project to build with NDKs older than r21, you can omit
# this block.
ifneq ($(call ndk-major-at-least,21),true)
$(call import-add-path,$(NDK_GRADLE_INJECTED_IMPORT_PATH))
endif

$(call import-module, prefab/common)
