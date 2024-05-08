LOCAL_PATH:= $(call my-dir)

# BOOST
BOOST_PATH := $(LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE := libboost_system
LOCAL_SRC_FILES := $(BOOST_PATH)//lib/$(TARGET_ARCH_ABI)/libboost_system.a
LOCAL_EXPORT_C_INCLUDES := $(BOOST_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := libboost_log
#LOCAL_SRC_FILES := $(BOOST_PATH)//lib/$(TARGET_ARCH_ABI)/libboost_log.a
#LOCAL_EXPORT_C_INCLUDES := $(BOOST_PATH)/include
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libboost_thread
LOCAL_SRC_FILES := $(BOOST_PATH)//lib/$(TARGET_ARCH_ABI)/libboost_thread.a
LOCAL_EXPORT_C_INCLUDES := $(BOOST_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)
