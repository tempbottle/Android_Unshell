/*
 * Ali.cpp
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <jni.h>
#include <pthread.h>
#include <sys/mman.h>
#include "SubHook.H"
#include "Utils.H"
#include "MDexFile.h"
#include "Ali_Old.H"
//声明变量
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static char* Dex_Parse = NULL;
static MDexFile* MDex;

/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	if(strstr(inName,Ali_mobisec) != 0)
		return true;
	return false;
}
void SetAppName(void* inAppName)
{
	AppName = (char*)inAppName;
}
void SetJavaVM(void* inJavaVM)
{
	GVM = (JavaVM*)inJavaVM;
}
/*
 ************************************************************
 *				New$fgets
 @char *fgets(char *buf, int bufsize, FILE *stream);
 ************************************************************
 */
 char* (*$fgets)(char *buf, int bufsize, FILE *stream);
 char* New$fgets(char *buf, int bufsize, FILE *stream)
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
/*
************************************************************
*				new$dexFileParse
@DexFile* dexFileParse(const u1* data, size_t length, int flags)
*用来Hook来进行获取JavaVM
************************************************************
*/
void* New_Delay_Dump(void* inAddr)
{
	DEXLOG(" Ali New_Delay_Dump");
	sleep(5);
	DEXLOG("New_Test Dump %p ", inAddr);
	MDex = new MDexFile(inAddr,0xFFFFFF);
	MDex->Parse();
	MDex->SaveDex(AppName,"Ali");
	return NULL;
}
void* (*$dexFileParse)(char* inAddr,size_t length, int flags) = NULL;
void* New$dexFileParse(char* inAddr,size_t length, int flags)
{
	void* result =$dexFileParse(inAddr,length,flags);
	DEXLOG("new$dexFileParse %p %p ", inAddr,length);
	pthread_t mpth ;
	pthread_create(&mpth, NULL,New_Delay_Dump,inAddr);
	return result;
}
/**/
void* SetBef_LoadNative(char* pathName)
{

	return pathName;
}
void* SetAft_LoadNative(char* pathName)
{
	return pathName;
}
/*
************************************************************
*				StartDump
* 开始进行Dump处理
* Hook关键点
* $JNI_OnLoad -> 获取GVM
* $dexFileParse -> 获取记载Dex
************************************************************
*/
void StartDump(char* inName)
{
//	dlopen("/data/local/tmp/libbaiduprotect.so",0);
	DEXLOG(" Ali StartDump");
	SubHook(libdvm,dvm_dexFileParse,(void*)&New$dexFileParse,(void**)&$dexFileParse);
//	SubHook(libc,libc_fgets,(void*)&New$fgets,(void**)&$fgets);
//	SubHook(libdvm,dvm_ResolveClass,(void*)&New$dvmResolveClass,(void**)&$dvmResolveClass);
}
