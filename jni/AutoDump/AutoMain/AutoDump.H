/*
 * AutoDump.H
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */

#ifndef AUTODUMP_H_
#define AUTODUMP_H_
//日志
#include <android/log.h>
#define DEXLOG(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "LsShell", __VA_ARGS__))
//定义函数指针
#define FunV_Void_pchar(arg) 	void(*arg)(char*)
#define Fun_Void_pchar 			void(*)(char*)
#define FunV_pvoid_pchar(arg) 	void*(*arg)(char*)
#define Fun_pvoid_pchar 		void*(*)(char*)
#define FunV_Bool_pchar	(arg)	bool(*arg)(char*)
#define Fun_Bool_pchar			bool(*)(char*)
//
extern "C"
{
	extern void MuCpy(char* inDest,char* insrc,size_t inLeng);
	extern void SetAppName(void* inAppName);
	extern void SetJavaVM(void* inJavaVM);
	extern bool isTarget(char* inName);
	extern void* SetBef_LoadNative(char* pathName);
	extern void* SetAft_LoadNative(char* pathName);
}



#endif /* AUTODUMP_H_ */
