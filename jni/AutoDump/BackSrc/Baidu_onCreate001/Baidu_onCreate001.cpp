/*
 * Baidu.cpp
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
#include "Baidu_onCreate001.H"
#include "Utils/Utils.H"
#include "MDexFile/MDexFile.h"
//声明变量
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static bool isRepair = false;
/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	if(strstr(inName,Baidu_protect) != NULL)
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
	GVM = vm;
	return $JNI_OnLoad(vm,reserved);
}
/*
************************************************************
*				new$dexFileParse
@DexFile* dexFileParse(const u1* data, size_t length, int flags)
*用来Hook来进行获取JavaVM
************************************************************
*/
void* (*$dexFileParse)(char* inAddr,size_t length, int flags) = NULL;
void* New$dexFileParse(char* inAddr,size_t length, int flags)
{
	void* result =$dexFileParse(inAddr,length,flags);
	DEXLOG("Baidu::new$dexFileParse %p %p ", inAddr,length);
	UnShell(inAddr,length);
	return result;
}
//_________________________________________________________________________________________________________________
//加密Hook
//将加载函数取消调用
void (*$ENC)(JNIEnv*,jobject,jobject) = NULL;
void New$ENC(JNIEnv *env, jobject obj,jobject mJsrt)
{
	DEXLOG("My_BaidDu_A_e %s ", Utils::jstringTostring(env,(jstring)mJsrt));
//	$BaidDu_A_e(env,obj,mJsrt);
}
//解密Hook
//判断是否解密成功，解密成功直接取消调用
void (*$DEC)(JNIEnv*,jobject,jobject) = NULL;
void New$DEC(JNIEnv *env, jobject obj,jobject mJsrt)
{
	DEXLOG("My_BaidDu_A_d %s ", Utils::jstringTostring(env,(jstring)mJsrt));
	if(!isRepair){
		$DEC(env,obj,mJsrt);
	}
}
/*
************************************************************
*				RestoreCode
* 将传入参数，直接解密
* 传入需要修复的类名，自动补齐名字后进行修复
* pstr->类名
************************************************************
*/
void RestoreCode(JNIEnv* env,char* pstr){
	char* mstr = (char*)malloc(1024);
	memset(mstr,0,1024);
	sprintf(mstr,"%s->onCreate001(Landroid/os/Bundle;)V",pstr);
	DEXLOG("RestoreCode %s ", mstr);
	jstring mJsrt = Utils::pcharToJstring(env,mstr,NULL);
	$DEC(env,NULL,mJsrt);
	free(mstr);
	env->DeleteLocalRef(mJsrt);
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
	DEXLOG("Baidu StartDump");
	SubHook(inName,Final_JNI_OnLoad,(void*)&New$JNI_OnLoad,(void**)&$JNI_OnLoad);
	SubHook(libdvm,dvm_dexFileParse,(void*)&New$dexFileParse,(void**)&$dexFileParse);
}
void* SaveFile(void* inDex)
{
	MDexFile* mDexFile = (MDexFile*)inDex;
	mDexFile->SaveDex(AppName,"Baidu_Unshell");
	sleep(10);
	return NULL;
}
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
void UnShell(char* inAddr,size_t length)
{
	JNIEnv* env = NULL;
	if(NULL == GVM)
	{
		DEXLOG("Baidu UnShell GVM is NULL!");
		return ;
	}
	if(GVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){
		DEXLOG("Baidu UnShell GetEnv fail！");
		return ;
	}
	jclass cls_protect = env->FindClass("com/baidu/protect/A");
	if(cls_protect == NULL){
		DEXLOG("Baidu UnShell com/baidu/protect/A class no find！");
		return ;
	}
	jmethodID method_enc = env->GetStaticMethodID(cls_protect,"e","(Ljava/lang/String;)V");
	jmethodID method_dec = env->GetStaticMethodID(cls_protect,"d","(Ljava/lang/String;)V");
	if((method_enc == NULL) || (method_dec == NULL))
	{
		DEXLOG("Baidu UnShell method_enc or method_dec no find！");
		return ;
	}
	MSJavaHookMethod(env,cls_protect,method_enc,(void*)&New$ENC,(void**)&$ENC);
	MSJavaHookMethod(env,cls_protect,method_dec,(void*)New$DEC,(void**)&$DEC);
	//
	DexUtils::SaveFile(inAddr,length*10,AppName,"Parse");
	DEXLOG("Baidu UnShell start parse！");
	//开始解析，并保存数据
	MDexFile* mDexFile = new MDexFile(inAddr,length);
	mDexFile->Parse();
	int Ids_onCreate = mDexFile->pStringIds->getIds("onCreate");
	int Ids_onCreate001 = mDexFile->pStringIds->getIds("onCreate001");
	DEXLOG("Baidu UnShell onCreate %d Ids_onCreate001 %d！",Ids_onCreate,Ids_onCreate001);
	MDexFile_StringId* pStringIds = mDexFile->pStringIds;
	MDexFile_TypeId* 	pTypeIds = mDexFile->pTypeIds;
	MDexFile_MethodId* pMethodIds = mDexFile->pMethodIds;
	pMethodIds->methodIds.Reset();
	while(pMethodIds->methodIds.Get() != NULL)
	{
		int mIdx = pMethodIds->methodIds.Get()->nameIdx;
		if(mIdx == Ids_onCreate001)
		{
			size_t classIdx =  pMethodIds->methodIds.Get()->classIdx;
			//从类名中获取字符串
			size_t inType = pTypeIds->getIdxNoType(classIdx);
			char* mString = pStringIds->getStringByIds(inType);
			DEXLOG("onCreate001 : %s",mString);
			RestoreCode(env,mString);
		}
		pMethodIds->methodIds.Next();
	}
	isRepair = true;
	//修复完毕先Dump
	DexUtils::SaveFile(inAddr,length*10,AppName,"Repair");
	//重新解析classdef
	mDexFile->pClassDefs = new MDexFile_ClassDef(mDexFile);
	//这里数据已经被完全修复，只需要将数据重新整理然后 DUMP
	pthread_t pth;
	pthread_create(&pth,NULL,SaveFile,mDexFile);
//	mDexFile->SavetoFile(AppName,"Baidu_Unshell");

}
