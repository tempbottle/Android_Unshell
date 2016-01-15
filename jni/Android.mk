#设置路径
CFLAGS := -Wwrite-strings -Wpointer-arith -fpermissive -Wwrite-strings -fpermissive -Woverflow  -Wformat
LOCAL_DIR := $(call my-dir)
LOCAL_PATH := $(call my-dir)
#包含子Android.mk
include $(call all-subdir-makefiles)


