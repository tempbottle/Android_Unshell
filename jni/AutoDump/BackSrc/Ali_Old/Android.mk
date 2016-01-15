CFLAGS := -Wwrite-strings -Wpointer-arith -fpermissive -Wwrite-strings -fpermissive -Woverflow  -Wformat
INCLUDE_PATH := $(LOCAL_DIR)/SubHook  \
				$(LOCAL_DIR)/Utils 	\
				$(LOCAL_DIR)/MDexFile 
LOCAL_PATH := $(call my-dir)
#Ali_Old 、Ali_ChangeClassDef
include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(INCLUDE_PATH)
LOCAL_MODULE    := Ali_Old
LOCAL_SRC_FILES := Ali_Old.cpp
LOCAL_STATIC_LIBRARIES := 	lib-Utils lib-SubHook lib-MDexFile  substrate-dvm substrate 
LOCAL_LDLIBS := -llog  -L$(LOCAL_PATH) 
LOCAL_LDFLAGS := $(LOCAL_DIR)/libsubstrate-dvm.so $(LOCAL_DIR)/libsubstrate.so
LOCAL_CFLAGS  	:= $(CFLAGS)
include $(BUILD_SHARED_LIBRARY) 