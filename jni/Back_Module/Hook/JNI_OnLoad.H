/*
 * JNI_OnLoad.H
 *
 *  Created on: 2016年1月5日
 *      Author: LsMouse
 */
#ifndef JNI_ONLOAD_H_
#define JNI_ONLOAD_H_
#include "stdio.h"
#include "SubHook/SubHook.H"
static JavaVM* OnLoad_GVM;
/*
************************************************************
*				$new$JNI_OnLoad
*用来Hook来进行获取JavaVM
@JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
************************************************************
*/
static void* (*$JNI_OnLoad)(JavaVM* vm, void *reserved) = NULL;
static void* New$JNI_OnLoad(JavaVM* vm, void *reserved)
{
	//通过Hook获取到JavaVM，保存JavaVM
	OnLoad_GVM = vm;
	return $JNI_OnLoad(vm,reserved);
}
static void Hook_Jni_OnLoad(char* inName)
{
	SubHook(inName,Final_JNI_OnLoad,(void*)&New$JNI_OnLoad,(void**)&$JNI_OnLoad);
}

#endif
/* JNI_ONLOAD_H_ */
