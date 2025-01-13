LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

EXT_LIB := $(LOCAL_PATH)/../../../../ext-libs
ANDROID_LIB := $(EXT_LIB)/$(TARGET_ARCH_ABI)/system/hrv

LOCAL_MODULE    := common

LOCAL_SRC_FILES := Log.cpp \
                   JNIBase.cpp \
                   Data.cpp \
                   Backtrace.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
                    $(EXT_LIB)/include/base \
                    $(EXT_LIB)/include/system/core

LOCAL_LDFLAGS       += -rdynamic

LOCAL_LDLIBS := -lz -llog -landroid
LOCAL_LDLIBS += -l$(ANDROID_LIB)/libandroid_runtime.so -l$(ANDROID_LIB)/libcutils.so

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_CFLAGS := -funwind-tables -Wl,--no-merge-exidx-entries -fno-omit-frame-pointer
LOCAL_ARM_NEON := true

include $(BUILD_SHARED_LIBRARY)


