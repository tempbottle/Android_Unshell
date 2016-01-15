/*
 * NDKUtils.cpp
 *
 *  Created on: 2015年12月8日
 *      Author: LsMouse
 */

#include "NDKUtils.h"
/**
 *
 */
static jclass findAppClass(JNIEnv *jenv,const char *apn){
    jclass clazzApplicationLoaders = jenv->FindClass("android/app/ApplicationLoaders");
    jthrowable exception = jenv->ExceptionOccurred();
    if (ClearException(jenv)) {
        ALOG("Exception","No class : %s", "android/app/ApplicationLoaders");
        return NULL;
    }
    jfieldID fieldApplicationLoaders = jenv->GetStaticFieldID(clazzApplicationLoaders,"gApplicationLoaders","Landroid/app/ApplicationLoaders;");
    if (ClearException(jenv)) {
        ALOG("Exception","No Static Field :%s","gApplicationLoaders");
        return NULL;
    }
    jobject objApplicationLoaders = jenv->GetStaticObjectField(clazzApplicationLoaders,fieldApplicationLoaders);
    if (ClearException(jenv)) {
        ALOG("Exception","GetStaticObjectField is failed [%s","gApplicationLoaders");
        return NULL;
    }
    jfieldID fieldLoaders = jenv->GetFieldID(clazzApplicationLoaders,"mLoaders","Ljava/util/Map;");
    if (ClearException(jenv)) {
        ALOG("Exception","No Field :%s","mLoaders");
        return NULL;
    }
    jobject objLoaders = jenv->GetObjectField(objApplicationLoaders,fieldLoaders);
    if (ClearException(jenv)) {
        ALOG("Exception","No object :%s","mLoaders");
        return NULL;
    }
    jclass clazzHashMap = jenv->GetObjectClass(objLoaders);
    jmethodID methodValues = jenv->GetMethodID(clazzHashMap,"values","()Ljava/util/Collection;");
    jobject values = jenv->CallObjectMethod(objLoaders,methodValues);

    jclass clazzValues = jenv->GetObjectClass(values);
    jmethodID methodToArray = jenv->GetMethodID(clazzValues,"toArray","()[Ljava/lang/Object;");
    if (ClearException(jenv)) {
        ALOG("Exception","No Method:%s","toArray");
        return NULL;
    }

    jobjectArray classLoaders = (jobjectArray)jenv->CallObjectMethod(values,methodToArray);
    if (ClearException(jenv)) {
        ALOG("Exception","CallObjectMethod failed :%s","toArray");
        return NULL;
    }

        int size = jenv->GetArrayLength(classLoaders);

        for(int i = 0 ; i < size ; i ++){
            jobject classLoader = jenv->GetObjectArrayElement(classLoaders,i);
            jclass clazzCL = jenv->GetObjectClass(classLoader);
            jmethodID loadClass = jenv->GetMethodID(clazzCL,"loadClass","(Ljava/lang/String;)Ljava/lang/Class;");
            jstring param = jenv->NewStringUTF(apn);
            jclass tClazz = (jclass)jenv->CallObjectMethod(classLoader,loadClass,param);
            if (ClearException(jenv)) {
                ALOG("Exception","No");
                continue;
            }
            return tClazz;
        }
    ALOG("Exception","No");
    return NULL;
}



NDK_Utils::NDK_Utils() {
	// TODO Auto-generated constructor stub

}

NDK_Utils::~NDK_Utils() {
	// TODO Auto-generated destructor stub
}

