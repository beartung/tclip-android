LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
include $(OPENCV_PACKAGE_DIR)/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := jtclip.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl
#LOCAL_CFLAGS     += -L $(OPENCV_PACKAGE_DIR)/sdk/native/libs/$(TARGET_ARCH_ABI)/
#LOCAL_LDLIBS     += -L $(OPENCV_PACKAGE_DIR)/sdk/native/libs/$(TARGET_ARCH_ABI)/ -llog -ldl

LOCAL_MODULE     := jtclip

include $(BUILD_SHARED_LIBRARY)
