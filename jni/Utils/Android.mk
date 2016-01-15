#设置CFLAGS、设置头文件
CFLAGS = -Wwrite-strings -Wpointer-arith -fpermissive -Wwrite-strings -fpermissive -Woverflow 
INCLUDE_PATH := $(LOCAL_DIR)/Utils   		
LOCAL_PATH := $(call my-dir)				
#设置通用模块头文件
include $(CLEAR_VARS)
LOCAL_MODULE    := lib-Utils
LOCAL_C_INCLUDES:= $(INCLUDE_PATH)
LOCAL_SRC_FILES := Common.cpp DDConfig.cpp \
				   ModMemory.cpp FindClass.cpp JNI_Class.cpp  FindNative.cpp 
LOCAL_CFLAGS  	:= $(CFLAGS)
LOCAL_LDLIBS := -llog
include $(BUILD_STATIC_LIBRARY)
