/*
 * MDexFileEncode.cpp
 *
 *  Created on: 2015年12月5日
 *      Author: LsMouse
 */

#include "MDexFile.h"
/*
************************************************************
*				EncodeArray_getValueLen
*	获取当前Type Value长度
************************************************************
*/
static size_t EncodeArray_getValueLen(int inType)
{
	int mTypeClass = inType&kDexAnnotationValueTypeMask;
	int mTypeLen = inType >>kDexAnnotationValueArgShift;
	switch (mTypeClass){
	case kDexAnnotationNull:
	case kDexAnnotationBoolean:
		return 0;
	}
	return mTypeLen+1;
}
uint MDexFile_Encode::getFileOffset(uint inDexOffset)
{
	encodeArray.Reset();
	for(int m_i = 0;m_i < encodeArray.GetSize();m_i++)
	{
		if(inDexOffset == encodeArray.Get()->DexOffset){
			return 	encodeArray.Get()->FileOffset;
		}
		encodeArray.Next();
	}
	return inDexOffset;
}
/*
************************************************************
*				initMemory_IDS
*	申请内存，存放索引数据
************************************************************
*/
void MDexFile_Encode::initMemory(ModMemory* inMemory)
{
	size_t mMemSize = 0;
	encodeArray.Reset();
	for(int m_i = 0;m_i < encodeArray.GetSize();m_i++)
	{
		mMemSize += encodeArray.Get()->MemSize;
		encodeArray.Next();
	}
	mMemSize = DexUtils::AlignAdd(mMemSize,4);
	item_Mem* item = inMemory->newNameMem(MemDex_EncodeArray,mMemSize);
	DEXLOG("MemDex_EncodeArray Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
*
************************************************************
*/
void MDexFile_Encode::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_Encode toMemory");
	item_Mem* mem_Encode =inMemory->getByName(MemDex_EncodeArray);
	char* mAddr_Encode = mem_Encode->Addr;
	encodeArray.Reset();
	for(int m_i = 0;m_i < encodeArray.GetSize();m_i++)
	{
		encodeArray.Get()->FileOffset = mAddr_Encode - (uint) mem_Encode->Addr +  mem_Encode->Offset;
		memcpy(mAddr_Encode,encodeArray.Get()->data,encodeArray.Get()->MemSize);
		mAddr_Encode += encodeArray.Get()->MemSize;
		encodeArray.Next();
	}
}
/*
************************************************************
*				MDexFile_Encode
*	解析EncodeArray
************************************************************
*/
MDexFile_Encode::MDexFile_Encode(MDexFile* inMDex)
{
	pMDex = inMDex;
	uint mOffset = pMDex->pMap->getOffset(kDexTypeEncodedArrayItem);
	uint mSize = pMDex->pMap->getSize(kDexTypeEncodedArrayItem);
	DEXLOG("MDexFile_Encode mOffset:%p,mSize:%d",mOffset,mSize);
	encodeArray.Clear();
	u_char* mAddr_Encode =(u_char*)AddAddr2(pMDex->pBackHeader,mOffset);
	for(int m_i=0;m_i<mSize;m_i++)
	{
		item_EncodeArray* mItem = new item_EncodeArray();
		mItem->DexOffset =(uint)mAddr_Encode - (uint)pMDex->pBackHeader;
		int m_Itemsize = DexUtils::readUnsignedLeb128((void**)&mAddr_Encode);
		for(int m_j = 0;m_j < m_Itemsize;m_j++)
		{
			//跳过Type
			int mType = *mAddr_Encode++;
			int mValueLen = EncodeArray_getValueLen(mType);
			mAddr_Encode += mValueLen;
		}
		mItem->MemSize = (uint)mAddr_Encode - (uint)pMDex->pBackHeader - mItem->DexOffset;
		mItem->data = malloc(mItem->MemSize);
		memcpy(mItem->data,(char*)pMDex->pBackHeader+mItem->DexOffset,mItem->MemSize);
		encodeArray.Add(mItem);
	}

}
