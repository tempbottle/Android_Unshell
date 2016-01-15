#设置CFLAGS、设置头文件
CFLAGS = -Wwrite-strings -Wpointer-arith -fpermissive -Wwrite-strings -fpermissive -Woverflow 
INCLUDE_PATH := $(LOCAL_DIR)/Utils  \
				$(LOCAL_DIR)/SubHook \
				$(LOCAL_DIR)/MDexFile \ 	
LOCAL_PATH := $(call my-dir)				

# MAIN
include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(INCLUDE_PATH)
LOCAL_MODULE    := SubstrateHook.cy
LOCAL_SRC_FILES := SubstrateHook.cy.cpp 
LOCAL_CFLAGS  	:= $(CFLAGS)
LOCAL_LDLIBS := -llog -lsubstrate-dvm -lsubstrate
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS += -L$(LOCAL_PATH) 
LOCAL_STATIC_LIBRARIES := 	lib-Utils lib-SubHook lib-MDexFile		  
include $(BUILD_SHARED_LIBRARY) 