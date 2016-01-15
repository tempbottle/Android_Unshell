/*
 * QH360.cpp
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */
#include "QH360.H"
#include <sys/mman.h>
#include "Utils/Utils.H"
#include "SubHook/SubHook.H"
#include "MDexFile/MDexFile.h"
typedef jint (*pJni_load)(JavaVM*, void *);
//申请变量
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static pJni_load QH_Jni_load = NULL;
/*
************************************************************
*				QH360_DumpDex
* 开始进行360DumpDex
@void QH360_DumpDex()
************************************************************
*/
void QH360_DumpDex(){
	DEXLOG("QH360_DumpDex");
	unsigned long mStart =Utils::getStartAddrByName("/dev/zero","rwxs");
	unsigned long mEnd =Utils::getEndAddrByName("/dev/zero","rwxs");
	if(Utils::getEndAddrByName("/dev/zero","rw-s") != 0)
	{
		mEnd = Utils::getEndAddrByName("/dev/zero","rw-s");
	}
	if(mStart == 0){
		mStart =Utils::getStartAddrByName("/dev/zero","rw-s");
	}
	if(mStart == 0)
	{
		mStart =Utils::getStartAddrByName("/dev/zero");
		DexUtils::SaveFile((void*)mStart,0xFFFFFFF,AppName,"QH360_1");
		mStart = *(int*)(mStart+4);
		DexUtils::SaveFile((void*)mStart,0xFFFFFFF,AppName,"QH360_2");
	}else
	{
		DEXLOG("QH360_DumpDex start:%p mlength：%p",mStart,mEnd-mStart);
		DexUtils::SaveFile((void*)mStart,0xFFFFFFF,AppName,"QH360");
	}
}
/*
************************************************************
*				New$memcpy
* Hook memcpy函数防止程序对Dex进行清除
@void memcpy(char *dest, char* src, unsigned n)
************************************************************
*/
static void (*$memcpy)(char *dest, char* src, unsigned n);
static void New$memcpy(char *dest, char* src, unsigned n){
	if((n == 0x70)&&
		(*dest == 'd')&&(*(dest+1) == 'e')&&(*(dest+2) == 'x')){

		return ;
	}
	$memcpy(dest,src,n);
}
/*
************************************************************
*					New$dlsym
*用来Hook来进行获取JavaVM
@
************************************************************
*/
static void* (*$dlsym)(void*handle,const char*symbol) = NULL;
jint QH360_UnShell(JavaVM* vm, void *reserved){
	DEXLOG("QH360 UnShell");
	if(QH_Jni_load == NULL)
	{
		DEXLOG("QH360 QH_Jni_load is Null");
		return -1;
	}
	QH_Jni_load(vm,reserved);
	DEXLOG("QH360 Start Dump");
	QH360_DumpDex();
	return JNI_VERSION_1_4;
}
static void* New$dlsym(void*handle,const char*symbol){
	DEXLOG("QH360 New$dlsym JNI_OnLoad %s",symbol);
	void* res = $dlsym(handle,symbol);
	if(strcmp(symbol,"JNI_OnLoad")!= 0)return res;
	if(QH_Jni_load != NULL)return res;
	QH_Jni_load = res;
	return &QH360_UnShell;
}

/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	if(strstr(inName,Final_360_Jiagu) != NULL)
	{
		return true;
	}else if(strstr(inName,Final_360_Jiagu_2) != NULL)
	{
		return true;
	}
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
void* SetAft_LoadNative(char* pathName)
{
	if(!isTarget(pathName))
		return pathName;
	QH360_DumpDex();
	return pathName;
}
/*
************************************************************
*				StartDump
* 开始进行Dump处理
* Hook关键函数点进行定点
************************************************************
*/
void StartDump(char* inPath)
{
	DEXLOG("StartDump %s",inPath);
	//直接进行Hook
	if(NULL == $memcpy)
	{
		DEXLOG("QH360 StartDump SubHook memcpy");
		SubHook(libc,libc_memcpy,(void*)&New$memcpy,(void**)&$memcpy);
	}
	if(NULL == $dlsym)
	{
		dlopen(inPath,RTLD_NOW);
		DEXLOG("QH360 StartDump SubHook dlsym");
		SubHook("libc.so",libc_dlsym,(void*)&New$dlsym,(void**)&$dlsym);
	}
}






