/*
 * Baidu.H
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */

#ifndef ALIOLD_H_
#define ALIOLD_H_
//
#define Ali_mobisec		"mobisec"
//
typedef unsigned char* (*Res_Class)(int pInsns, const void *inBuf, unsigned int inLen, void **newBuf);
typedef unsigned char* (*Res_Des)(void *des, int len);
//
typedef struct DesHeader
{
	unsigned int fileSize_r;
	unsigned int Md5_val_r;
	unsigned int classDataOffset_r;
	unsigned int classDataSize_r;
	unsigned int insntDataOffset_r;
	unsigned int insntDataSize_r;
}DesHeader;
typedef struct DesIdxData
{
	unsigned int InsntOffset;
	unsigned int InsntSize;
	unsigned int ClassIdx;
	unsigned int DebugInfoOff;
	unsigned int field_10;
}DesIdxData;
//
extern "C"{
	extern bool isTarget(char* inName);
	extern void SetAppName(void* inAppName);
	extern void SetJavaVM(void* inJavaVM);
	extern void StartDump(char* inName);
	extern void* SetBef_LoadNative(char* pathName);
	extern void* SetAft_LoadNative(char* pathName);

}
#endif /* ALIOLD_H_ */
