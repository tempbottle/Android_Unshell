/*
 * MDexFileEncode.h
 *
 *  Created on: 2015年12月5日
 *      Author: LsMouse
 */

#ifndef MDEXFILEENCODE_H_
#define MDEXFILEENCODE_H_
#include "MDexFile.h"

typedef struct item_EncodeArray{
	uint DexOffset;
	uint FileOffset;
	uint  MemSize;
	void* data;
}item_EncodeArray;

class MDexFile_Encode {
public:
//
	MeVertor<item_EncodeArray> encodeArray;
	MDexFile* pMDex;
//
	uint getFileOffset(uint inDexOffset);
//
	void initMemory(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
//
	MDexFile_Encode(MDexFile* inMDex);
};

#endif /* MDEXFILEENCODE_H_ */
