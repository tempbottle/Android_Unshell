/*
 * FindClass.cpp
 *
 *  Created on: 2015年12月11日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <ctype.h>
#include <jni.h>
#include "Common.H"
#include "FindClass.H"
/**
 *	清楚异常
 */
int ClearException(JNIEnv *jenv){
    jthrowable exception = (jenv)->ExceptionOccurred();
    if (exception != NULL) {
        (jenv)->ExceptionDescribe();
        (jenv)->ExceptionClear();
        return 1;
    }
    return 0;
}
/**
 *	类查找
 */
jclass findAppClass(JNIEnv *jenv,const char *apn){

    jclass clazzApplicationLoaders = (jenv)->FindClass("android/app/ApplicationLoaders");
    jthrowable exception = (jenv)->ExceptionOccurred();
    if (ClearException(jenv)) {
        DEXLOG("Exception","No class : %s", "android/app/ApplicationLoaders");
        return NULL;
    }
    jfieldID fieldApplicationLoaders = (jenv)->GetStaticFieldID(clazzApplicationLoaders,"gApplicationLoaders","Landroid/app/ApplicationLoaders;");
    if (ClearException(jenv)) {
    	DEXLOG("Exception","No Static Field :%s","gApplicationLoaders");
        return NULL;
    }
    jobject objApplicationLoaders = (jenv)->GetStaticObjectField(clazzApplicationLoaders,fieldApplicationLoaders);
    if (ClearException(jenv)) {
    	DEXLOG("Exception","GetStaticObjectField is failed [%s","gApplicationLoaders");
        return NULL;
    }
    jfieldID fieldLoaders = (jenv)->GetFieldID(clazzApplicationLoaders,"mLoaders","Landroid/util/ArrayMap;");
    if (ClearException(jenv)) {
    	DEXLOG("Exception","No Field :%s","mLoaders");
        return NULL;
    }
    jobject objLoaders = (jenv)->GetObjectField(objApplicationLoaders,fieldLoaders);
    if (ClearException(jenv)) {
    	DEXLOG("Exception","No object :%s","mLoaders");
        return NULL;
    }
    //??map??alues
    jclass clazzHashMap = (jenv)->GetObjectClass( objLoaders);
    jmethodID methodValues = (jenv)->GetMethodID( clazzHashMap,"values","()Ljava/util/Collection;");
    jobject values = (jenv)->CallObjectMethod( objLoaders,methodValues);

    jclass clazzValues = (jenv)->GetObjectClass( values);
    jmethodID methodToArray = (jenv)->GetMethodID( clazzValues,"toArray","()[Ljava/lang/Object;");
    if (ClearException(jenv)) {
    	DEXLOG("Exception","No Method:%s","toArray");
        return NULL;
    }

    jobjectArray classLoaders = (jobjectArray)(jenv)->CallObjectMethod( values,methodToArray);
    if (ClearException(jenv)) {
    	DEXLOG("Exception","CallObjectMethod failed :%s","toArray");
        return NULL;
    }

	int size = (jenv)->GetArrayLength( classLoaders);
	int i = 0;
	for(i = 0 ; i < size ; i ++){
		jobject classLoader = (jenv)->GetObjectArrayElement( classLoaders,i);
		jclass clazzCL = (jenv)->GetObjectClass( classLoader);
		jmethodID loadClass = (jenv)->GetMethodID( clazzCL,"loadClass","(Ljava/lang/String;)Ljava/lang/Class;");
		jstring param = (jenv)->NewStringUTF( apn);
		jclass tClazz = (jclass)(jenv)->CallObjectMethod( classLoader,loadClass,param);
		if (ClearException(jenv)) {
			DEXLOG("Exception","No");
			continue;
		}
		return tClazz;
	}
	DEXLOG("Exception","No");
    return NULL;
}
/*
 *
 */
jclass dvmFindJNIClass(JNIEnv *env, const char *classDesc)
{
	jclass classObj = env->FindClass(classDesc);

	if (env->ExceptionCheck() == JNI_TRUE) {
		env->ExceptionClear();
	}
	jclass  clazzHashMap = env->FindClass("android/util/ArrayMap");

	if (env->ExceptionCheck() == JNI_TRUE)
	{
		env->ExceptionClear();
	}

	if (classObj == NULL) {
		jclass clazzApplicationLoaders = env->FindClass("android/app/ApplicationLoaders");
		CHECK_VALID(clazzApplicationLoaders);

		jfieldID fieldApplicationLoaders = env->GetStaticFieldID(
				clazzApplicationLoaders, "gApplicationLoaders",
				"Landroid/app/ApplicationLoaders;");
		CHECK_VALID(fieldApplicationLoaders);

		jobject objApplicationLoaders = env->GetStaticObjectField(
				clazzApplicationLoaders, fieldApplicationLoaders);
		CHECK_VALID(objApplicationLoaders);

		jfieldID fieldLoaders = NULL; //env->GetFieldID(clazzApplicationLoaders, "mLoaders", "Ljava/util/Map;");

		if(NULL == clazzHashMap)
		{
			fieldLoaders = env->GetFieldID(clazzApplicationLoaders, "mLoaders", "Ljava/util/Map;");
		}
		else
		{
			fieldLoaders = env->GetFieldID(clazzApplicationLoaders, "mLoaders", "Landroid/util/ArrayMap;");
		}

		CHECK_VALID(fieldLoaders);

		jobject objLoaders = env->GetObjectField(objApplicationLoaders,
				fieldLoaders);
		CHECK_VALID(objLoaders);

		if(NULL == clazzHashMap)
		{
			clazzHashMap = env->GetObjectClass(objLoaders);
		}
		CHECK_VALID(clazzHashMap);
		jmethodID methodValues = env->GetMethodID(clazzHashMap, "values",
				"()Ljava/util/Collection;");
		CHECK_VALID(methodValues);
		jobject values = env->CallObjectMethod(objLoaders, methodValues);


		CHECK_VALID(values);
		jclass clazzValues = env->GetObjectClass(values);
		CHECK_VALID(clazzValues);
		jmethodID methodToArray = env->GetMethodID(clazzValues,
				"toArray", "()[Ljava/lang/Object;");
		jobjectArray classLoaders = (jobjectArray) env->CallObjectMethod(values,
				methodToArray);
		CHECK_VALID(classLoaders);

		int size = env->GetArrayLength(classLoaders);
		int i = 0;
		jstring param = env->NewStringUTF(classDesc);

		for (i = 0; i < size; i++) {
			jobject classLoader = env->GetObjectArrayElement(classLoaders, i);
			jclass clazzCL = env->GetObjectClass(classLoader);
			jmethodID loadClass = env->GetMethodID(clazzCL, "loadClass",
					"(Ljava/lang/String;)Ljava/lang/Class;");
			classObj = (jclass) env->CallObjectMethod(classLoader, loadClass,param);

			if (env->ExceptionCheck() == JNI_TRUE)
			{
				env->ExceptionDescribe();
				env->ExceptionClear();
				continue;
			}
			else
			{
				break;
			}
		}
		if(i == size)
		{
			return NULL;
		}
	}

	return (jclass) env->NewGlobalRef(classObj);
}



