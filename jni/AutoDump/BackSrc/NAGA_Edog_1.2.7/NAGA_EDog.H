/*
 * NAGA_EDog.H
 *
 *  Created on: 2015年12月11日
 *      Author: LsMouse
 */

#ifndef NAGA_H_
#define NAGA_H_
//
#define LIB_NAGEEDOG	"libedog.so"

extern "C"
{
extern bool isTarget(char* inName);
extern void StartDump(char* inPath);
//
extern void SetAppName(void* inAppName);
extern void SetJavaVM(void* inJavaVM);
}
#endif


