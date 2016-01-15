/*
 * PayEgis_V3_10
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
#include "SubHook/SubHook.H"
#include "PayEgis_V3_10.H"
#include "Utils/Utils.H"
#include "MDexFile/MDexFile.h"
//声明变量
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static bool isRepair = false;
static MDexFile* Dex = NULL;
MeVertor<BackClass>* bClass = NULL;
/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	if(strstr(inName,PayEgis_protect) != NULL)
	{
		return true;
	}else if(strstr(inName,PayEgisboot_protect) != NULL)
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
/*
************************************************************
*				$new$JNI_OnLoad
*用来Hook来进行获取JavaVM
@JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
************************************************************
*/
void* (*$JNI_OnLoad)(JavaVM* vm, void *reserved) = NULL;
void* New$JNI_OnLoad(JavaVM* vm, void *reserved)
{
	//通过Hook获取到JavaVM，保存JavaVM
	void* result ;
	DEXLOG("new$JNI_OnLoad GVM is get!");
	GVM = vm;
	result =  $JNI_OnLoad(vm,reserved);
	//
	unsigned long mStart =Utils::getStartAddrByName("/data/dalvik-cache/data");
	if(mStart != 0)
	{
		DEXLOG("通付盾 Dump");
		DexUtils::SaveFile((void*)mStart,0xFFFFFFF,AppName,"Payegis_Static");
	}else{
		DEXLOG("通付盾 get dex seg fail");
	}
	//先解析现在函数
	Dex = new MDexFile((void*)mStart,0xFFFFFFF);
	Dex->Parse();
	//直接获取数据保存地址，
	bClass = new MeVertor<BackClass>;
	bClass->Clear();
	BackClass* back =(BackClass*)((uint)Dex->pBackHeader + Dex->pBackHeader->dataOff+ Dex->pBackHeader->dataSize);
	while(back->id < Dex->pMethodIds->methodIds.GetSize())
	{
		item_ClassMethod* method = Dex->pClassDefs->getClassMethod(back->id);
		if(method != NULL)
		{
			method->flags = back->flags;
			method->Offset =  back->offset;
	//		DEXLOG("method->id: %d ",back->id);
		}else {
			DEXLOG("no find back id:%d",back->id);
		}
		back++;
	}
	Dex->pClassDefs->Reset_ClassCode();
	Dex->SaveDex(AppName,"ODex_PayEgis");
	return result;
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
	dlopen(inName,RTLD_LAZY);
	DEXLOG("通付盾 StartDump");
	SubHook(inName,Final_JNI_OnLoad,(void*)&New$JNI_OnLoad,(void**)&$JNI_OnLoad);
}
void* pth_SaveFile(void* inDex)
{

	sleep(10);
	return NULL;
}
/**/
/*
 *					MatchCode
 * 匹配数据
 * 用来匹配虚拟机指令
 *	const-string                    v0, aYs5hlmeuyi5j # "YS5hLmEuYi5j"
 *	invoke-static                   {v0}, <void ProxyApplication.init(ref) imp. @ _def_ProxyApplication_init@VL>
 */
int PayEgis_MatchCode[0x0A] ={0x1A,0x00,0x100,0x100,
							0x71,0x10,0x100,0x100,0x00,0x00};
/**
 * 				GetNaga_JavaIds
 *  根据clinit指令来获取 调用的stringIds索引
 */

/*
************************************************************
*				UnShell
* 对百度进行脱壳处理
* 1.先判断是否获取到JavaVM 和JNIEnv
* 2.对修复和抹除函数进行Hook
* 3.对内存中的dexFile文件进行解析
* 4.通过解析的dexFile文件进行
* 5.定制Dump数据
************************************************************
*/
void UnShell(JNIEnv* env,jobject obj)
{


	//
}
