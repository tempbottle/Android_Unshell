/*
 * MDexFileMap.h
 *
 *  Created on: 2015年12月5日
 *      Author: LsMouse
 */

#ifndef MDEXFILEMAP_H_
#define MDEXFILEMAP_H_
#include "MDexFile.h"
typedef struct item_Map
{
	u4	type;	 /*Section type*/
	u4	unused;	/*unused*/
	u4	size;	/* section size*/
	u4	offset;	/* section offset */
}item_Map;
class MDexFile_Map {
public:
	MeVertor<item_Map> map;
	MDexFile* pMDex = NULL;
	//
	item_Map* getMap(uint inType);
	uint getSize(uint inType);
	uint getOffset(uint inType);
	//
	void initMemory_IDS(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
	//
	MDexFile_Map(MDexFile* inMDex);
};

#endif /* MDEXFILEMAP_H_ */
