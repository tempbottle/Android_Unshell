/*
 * FindClass.H
 *
 *  Created on: 2015年12月11日
 *      Author: LsMouse
 */

#ifndef FINDCLASS_H_
#define FINDCLASS_H_
#define	LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "LsShell", __VA_ARGS__))
#define	LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "LsShell", __VA_ARGS__))
//
#define CHECK_VALID(V) 				\
	if(V == NULL){					\
		LOGE("%s is null.", #V);	\
		exit(-1);					\
	}else{							\
		LOGI("%s is %p.", #V, V);	\
	}
//
extern int ClearException(JNIEnv *jenv);
extern jclass findAppClass(JNIEnv *jenv,const char *apn);



#endif /* FINDCLASS_H_ */
