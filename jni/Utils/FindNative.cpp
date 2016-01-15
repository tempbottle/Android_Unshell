/*
 * FindNative.cpp
 *
 *  Created on: 2016年1月12日
 *      Author: LsMouse
 */
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
/**
 *		FindNative
 *	在inLib库中查找inSym
 */
void* FindNative(char* inLib,char* inSym)
{
	void* mHandler = dlopen(inLib,RTLD_NOW);
	if(NULL == mHandler)
		return NULL;
	void* Func = dlsym(mHandler,inSym);
	return Func;
}



