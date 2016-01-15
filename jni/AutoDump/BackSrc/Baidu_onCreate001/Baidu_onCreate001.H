/*
 * Baidu.H
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */

#ifndef BAIDU_H_
#define BAIDU_H_
//
#define	Baidu_protect		"libbaiduprotect.so"
//
extern "C"{
	extern bool isTarget(char* inName);
	extern void SetAppName(void* inAppName);
	extern void SetJavaVM(void* inJavaVM);
	extern void StartDump(char* inName);
	extern void RestoreCode(JNIEnv* env,char* pstr);
	extern void UnShell(char* inAddr,size_t length);
}



#endif /* BAIDU_H_ */
