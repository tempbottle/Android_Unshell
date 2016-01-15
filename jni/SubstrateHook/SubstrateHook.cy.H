/*
 * SubstrateHook.cy.H
 *
 *  Created on: 2015年11月30日
 *      Author: LsMouse
 */

#ifndef SUBSTRATEHOOK_CY_H_
#define SUBSTRATEHOOK_CY_H_
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <jni.h>
#include "Utils.H"
#include "SubHook.H"
//
MSConfig(MSFilterExecutable, "/system/bin/app_process")
//定义函数指针
#define FunV_Void_pchar(arg) 	void(*arg)(char*)
#define Fun_Void_pchar 			void(*)(char*)
#define FunV_pvoid_pchar(arg) 	void*(*arg)(char*)
#define Fun_pvoid_pchar 		void*(*)(char*)
#define FunV_Bool_pchar	(arg)	bool(*arg)(char*)
#define Fun_Bool_pchar			bool(*)(char*)
//extern void (*Bef_LoadNative)(char* pathName);
//extern void (*Aft_LoadNative)(char* pathName);
#endif /* SUBSTRATEHOOK_CY_H_ */
