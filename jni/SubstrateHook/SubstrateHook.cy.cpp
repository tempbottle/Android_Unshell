/*
 * SubstrateHook.cy
 *	1.本程序使用Substate框架，可以使用注入来实现加载。
 *	2.
 *
 *  Created on: 2015年12月21日
 *      Author: LsMouse
 */
#include <stdio.h>
#include "Utils.H"
#include "SubHook.H"
#include "SubstrateHook.cy.H"
//存放读取的配置文件
char* Config = NULL;
char* AppName = NULL;
JavaVM* GVM = NULL;
void* (*Bef_LoadNative)(char* pathName) = NULL;
void* (*Aft_LoadNative)(char* pathName) = NULL;
/*
************************************************************
*				Init_ENV
* 初始化Init_ENV所需环境
************************************************************
*/
void Init_ENV(char* inLib,char* inPath)
{
	if(inLib == NULL)return ;
	void* mHandler = dlopen(inLib,RTLD_NOW);
	if(mHandler != NULL)
	{
		void* f_SetAppName = dlsym(mHandler,"SetAppName");
		void* f_StartDump = dlsym(mHandler,"StartDump");
		void* f_SetBef_LoadNative = dlsym(mHandler,"SetBef_LoadNative");
		void* f_SetAft_LoadNative = dlsym(mHandler,"SetAft_LoadNative");
		if(f_SetAppName != NULL)
		{
			DEXLOG("Init_ENV SetAppName is Find");
			FunV_Void_pchar(m_fnc) = (Fun_Void_pchar)f_SetAppName;
			m_fnc(AppName);
		}
		if(f_SetBef_LoadNative != NULL)
		{
			DEXLOG("Init_ENV SetBef_LoadNative is Find");
			Bef_LoadNative = (Fun_pvoid_pchar)f_SetBef_LoadNative;
		}
		if(f_SetAft_LoadNative != NULL)
		{
			DEXLOG("Init_ENV SetAft_LoadNative is Find");
			Aft_LoadNative = (Fun_pvoid_pchar)f_SetAft_LoadNative;
		}
		if(f_StartDump != NULL)
		{
			DEXLOG("Init_ENV StartDump is Find");
			FunV_Void_pchar(m_fnc) =(Fun_Void_pchar)f_StartDump;
			m_fnc(inPath);
		}
	}else
	{
		DEXLOG("Init_ENV is fail:%s",inLib);
	}
}
/*
************************************************************
*				New$dvmLoadNativeCode
*用于SO 加载，利用一个SO加载判断类型
************************************************************
*/

bool (*$dvmLoadNativeCode)(char* pathName, void* classLoader, char** detail) = NULL;
bool New$dvmLoadNativeCode(char* pathName, void* classLoader, char** detail)
{
	bool result = false;
	Utils::getDDAppConfig();
	if(AppName == NULL){
		AppName = Utils::DDAppTarget(pathName);
		if(AppName != NULL)
		{
			DEXLOG("New$dvmLoadNativeCode App:%s,So:%s",AppName,pathName);
			//筛选结束。
			if(Utils::getDDAppMode() == DDAMODE_DEBUG)
			{
				DEXLOG("DDAMODE_DEBUG");
				Init_ENV("/data/local/MHookDemo/Shell_Debug.so",pathName);
			}else if(Utils::getDDAppMode() == DDAMODE_TestDump)
			{
				DEXLOG("DDAMODE_TestDump");
				Init_ENV("/data/local/MHookDemo/TestDump.so",pathName);
			}else if(Utils::getDDAppMode() == DDAMODE_AutoDump)
			{
				//获取动态加载函数数据，实现
				DEXLOG("DDAMODE_AutoDump");
				Init_ENV("/data/local/MHookDemo/AutoDump.so",pathName);
			}
		}
	}
	//正常运行，一般使用劫持
	if(Bef_LoadNative != NULL)
	{
		pathName = (char*)Bef_LoadNative(pathName);
	}
	result =$dvmLoadNativeCode(pathName,classLoader,detail);
	//一般使用DUMP，但是这里DUMP可能使一些指令ODEX化
	if(Aft_LoadNative != NULL)
	{
		pathName =  (char*)Aft_LoadNative(pathName);
	}
	return result;
}
/**
 *			MSInitialize
 *	程序入口点，
 * 		一定是最开始运行，但是不一定是进程中最开始运行
 *	程序只HOOK了LoadNativeCode来判别是否调试进程
 */
MSInitialize
{
	//开始一些基本Hook来捕捉程序
	Utils::getDDAppConfig();
	if(Utils::getDDAppMode() == 0)
	{
		DEXLOG("Substrate is Close");
		return;
	}
	if($dvmLoadNativeCode == NULL)
	{
		SubHook(libdvm,dvm_LoadNativeCode,(void*)&New$dvmLoadNativeCode,(void**)&$dvmLoadNativeCode);
	}

}
/*
************************************************************
*				JNI_OnLoad
*这是JNI_OnLoad的声明，必须按照这样的方式声明
************************************************************
*/
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
{
	DEXLOG("Substrate JNI_OnLoad");
	//保存全局JavaVM
	GVM = vm;
	//注册时在JNIEnv中实现的，所以必须首先获取它
	JNIEnv* env = NULL;
	//从JavaVM获取JNIEnv，一般使用1.4的版本
	if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
		return -1;
	jclass mClass = findAppClass(env,"ls/mhookdemo/MainActivity");
	if(mClass != NULL)
	{
		DEXLOG("mClass %p",mClass);
	}
	//这里很重要，必须返回版本，否则加载会失败。
	return JNI_VERSION_1_4;
}
/*
************************************************************
*				hook_entry
*这个提供注入函数调用
************************************************************
*/
void __attribute__ ((constructor)) hook_entry(void);
void hook_entry(char * inStart){
	DEXLOG("Hook success, pid = %d\n", getpid());
	//开始一些基本Hook来捕捉程序
	Utils::getDDAppConfig();
	if(Utils::getDDAppMode() == 0 )
	{
		DEXLOG("Substrate is Close");
		return ;
	}
	//
	if($dvmLoadNativeCode == NULL)
	{
		SubHook(libdvm,dvm_LoadNativeCode,(void*)&New$dvmLoadNativeCode,(void**)&$dvmLoadNativeCode);
	}
}


