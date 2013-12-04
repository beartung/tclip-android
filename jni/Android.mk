LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
OPENCV_LIB_TYPE:=STATIC
include $(OPENCV_PACKAGE_DIR)/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := jtclip.cpp tclip.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDFLAGS    += -ljnigraphics
LOCAL_LDLIBS     += -llog -ldl -landroid

LOCAL_MODULE     := jtclip

include $(BUILD_SHARED_LIBRARY)
