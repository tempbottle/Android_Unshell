/*
 * Utils.H
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <jni.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <android/log.h>
#include "Common.H"
#include "DDConfig.H"
#include "ModMemory.H"
#include "FastLink.H"
#include "MeVertor.H"
#include "FindClass.H"
#include "JNI_Class.H"
#include "FindNative.H"
//
#define	DEXLOG(...)		((void)__android_log_print(ANDROID_LOG_DEBUG, "LsShell", __VA_ARGS__))

typedef jint (*pJni_load)(JavaVM*, void *);
//
#define AddAddr2(arg0,arg1)		((u4)arg0+(u4)arg1)
#define SubAddr2(arg0,arg1)		((u4)arg0-(u4)arg1)
#define SubAddr3(arg0,arg1,arg2)		((u4)arg0-(u4)arg1-(u4)arg2)
#define Ali(arg0,arg1)			(((u4)arg0+((u4)arg1-1))&(~((u4)arg1-1)))
//




#endif /* UTILS_H_ */
