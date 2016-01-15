/*
 * MDexFileTypeId.cpp
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#include "MDexFile.h"
/*
 ************************************************************
 *				getIdxNoType
 *
 ************************************************************
 */
size_t MDexFile_TypeId::getIdxNoType(int inNo) {
	item_DexTypeIds* item = typeIds.Get(inNo);
	if (item == NULL)
		return NULL;
	return item->type;
}
/*
 ************************************************************
 *				getListFileOffset
 *	根据DexOffset获取Fileoffset
 ************************************************************
 */
uint MDexFile_TypeId::getListFileOffset(uint inDexOffset) {
	typeList.Reset();
	for (int m_i = 0; m_i < typeList.GetSize(); m_i++) {
		if (typeList.Get()->DexOffset == inDexOffset) {
			return typeList.Get()->FileOffset;
		}
		typeList.Next();
	}
	return inDexOffset;
}
/*
 ************************************************************
 *				initMemory_IDS
 *	申请内存到inMemory模块
 ************************************************************
 */
void MDexFile_TypeId::initMemory_IDS(ModMemory* inMemory) {
	uint mIdsSize = typeIds.GetSize();
	item_Mem* item = inMemory->newNameMem(MemDex_TypeIds, mIdsSize * 4);
	DEXLOG("MDexFile_TypeId Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
 ************************************************************
 *				initMemory_IDS
 *	申请内存到inMemory模块
 ************************************************************
 */
void MDexFile_TypeId::initMemory_List(ModMemory* inMemory) {
	uint mCodeSize = 0;
	typeList.Reset();
	for (int m_i = 0; m_i < typeList.GetSize(); m_i++) {
		mCodeSize += typeList.Get()->MemSize;
		typeList.Next();
	}
	item_Mem* item = inMemory->newNameMem(MemDex_TypeList, mCodeSize);
	DEXLOG("MemDex_TypeList Offset :%p,Length :%p", item->Offset, item->Length);
}
/*
 ************************************************************
 *				toMemory
 *	将数据输出到ModMemory中
 ************************************************************
 */
void MDexFile_TypeId::toMemory(ModMemory* inMemory) {
	DEXLOG("MDexFile_TypeId toMemory");
	//获取基本数据
	item_Mem* memIds = inMemory->getByName(MemDex_TypeIds);
	uint* mAddr_Ids = (uint*) memIds->Addr;
	//遍历填充TypeIds
	size_t mSize = typeIds.GetSize();
	typeIds.Reset();
	for (int m_i = 0; m_i < mSize; m_i++) {
		*mAddr_Ids++ = typeIds.Get()->type;
		typeIds.Next();
	}
	//遍历填充TypeList
	item_Mem* memList = inMemory->getByName(MemDex_TypeList);
	uint* mAddr_List = (uint*) memList->Addr;
	typeList.Reset();
	mSize = typeList.GetSize();
	DEXLOG("memList->Offset :%p", memList->Offset);
	for (int m_i = 0; m_i < mSize; m_i++) {
		typeList.Get()->FileOffset = (uint) mAddr_List - (uint) memList->Addr
				+ memList->Offset;
		memcpy(mAddr_List, typeList.Get()->data, typeList.Get()->MemSize);
		mAddr_List = (uint*) ((uint) mAddr_List + typeList.Get()->MemSize);
		typeList.Next();
	}
}
/*
 ************************************************************
 *				MDexFile_TypeList
 *	解析MDexFile_TypeList
 *	inAddr -> TypeList内存偏移地址
 *	inSize -> TypeList个数*
 ************************************************************
 */
void MDexFile_TypeId::MDexFile_TypeList(uint inOffset, size_t inSize) {
	DEXLOG("MDexFile_TypeList inOffset:%p,inSize:%d", inOffset, inSize);
	uint* mAddr_List = (uint*) ((char*) pMDex->pBackHeader + inOffset);
	typeList.Clear();
	for (int m_i = 0; m_i < inSize; m_i++) {
		item_DexTypelist* mItem = new item_DexTypelist();
		mItem->DexOffset = (uint) mAddr_List - (uint) pMDex->pBackHeader;
		int mSize_List = *mAddr_List++;
		mAddr_List += (mSize_List + 1) / 2;
		mItem->MemSize = (uint) mAddr_List - (uint) pMDex->pBackHeader
				- mItem->DexOffset;
		mItem->data = malloc(mItem->MemSize);
		memcpy(mItem->data,
				(char*) ((uint) pMDex->pBackHeader + mItem->DexOffset),
				mItem->MemSize);
		typeList.Add(mItem);
	}
}
/*
 ************************************************************
 *			MDexFile_TypeId
 *	构造函数，根据DexFile分析TypeId
 *
 *
 ************************************************************
 */
MDexFile_TypeId::MDexFile_TypeId(MDexFile* inMDex) {
	pMDex = inMDex;
	uint* mMemIds = (uint*) MDexFile_Header::GetAddrByTypeIds(
			inMDex->pBackHeader);
	//遍历typeIds
	typeIds.Clear();
	for (int m_i = 0; m_i < inMDex->pBackHeader->typeIdsSize; m_i++) {
		item_DexTypeIds* mItem = new item_DexTypeIds();
		mItem->type = *mMemIds;
		mMemIds++;
		typeIds.Add(mItem);
	}
	DEXLOG("MDexFile_TypeId Size:%d", typeIds.GetSize());
}
