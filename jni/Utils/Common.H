#ifndef __COMMON_H_
#define __COMMON_H_
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "jni.h"
#include <android/log.h>
//定义日志输出
#define	DEXLOG(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "LsShell", __VA_ARGS__))
//声明函数供外部调用
namespace Utils{
//	函数申明
	char* getpidName(pid_t inpid);
	char* getConfig();
	char* getSelfName();
	char* getSelfCmdline();
	unsigned long getStartAddrByName(char* inName);
	unsigned long getEndAddrByName(char* inName);
	unsigned long getStartAddrByName(char* inName,char* inRW);
	unsigned long getEndAddrByName(char* inName,char* inRW);
	char* jstringTostring(JNIEnv* env, jstring jstr);
	jstring pcharToJstring(JNIEnv* env, const char* pat, const char* encoding);

	void SaveFile(char* appName,char* inFile,void* inAddr,size_t inLen);
	char*  GetTimeName(char* inName);
//
};
#endif
