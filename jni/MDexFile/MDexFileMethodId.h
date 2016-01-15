/*
 * MDexFileMethodId.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef MDEXFILEMETHODID_H_
#define MDEXFILEMETHODID_H_
#include "MDexFile.h"


typedef struct item_DexMethodIds{
	u4  classIdx;           /* index into typeIds list for defining class */
	u4  protoIdx;           /* index into protoIds for method prototype */
	u4  nameIdx;            /* index into stringIds for method name */
}item_DexMethodIds;

class MDexFile_MethodId {
public:
	MeVertor<item_DexMethodIds>  methodIds;
	MDexFile* pMDex;
	//
	void initMemory_IDS(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
	//
	MDexFile_MethodId(MDexFile* inMDex);
};

#endif /* MDEXFILEMETHODID_H_ */
