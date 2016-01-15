/*
 * MDexFileStringId.cpp
 * 本处理字符串索引和字符串。
 * 1.
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */
#include "MDexFile.h"
/*
 ************************************************************
 *				getStringCodeByOffset
 *	根据偏移地址获取绝对地址
 ************************************************************
 */
void* MDexFile_StringId::getStringCodeByOffset(uint inOffset)
{
	void* mAddr = (char*)pMDex->pBackHeader + inOffset;
	return mAddr;
}
/*
 ************************************************************
 *				getStringByIds
 *	根据索引获取字符串内容
 ************************************************************
 */
char* MDexFile_StringId::getStringByIds(uint inStringIds)
{
	item_Dexstring* mString = stringIds.Get(inStringIds);
	if(NULL != mString)
	{
		void* mStringCode = mString->string;
		DexUtils::readUnsignedLeb128(&mStringCode);
		return mStringCode;
	}
	return NULL;
}
/*
 ************************************************************
 *				getIds
 *	根据字符串获取索引
 ************************************************************
 */
int MDexFile_StringId::getIds(char* inStr)
{
	stringIds.Reset();
	for(int m_i = 0;m_i < stringIds.GetSize();m_i++)
	{
		void* mStringCode = stringIds.Get()->string;
		DexUtils::readSignedLeb128(&mStringCode);
		if(strcmp((char*)mStringCode,inStr) == 0)
		{
			return m_i;
		}
		stringIds.Next();
	}
	return -1;
}
/*
************************************************************
*				initMemory_IDS
*	申请内存，存放索引数据
************************************************************
*/
void MDexFile_StringId::initMemory_IDS(ModMemory* inMemory)
{
	uint mIdsSize = stringIds.GetSize();
	item_Mem* item = inMemory->newNameMem(MemDex_StringIds,mIdsSize*4);
	DEXLOG("MDexFile_StringId Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				initMemory_Code
*	申请内存，存字符串数据
************************************************************
*/
void MDexFile_StringId::initMemory_Code(ModMemory* inMemory)
{
	uint mCodeSize = 0;
	stringIds.Reset();
	for(int m_i = 0;m_i < stringIds.GetSize();m_i++)
	{
		mCodeSize +=stringIds.Get()->stringSize;
		stringIds.Next();
	}
	mCodeSize = (mCodeSize+3)&(~3);
	item_Mem* item = inMemory->newNameMem(MemDex_StringCode,mCodeSize);
	DEXLOG("MemDex_StringCode Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
* 这里直接将StringIds和StringCode数据填充完毕
* 实现数据重排列
************************************************************
*/
void MDexFile_StringId::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_StringId toMemory");
	//获取基本数据
	item_Mem* mem_Code = inMemory->getByName(MemDex_StringCode);
	uint* mAddr_Ids = (uint*)inMemory->getByName(MemDex_StringIds)->Addr;
	uint mString = mem_Code->Offset;
	char* mCodeAddr = mem_Code->Addr;
	//StringCode数据
	stringIds.Reset();
	for(int m_i = 0;m_i < stringIds.GetSize();m_i++)
	{
		stringIds.Get()->FileOffset =(uint)mCodeAddr - (uint)mem_Code->Addr + mem_Code->Offset;
		*mAddr_Ids++= stringIds.Get()->FileOffset;
		memcpy(mCodeAddr,stringIds.Get()->string,stringIds.Get()->stringSize);
		mCodeAddr += stringIds.Get()->stringSize;
		if(stringIds.Get()->stringSize == 1)
		{
			mCodeAddr++;
		}
		stringIds.Next();
	}
}
/*
************************************************************
*			ParseStringCode
*	将索引与内存中的StringCode匹配
*	更新现有字符串数据
************************************************************
*/
void MDexFile_StringId::ParseStringCode()
{
	stringIds.Reset();
	for(int m_i = 0;m_i < stringIds.GetSize();m_i++)
	{
	//	stringIds.Get()->string = getStringCodeByOffset(stringIds.Get()->DexOffset);
		stringIds.Next();
	}
}
/*
************************************************************
*			StringCodeSize
*	注意，程序保存时候并没有对字符串进行解码，只是保存长度和数据，并且长度和数据没有分开保存
*	但是如果Leb128经过特殊处理 程序可能出现问题，不考虑问题
************************************************************
*/
size_t MDexFile_StringId::StringCodeSize()
{
	size_t mSize = 0;
	stringIds.Reset();
	for(int m_i = 0;m_i < stringIds.GetSize();m_i++)
	{
		mSize += strlen(stringIds.Get()->string) +1;
		stringIds.Next();
	}
	return mSize;
}
/*
************************************************************
*			MDexFile_StringId
*	构造函数，根据DexFile分析StringIds
*	这里只是解析Ids然后在后续将才进行string数据读取。
************************************************************
*/
MDexFile_StringId::MDexFile_StringId(MDexFile* inMDex)
{
	pMDex = inMDex;
	//获取偏移地址
	uint* mMemIds =(uint*)MDexFile_Header::GetAddrByStringIds(pMDex->pBackHeader);
	uint mFiroffset = *mMemIds;
	//开始遍历查询
	stringIds.Clear();
	for(int m_i = 0;m_i < inMDex->pBackHeader->stringIdsSize;m_i++)
	{
		item_Dexstring* mItem = new item_Dexstring();
		mItem->DexOffset = *mMemIds;
		mItem->string = (char*)inMDex->pBackHeader + mItem->DexOffset;
		mItem->stringSize = strlen(mItem->string) +1;
		mMemIds++;
		stringIds.Add(mItem);
	}/**/
	DEXLOG("MDexFile_StringId Size:%d",stringIds.GetSize());
}
