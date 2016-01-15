/*
 * AutoDump.c
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <dlfcn.h>
#include <jni.h>
#include <unistd.h>
#include <AutoDump.H>
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static void* (*Bef_LoadNative)(char* pathName) = NULL;
static void* (*Aft_LoadNative)(char* pathName) = NULL;
static bool isInit_ENV= false;
static char* Unshell_SO = NULL;
//
void MuCpy(char* inDest,char* insrc,size_t inLeng)
{
	inLeng *= 4;
	for(int m_i = 0;m_i < inLeng;m_i++)
	{
		*inDest = *insrc;
		inDest++;
		insrc++;
	}
}
/*
************************************************************
*				Init_ENV
*已匹配成功，现在开始获取处理数据
************************************************************
*/
void Init_ENV(char* inPath)
{
	if(Unshell_SO == NULL)return ;
	void* mHandler = dlopen(Unshell_SO,RTLD_NOW);
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
		DEXLOG("Init_ENV is fail");
	}
}
/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	//判断是否是娜迦加固
	void* mHandler = dlopen("/data/local/MHookDemo/NAGA.so",RTLD_NOW);
	bool (*mTarget)(char*) = NULL;
	bool result = false;
	if(mHandler != NULL)
	{
		mTarget = (Fun_Bool_pchar)dlsym(mHandler,"isTarget");
		result = mTarget(inName);
		if(result)
		{
			Unshell_SO = (char*)malloc(1024);
			memset(Unshell_SO,0,1024);
			strcpy(Unshell_SO,"/data/local/MHookDemo/NAGA.so");
			return result;
		}
	}
	//判断是否是奇虎360加固
	mHandler = dlopen("/data/local/MHookDemo/QH360.so",RTLD_NOW);
	if(mHandler != NULL)
	{
		mTarget = (Fun_Bool_pchar)dlsym(mHandler,"isTarget");
		result = mTarget(inName);
		if(result)
		{
			Unshell_SO = (char*)malloc(1024);
			memset(Unshell_SO,0,1024);
			strcpy(Unshell_SO,"/data/local/MHookDemo/QH360.so");
			return result;
		}
	}
	//判断是否是百度加固
	mHandler = dlopen("/data/local/MHookDemo/Baidu.so",RTLD_NOW);
	if(mHandler != NULL)
	{
		mTarget = (Fun_Bool_pchar)dlsym(mHandler,"isTarget");
		result = mTarget(inName);
		if(result)
		{
			Unshell_SO = (char*)malloc(1024);
			memset(Unshell_SO,0,1024);
			strcpy(Unshell_SO,"/data/local/MHookDemo/Baidu.so");
			return result;
		}
	}
	//判断是否是通付盾加固
	mHandler = dlopen("/data/local/MHookDemo/PayEgis.so",RTLD_NOW);
	if(mHandler != NULL)
	{
		mTarget = (Fun_Bool_pchar)dlsym(mHandler,"isTarget");
		result = mTarget(inName);
		if(result)
		{
			Unshell_SO = (char*)malloc(1024);
			memset(Unshell_SO,0,1024);
			strcpy(Unshell_SO,"/data/local/MHookDemo/PayEgis.so");
			return result;
		}
	}
	//判断 是否是梆梆加固
	mHandler = dlopen("/data/local/MHookDemo/Bangcle.so",RTLD_NOW);
	if(mHandler != NULL)
	{
		mTarget = (Fun_Bool_pchar)dlsym(mHandler,"isTarget");
		result = mTarget(inName);
		if(result)
		{
			Unshell_SO = (char*)malloc(1024);
			memset(Unshell_SO,0,1024);
			strcpy(Unshell_SO,"/data/local/MHookDemo/Bangcle.so");
			return result;
		}
	}
	//判断 是否啊里
	mHandler = dlopen("/data/local/MHookDemo/Ali.so",RTLD_NOW);
	if(mHandler != NULL)
	{
		mTarget = (Fun_Bool_pchar)dlsym(mHandler,"isTarget");
		result = mTarget(inName);
		if(result)
		{
			Unshell_SO = (char*)malloc(1024);
			memset(Unshell_SO,0,1024);
			strcpy(Unshell_SO,"/data/local/MHookDemo/Ali.so");
			return result;
		}
	}
	return false;
}
/*
************************************************************
*				SetAppName
* 直接设置AppName
************************************************************
*/
void SetAppName(void* inAppName)
{
	AppName = (char*)inAppName;
}
/*
************************************************************
*				SetJavaVM
* 直接设置GVM
************************************************************
*/
void SetJavaVM(void* inJavaVM)
{
	GVM = (JavaVM*)inJavaVM;
}
/*
************************************************************
*				SetBef_LoadNative
*这个是供上层调用
*主要是LoadNative之前处理
************************************************************
*/
void* SetBef_LoadNative(char* pathName)
{
	if(!isTarget(pathName))return pathName;
	if(!isInit_ENV)
	{
		DEXLOG("SetBef_LoadNative isInit_ENV");
		isInit_ENV =true;
		Init_ENV(pathName);
	}
	if(Bef_LoadNative != NULL)
	{
		pathName = Bef_LoadNative(pathName);
	}
	return pathName;
}
/*
************************************************************
*				SetAft_LoadNative
* 这个是供上层调用
* 主要是LoadNative之后处理
************************************************************
*/
void* SetAft_LoadNative(char* pathName)
{
	if(Aft_LoadNative != NULL)
	{
		pathName = Aft_LoadNative(pathName);
	}
	return pathName;
}





