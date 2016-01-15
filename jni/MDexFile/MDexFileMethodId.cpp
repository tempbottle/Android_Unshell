/*
 * MDexFileMethodId.cpp
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#include "MDexFile.h"
/*
************************************************************
*				initMemory_IDS
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_MethodId::initMemory_IDS(ModMemory* inMemory)
{
	uint mSize = methodIds.GetSize();
	item_Mem* item = inMemory->newNameMem(MemDex_MethoIds,mSize*sizeof(DexMethodId));
	DEXLOG("MDexFile_MethodId Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
*	将数据输出到ModMemory中
************************************************************
*/
void MDexFile_MethodId::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_MethodId toMemory");
	DexMethodId* mIdsAddr = (DexMethodId*)inMemory->getByName(MemDex_MethoIds)->Addr;
	methodIds.Reset();
	for(int m_i = 0;m_i < methodIds.GetSize();m_i++)
	{
		mIdsAddr->classIdx = methodIds.Get()->classIdx;
		mIdsAddr->protoIdx = methodIds.Get()->protoIdx;
		mIdsAddr->nameIdx = methodIds.Get()->nameIdx;
		mIdsAddr++;
		methodIds.Next();
	}
}
/*
************************************************************
*			MDexFile_MethodId
*	构造函数，根据DexFile分析MethodId
************************************************************
*/
MDexFile_MethodId::MDexFile_MethodId(MDexFile* inMDex)
{
	pMDex = inMDex;
	DexMethodId* mMethodIds =(DexMethodId*)MDexFile_Header::GetAddrByMethodIds(inMDex->pBackHeader);
	methodIds.Clear();
	for(int m_i = 0;m_i < inMDex->pBackHeader->methodIdsSize;m_i++)
	{
		item_DexMethodIds* mItem = new item_DexMethodIds();
		mItem->classIdx = mMethodIds->classIdx;
		mItem->protoIdx = mMethodIds->protoIdx;
		mItem->nameIdx = mMethodIds->nameIdx;
		mMethodIds++;
		methodIds.Add(mItem);
	}
	DEXLOG("MDexFile_MethodId %d",methodIds.GetSize());
}
