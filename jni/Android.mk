LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := sift_prebuilt
LOCAL_SRC_FILES := libnonfree.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)  
LOCAL_MODULE    := opencv_java_prebuilt  
LOCAL_SRC_FILES := libopencv_java.so  
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
include $(OPENCV_PACKAGE_DIR)/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := jtclip.c tclip.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_SHARED_LIBRARIES := sift_prebuilt opencv_java_prebuilt

LOCAL_MODULE     := jtclip

include $(BUILD_SHARED_LIBRARY)
