/*
 * JNI_Class.cpp
 *
 *  Created on: 2016年1月11日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stddef.h>
#include <jni.h>
/*
************************************************************
*			Clear_Exception
*	清除异常
************************************************************
*/
void Clear_Exception(JNIEnv* env)
{
	jthrowable exception = env->ExceptionOccurred();
	if (exception != NULL)
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}



