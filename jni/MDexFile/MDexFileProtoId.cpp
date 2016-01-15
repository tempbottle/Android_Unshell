/*
 * MDexFileProtoId.cpp
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#include "MDexFile.h"
/*
************************************************************
*				initMemory_IDS
*	将数据设置到索引
************************************************************
*/
void MDexFile_ProtoId::initMemory_IDS(ModMemory* inMemory)
{
	uint mIdsSize = protoIds.GetSize();
	item_Mem* item = inMemory->newNameMem(MemDex_ProtoIds,mIdsSize*sizeof(DexProtoId));
	DEXLOG("MDexFile_ProtoId Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				initMemory_IDS
*	申请内存，存放索引数据
************************************************************
*/
void MDexFile_ProtoId::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_ProtoId toMemory");
	DexProtoId* mProtoIds = (DexProtoId*)inMemory->getByName(MemDex_ProtoIds)->Addr;
	protoIds.Reset();
	for(int m_i = 0;m_i< protoIds.GetSize() ;m_i ++)
	{
		mProtoIds->shortyIdx = protoIds.Get()->shortyIdx;
		mProtoIds->returnTypeIdx = protoIds.Get()->returnTypeIdx;
		mProtoIds->parametersOff = pMDEX->pTypeIds->getListFileOffset(protoIds.Get()->parametersOff);
		mProtoIds++;
		protoIds.Next();
	}
}
/*
************************************************************
*				MDexFile_ProtoId
*	解析MDexFile_ProtoId
************************************************************
*/
MDexFile_ProtoId::MDexFile_ProtoId(MDexFile* inMDex)
{
	pMDEX = inMDex;
	DexProtoId* mproto =(DexProtoId*)MDexFile_Header::GetAddrByProtoIds(inMDex->pBackHeader);
	protoIds.Clear();
	for(int m_i = 0;m_i < inMDex->pBackHeader->protoIdsSize;m_i++)
	{
		item_ProtoIds* mItem = new item_ProtoIds();
		mItem->shortyIdx = mproto->shortyIdx;
		mItem->returnTypeIdx = mproto->returnTypeIdx;
		mItem->parametersOff = mproto->parametersOff;
		mproto++;
		protoIds.Add(mItem);
	}
	DEXLOG("MDexFile_ProtoId %d",protoIds.GetSize());
}
