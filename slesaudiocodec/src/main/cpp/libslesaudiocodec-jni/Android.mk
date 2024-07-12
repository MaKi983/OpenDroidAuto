LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libslesaudiocodec-jni
LOCAL_SRC_FILES := it_smg_libs_audiocodec_SLESAudioCodec.cpp \
                   SLESAudioCodec.cpp \
                   SLESStream.cpp

LOCAL_LDLIBS += -llog -landroid -lOpenSLES

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

