/*
 * Baidu.H
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */

#ifndef BAIDU_H_
#define BAIDU_H_
//
#define PayEgis_protect		"libegis.so"
#define PayEgisboot_protect	"libegisboot.so"
//设置保存数据
typedef struct BackClass{
	int id;
	int flags;
	int offset;
	int nop;
}BackClass;
//
extern "C"{
	extern bool isTarget(char* inName);
	extern void SetAppName(void* inAppName);
	extern void SetJavaVM(void* inJavaVM);
	extern void StartDump(char* inName);
	extern void RestoreCode(JNIEnv* env,char* pstr);
	extern void UnShell(JNIEnv* env,jobject obj);
//	extern void UnShell(char* inAddr,size_t length);
}
#endif /* BAIDU_H_ */
