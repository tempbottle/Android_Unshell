/*
 * QH360.H
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */

#ifndef QH360_H_
#define QH360_H_
//定义奇虎360加密库
#define Final_360_Jiagu		"jiagu"
#define Final_360_Jiagu_2	"libprotectClass"

extern "C" {
	extern bool isTarget(char* inName);
	extern void SetAppName(void* inAppName);
	extern void SetJavaVM(void* inJavaVM);
	extern void StartDump(char* inPath);
	extern void* SetAft_LoadNative(char* pathName);
}


#endif /* QH360_H_ */
