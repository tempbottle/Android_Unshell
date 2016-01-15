CFLAGS = -Wwrite-strings -Wpointer-arith -fpermissive -Wwrite-strings -fpermissive -Woverflow 
INCLUDE_PATH := $(LOCAL_DIR)/SubHook  \
				$(LOCAL_DIR)/Utils 	\
				$(LOCAL_DIR)/MDexFile 
LOCAL_PATH := $(call my-dir)
#Shell_TestDump
include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(INCLUDE_PATH)
LOCAL_MODULE    := 	Shell_Debug
LOCAL_SRC_FILES := 	Shell_Debug.cpp 

LOCAL_STATIC_LIBRARIES := 	lib-Utils lib-SubHook lib-MDexFile
LOCAL_LDLIBS := -llog  -L$(LOCAL_PATH) 
LOCAL_LDFLAGS := $(LOCAL_DIR)/libsubstrate-dvm.so $(LOCAL_DIR)/libsubstrate.so
LOCAL_CFLAGS  	:= $(CFLAGS)
include $(BUILD_SHARED_LIBRARY) 