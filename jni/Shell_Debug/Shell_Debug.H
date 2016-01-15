/*
 * Shell_Debug.H
 *
 *  Created on: 2015年11月30日
 *      Author: LsMouse
 */

#ifndef SHELL_DEBUG_SHELL_DEBUG_H_
#define SHELL_DEBUG_SHELL_DEBUG_H_
//定义日志输出
extern "C"
{
	extern bool isTarget(char* inName);
	extern void SetAppName(void* inAppName);
	extern void SetJavaVM(void* inJavaVM);
	extern void StartDump(char* inName);
	extern void* SetBef_LoadNative(char* pathName);
	extern void* SetAft_LoadNative(char* pathName);
}
//

#endif /* SHELL_DEBUG_SHELL_DEBUG_H_ */
