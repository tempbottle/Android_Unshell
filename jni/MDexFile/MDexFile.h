/*
 * MDexFile.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */
#ifndef MDEXFILE_H_
#define MDEXFILE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "Utils.H"
#include "DexUtils.h"
#include "Dex_SystemDefine.H"
//
#include "MDexFileOPT.h"
#include "MDexFileMap.h"
#include "MDexFileHeader.h"
#include "MDexFileStringId.h"
#include "MDexFileTypeId.h"
#include "MDexFileFieldId.h"
#include "MDexFileMethodId.h"
#include "MDexFileProtoId.h"
#include "MDexFileClassDef.h"
#include "MDexFileEncode.h"
#include "MDexFileAnnot.h"
#include "MDexFileDebug.h"


class MDexFile {
public:
	DexHeader* pBackHeader;
	void* LoadAddr;
	bool isErr;
	size_t Length;
	ModMemory* Dexmemory;
//	定义模块基本结构
	MDexFile_OPT*		pOptHeader;
	MDexFile_Header*	pHeader;
	MDexFile_StringId* 	pStringIds;
	MDexFile_TypeId* 	pTypeIds;
	MDexFile_FieldId*	pFieldIds;
	MDexFile_MethodId*	pMethodIds;
	MDexFile_ProtoId*	pProtoIds;
	MDexFile_ClassDef*  pClassDefs;
//
	MDexFile_Map*		pMap;
	MDexFile_Encode*	pEncode;
	MDexFile_Annot*		pAnnot;
//
	void LoadAllClass();
	void Parse();
//
	char* getClassDescriptor(int inNo);
//
	size_t getClassDefSize();
//
	void realignment();
	void Save_BySVC(char* appName,char* inFile);
	void SaveDex(char* appName,char* inFile);
	MDexFile(void* pDexFile,size_t inLength);
	MDexFile();
};

#endif /* MDEXFILE_H_ */
