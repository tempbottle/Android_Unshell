#设置本地目录、报警标志（CFLAGS）、头文件搜索目录
CFLAGS = -Wwrite-strings -Wpointer-arith -fpermissive -Wwrite-strings -fpermissive -Woverflow 
INCLUDE_PATH := 	$(LOCAL_DIR)/MDexFile  \
					$(LOCAL_DIR)/Utils  \
					$(LOCAL_DIR)  
LOCAL_PATH := $(call my-dir)					
#静态库—MDexFile 标志 
include $(CLEAR_VARS)
LOCAL_MODULE    := lib-MDexFile
LOCAL_C_INCLUDES:= $(INCLUDE_PATH)
LOCAL_CFLAGS  	:= $(CFLAGS)
LOCAL_SRC_FILES := 	MDexFile.cpp 	\
					DexUtils.cpp \
					MDexFileMap.cpp MDexFileOPT.cpp  \
				   	MDexFileHeader.cpp MDexFileStringId.cpp MDexFileTypeId.cpp \
				   	MDexFileFieldId.cpp MDexFileMethodId.cpp MDexFileProtoId.cpp \
				   	MDexFileClassDef.cpp \
				   	MDexFileAnnot.cpp MDexFileEncode.cpp  MDexFileDebug.cpp 
LOCAL_STATIC_LIBRARIES := 	-L$(SYSROOT)/usr/lib  lib-Utils	
include $(BUILD_STATIC_LIBRARY)

