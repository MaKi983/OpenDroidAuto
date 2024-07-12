LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

EXT_LIB := $(LOCAL_PATH)/../../../../../ext-libs
ANDROID_LIB := $(EXT_LIB)/$(TARGET_ARCH_ABI)/system/hrv

LOCAL_MODULE    := libomxvideocodec-jni
LOCAL_SRC_FILES := OMXVideoCodec.cpp \
                   OMXDecoder.cpp \
                   OMXSource.cpp \
                   it_smg_libs_videocodec_OMXVideoCodec.cpp


LOCAL_LDLIBS += -llog -landroid -l$(ANDROID_LIB)/libstagefright.so -l$(ANDROID_LIB)/libutils.so -l$(ANDROID_LIB)/libcutils.so

LOCAL_C_INCLUDES := $(EXT_LIB)/include/system/core \
                    $(EXT_LIB)/include/base \
                    $(EXT_LIB)/include/libhardware \
                    $(EXT_LIB)/include/base/media/stagefright/openmax

LOCAL_SHARED_LIBRARIES := common

LOCAL_CFLAGS += -DHAVE_PTHREADS -g -funwind-tables -Wl,--no-merge-exidx-entries -fno-omit-frame-pointer
LOCAL_LDFLAGS := -Wl,--allow-multiple-definition
LOCAL_CPPFLAGS += -fno-rtti -fexceptions
LOCAL_ARM_NEON := true

include $(BUILD_SHARED_LIBRARY)

# If you don't need your project to build with NDKs older than r21, you can omit
# this block.
ifneq ($(call ndk-major-at-least,21),true)
$(call import-add-path,$(NDK_GRADLE_INJECTED_IMPORT_PATH))
endif

$(call import-module, prefab/common)

