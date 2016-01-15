#设置本地目录、报警标志（CFLAGS）、头文件搜索目录
CFLAGS = -Wwrite-strings -Wpointer-arith -fpermissive -Wwrite-strings -fpermissive -Woverflow 
INCLUDE_PATH := $(LOCAL_DIR)/SubHook   	
LOCAL_PATH := $(call my-dir)

#静态库SubHook
include $(CLEAR_VARS)
LOCAL_MODULE    := lib-SubHook
LOCAL_SRC_FILES := SubHook.cpp
LOCAL_C_INCLUDES:= $(INCLUDE_PATH)
LOCAL_CFLAGS  	:= $(CFLAGS)
LOCAL_LDLIBS := -llog 
LOCAL_LDFLAGS := $(LOCAL_DIR)/libsubstrate-dvm.so $(LOCAL_DIR)/libsubstrate.so

include $(BUILD_STATIC_LIBRARY)
