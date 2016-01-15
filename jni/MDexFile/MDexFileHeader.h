/*
 * MDexFileHeader.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef MDEXFILEHEADER_H_
#define MDEXFILEHEADER_H_
#include "MDexFile.H"
//
class MDexFile_Header {
public:
	MDexFile* pMDex = NULL;
	DexHeader* pHeader = NULL;
	DexHeader* pBackHeader = NULL;
	//
	static void* GetAddrByHeader(void* inHeader);
	static void* GetAddrByStringIds(void* inHeader);
	static void* GetAddrByTypeIds(void* inHeader);
	static void* GetAddrByFieldIds(void* inHeader);
	static void* GetAddrByProtoIds(void* inHeader);
	static void* GetAddrByMethodIds(void* inHeader);
	static void* GetAddrByClassDef(void* inHeader);
	static void* GetAddrByData(void* inHeader);
	static void* GetAddrByMap(void* inHeader);
	//
	void Printf(void* inHeader);
	void Printf();
	//
	void initMemory_IDS(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
	//
	MDexFile_Header(MDexFile* inMDex);
};

#endif /* MDEXFILEHEADER_H_ */
