/*
 * MDexFileTypeId.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef MDEXFILETYPEID_H_
#define MDEXFILETYPEID_H_
#include "MDexFile.h"
//item_DexTypeIds
/*
 *	type ->类型值
 */
typedef struct item_DexTypeIds
{
	uint type;
}item_DexTypeIds;
/*
 * LDoffset -> 段起始
 * segshift -> 段偏移，表示距离首个Typelist偏移
 * typeDef -> 保存List数据，可以GetSize获取这个Typelist的size
 */
typedef struct item_DexTypelist{
	uint DexOffset;
	uint FileOffset;
	uint MemSize;
	void* data;
}item_DexTypelist;
//
class MDexFile_TypeId {
public:
	MeVertor<item_DexTypeIds> typeIds;
	MeVertor<item_DexTypelist> typeList;
	MDexFile* pMDex = NULL;

//
	size_t getIdxNoType(int inNo);
	uint getListFileOffset(uint inDexOffset);



//
	void initMemory_IDS(ModMemory* inMemory);
	void initMemory_List(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
//
	MDexFile_TypeId(MDexFile* inMDex);
	void MDexFile_TypeList(uint inOffset,size_t inSize);
};

#endif /* MDEXFILETYPEID_H_ */
