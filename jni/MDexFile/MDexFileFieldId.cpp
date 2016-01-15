/*
 * MDexFileFieldId.cpp
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#include "MDexFile.h"
/*
************************************************************
*				initMemory_IDS
*	申请内存，存放索引数据
************************************************************
*/
void MDexFile_FieldId::initMemory_IDS(ModMemory* inMemory)
{
	uint mIdsSize = fieldIds.GetSize();
	item_Mem* item = inMemory->newNameMem(MemDex_FieldIds,mIdsSize*sizeof(DexFieldId));
	DEXLOG("MemDex_FieldIds Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
*	将数据输出到ModMemory中
************************************************************
*/
void MDexFile_FieldId::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_FieldId toMemory");
	DexFieldId* mAddr_Ids = (DexFieldId*)inMemory->getByName(MemDex_FieldIds)->Addr;
	fieldIds.Reset();
	for(int m_i = 0;m_i < fieldIds.GetSize();m_i++)
	{
		mAddr_Ids->classIdx = fieldIds.Get()->classIdx;
		mAddr_Ids->typeIdx = fieldIds.Get()->typeIdx;
		mAddr_Ids->nameIdx = fieldIds.Get()->nameIdx;
		mAddr_Ids++;
		fieldIds.Next();
	}
}
/*
************************************************************
*			MDexFile_FieldId
*	构造函数，根据DexFile分析FieldId
************************************************************
*/
MDexFile_FieldId::MDexFile_FieldId(MDexFile* inMDex)
{
	pMDex = inMDex;
	DexFieldId* mfield =(DexFieldId*)MDexFile_Header::GetAddrByFieldIds(inMDex->pBackHeader);
	fieldIds.Clear();
	for(int m_i = 0;m_i < inMDex->pBackHeader->fieldIdsSize;m_i++)
	{
		item_FieldIds* mItem = new item_FieldIds();
		mItem->classIdx = mfield->classIdx;
		mItem->typeIdx = mfield->typeIdx;
		mItem->nameIdx = mfield->nameIdx;
		mfield++;
		fieldIds.Add(mItem);
	}
	DEXLOG("MDexFile_FieldId Size:%d",fieldIds.GetSize());
}
