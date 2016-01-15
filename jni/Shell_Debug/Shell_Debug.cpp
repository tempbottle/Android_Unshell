/*
 *
 *  Created on: 2015年11月30日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <jni.h>
#include <pthread.h>
#include "SubHook.H"
#include "Utils.H"
#include "MDexFile.h"
#include "Shell_Debug.H"

static char* AppName = NULL;
static JavaVM* GVM = NULL;
static void* (*Bef_LoadNative)(char* pathName) = NULL;
static void* (*Aft_LoadNative)(char* pathName) = NULL;
/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	dlopen("/data/local/tmp/libedog.so",RTLD_NOW);
	return true;
}
void SetAppName(void* inAppName)
{
	AppName = (char*)inAppName;
}
void SetJavaVM(void* inJavaVM)
{
	GVM = (JavaVM*)inJavaVM;
}
void* SetBef_LoadNative(char* pathName)
{

	if(Bef_LoadNative != NULL)
		Bef_LoadNative(pathName);
//开始Hook JNI_OnLoad
	return "/data/local/tmp/libedog.so";
}
void* SetAft_LoadNative(char* pathName)
{
	if(Aft_LoadNative != NULL)
		Aft_LoadNative(pathName);
	return "/data/local/tmp/libedog.so";
}
/*
 ************************************************************
 *				New$fgets
 @char *fgets(char *buf, int bufsize, FILE *stream);
 ************************************************************
 */
static char* (*$fgets)(char *buf, int bufsize, FILE *stream);
static char* New$fgets(char *buf, int bufsize, FILE *stream)
 {
 	char* result = $fgets(buf,bufsize,stream);
 	if(strstr(buf,"TracerPid:") != NULL)
 	{
 		memset(buf,0,40);
 		strcpy(buf,"TracerPid:0");
 	}else{
 		return result;
 	}
 	return buf;
 }
/**
************************************************************
 *	设置调试细节
 *	主要为第一个SO库加载是配置环境，主要用于开始Hook和劫持SO
************************************************************
 */
void StartDump(char* inName)
{
	SubHook(libc,libc_fgets,(void*)&New$fgets,(void**)&$fgets);
}
