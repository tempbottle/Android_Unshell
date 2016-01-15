/*
 * MDexFileClassDef.cpp
 *
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */
#include "MDexFile.h"
/**/
size_t MDexFile_ClassDef::getCodeLength(uint inCodeOff)
{
	DexClassCode* mCode = (DexClassCode*)(inCodeOff +(uint)pMDex->pBackHeader);
	uint mSize= DexUtils::AlignAdd(mCode->code_size*2 + 0x10,4);
	uint m_Try = mCode->try_size;
	if(m_Try != 0)
	{
		//直接跳过固定长度的 Tries info，定位到Catch handlers info
		mSize += 0x08*m_Try;
		void* mCatch = (void*)((uint)mCode +  mSize);
		//获取到CatchBlocks
		uint mCatchBlocks = DexUtils::readUnsignedLeb128(&mCatch);
		for(int m_i = 0;m_i < mCatchBlocks;m_i++)
		{
			uint mSize_Handler = DexUtils::readUnsignedLeb128(&mCatch);
			if(mSize_Handler == 0)
			{
				DexUtils::readUnsignedLeb128(&mCatch);
			}else if(mSize_Handler > 0x40){
				int m_Handler = (0 - mSize_Handler)&0x7F;
				DexUtils::readUnsignedLeb128(&mCatch);
				for(int m_j = 0;m_j < m_Handler; m_j++)
				{
					DexUtils::readUnsignedLeb128(&mCatch);
					DexUtils::readUnsignedLeb128(&mCatch);
				}
			}else if(mSize_Handler < 0x40){
				for(int m_j = 0;m_j < mSize_Handler; m_j++)
				{
					DexUtils::readUnsignedLeb128(&mCatch);
					DexUtils::readUnsignedLeb128(&mCatch);
				}
			}
		}
		mSize = DexUtils::Align_4((uint)mCatch - (uint)mCode);
	}
	return mSize;
}
/**/
item_ClassCode* MDexFile_ClassDef::initItem_ClassCode(uint inIdx,uint inCodeOff)
{
	DexClassCode* mCode = (DexClassCode*)(inCodeOff + (uint)pMDex->pBackHeader);
	item_ClassCode* mClassCode = new item_ClassCode();
	mClassCode->method_ids = inIdx;
	mClassCode->DexOffset = inCodeOff;
	mClassCode->MemSize = getCodeLength(inCodeOff);
	mClassCode->data = malloc(mClassCode->MemSize);
	memcpy(mClassCode->data,mCode,mClassCode->MemSize);
	mCode =  (DexClassCode*)mClassCode->data;
	//mCode->debug_info = 0;
	return mClassCode;
}
/*
************************************************************
*				Clear_DebugInfo
* 清楚DebugInfo
************************************************************
*/
void MDexFile_ClassDef::Clear_DebugInfo()
{
	classCode.Reset();
	for(int m_i = 0;m_i < classCode.Get();m_i++)
	{
		item_ClassCode* mItem = classCode.Get();
		DexClassCode* mDexCode =mItem->data;
		mDexCode->debug_info = 0;
		classCode.Next();
	}

}
/*
************************************************************
*				initMemory_IDS
************************************************************
*/
uint MDexFile_ClassDef::getCodeFileOffset(uint inDexOffset)
{
	if(inDexOffset == 0)
		return 0;
	if(classCode.Get() == NULL)
		classCode.Reset();
	item_ClassCode* mthiz = classCode.Get();
	do{
		if(classCode.Get()->DexOffset == inDexOffset)
		{
			return classCode.Get()->FileOffset;
		}
		classCode.Next();
		if(classCode.Get() == NULL)
			classCode.Reset();
	}while(mthiz != classCode.Get());
	return inDexOffset;
}
uint MDexFile_ClassDef::getDataFileOffset(uint inDexOffset)
{
	if(inDexOffset == 0)
		return 0;
	classData.Reset();
	for(int m_i = 0;m_i < classData.GetSize();m_i++)
	{
		if(inDexOffset == classData.Get()->DexOffset)
		{
			return classData.Get()->FileOffset;
		}
		classData.Next();
	}
	return inDexOffset;
}

item_ClassMethod* MDexFile_ClassDef::getClassMethod(uint inMethodIdx)
{
	classData.Reset();
	for(int m_i = 0; m_i < classData.GetSize();m_i++)
	{
		MeVertor<item_ClassMethod> mMehtod = classData.Get()->Direct;
		mMehtod.Reset();
		for(int m_i = 0; m_i < mMehtod.GetSize();m_i++)
		{
			if(mMehtod.Get()->method_ids == inMethodIdx)
			{
				return mMehtod.Get();
			}
			mMehtod.Next();
		}
		mMehtod = classData.Get()->Virtual;
		mMehtod.Reset();
		for(int m_i = 0; m_i < mMehtod.GetSize();m_i++)
		{
			if(mMehtod.Get()->method_ids == inMethodIdx)
			{
				return mMehtod.Get();
			}
			mMehtod.Next();
		}
		classData.Next();
	}
}

/*
************************************************************
*				initMemory_IDS
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_ClassDef::initMemory_IDS(ModMemory* inMemory)
{
	uint mSize = classDef.GetSize() + 1;
	item_Mem* item = inMemory->newNameMem(MemDex_ClassDef,mSize*sizeof(struct DexClassDef));
	DEXLOG("MDexFile_ClassDef Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				initMemory_Data
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_ClassDef::initMemory_Data(ModMemory* inMemory)
{
	uint mSize = 0;
	classData.Reset();
	for(int m_i = 0;m_i < classData.GetSize();m_i++)
	{
		mSize += classData.Get()->MemSize;
		classData.Next();
	}
	//多分配内存，以防leb128修改地址导致数据溢出
	item_Mem* item = inMemory->newNameMem(MemDex_ClassData,DexUtils::AlignAdd(mSize+0x1000,4));
	DEXLOG("MemDex_ClassData Offset :%p,Length :%p",item->Offset,item->Length);
}


/*
************************************************************
*				initMemory_Code
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_ClassDef::initMemory_Code(ModMemory* inMemory)
{
	uint mSize = 0;
	classCode.Reset();
	for(int m_i = 0;m_i < classCode.GetSize();m_i++)
	{
		mSize += classCode.Get()->MemSize;
		classCode.Next();
	}
	item_Mem* item = inMemory->newNameMem(MemDex_ClassCode,DexUtils::AlignAdd(mSize+0x1000,4));
	DEXLOG("MemDex_ClassCode Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
************************************************************
*/
void MDexFile_ClassDef::toMemory(ModMemory* inMemory)
{

	//Code
	DEXLOG("toMemory classCode size: %p ",classCode.GetSize());
	item_Mem* mem_Code = inMemory->getByName(MemDex_ClassCode);
	char* mAddr_Code = (char*)mem_Code->Addr;
	classCode.Reset();
	for(int m_i = 0;m_i < classCode.GetSize();m_i++)
	{
		classCode.Get()->FileOffset = (uint)mAddr_Code - (uint)mem_Code->Addr + mem_Code->Offset;
		memcpy(mAddr_Code,classCode.Get()->data,classCode.Get()->MemSize);
		mAddr_Code += classCode.Get()->MemSize;
		classCode.Next();
	}
	//Data
	DEXLOG("toMemory classData size: %p ",classData.GetSize());
	item_Mem* mem_Data = inMemory->getByName(MemDex_ClassData);
	char* mAddr_Data = (char*)mem_Data->Addr;
	classData.Reset();
	for(int m_i = 0;m_i < classData.GetSize();m_i++)
	{
		item_ClassData* mitem = classData.Get();
		mitem->FileOffset = (uint)mAddr_Data - (uint)mem_Data->Addr + mem_Data->Offset;
		mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Static.GetSize());
		mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Instance.GetSize());
		mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Direct.GetSize());
		mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Virtual.GetSize());
		mitem->Static.Reset();
		for(int m_i = 0;m_i < mitem->Static.GetSize();m_i++)
		{
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Static.Get()->ids);
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Static.Get()->flags);
			mitem->Static.Next();
		}
		mitem->Instance.Reset();
		for(int m_i = 0;m_i < mitem->Instance.GetSize();m_i++)
		{
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Instance.Get()->ids);
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Instance.Get()->flags);
			mitem->Instance.Next();
		}
		mitem->Direct.Reset();
		for(int m_i = 0;m_i < mitem->Direct.GetSize();m_i++)
		{
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Direct.Get()->ids);
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Direct.Get()->flags);
			if(mitem->Direct.Get()->Offset != 0)
			{
				uint mOffset = getCodeFileOffset(mitem->Direct.Get()->Offset);
				mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mOffset);
			}else{
				mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,0);
			}
			mitem->Direct.Next();
		}
		mitem->Virtual.Reset();
		for(int m_i = 0;m_i < mitem->Virtual.GetSize();m_i++)
		{
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Virtual.Get()->ids);
			mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mitem->Virtual.Get()->flags);
			if(mitem->Virtual.Get()->Offset != 0)
			{
				uint mOffset = getCodeFileOffset(mitem->Virtual.Get()->Offset);
				mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,mOffset);
			}else{
				mAddr_Data = DexUtils::writeUnsignedLeb128(mAddr_Data,0);
			}
			mitem->Virtual.Next();
		}
		classData.Next();
	}
	//ClassDef
	DEXLOG("toMemory classDef size: %p ",classDef.GetSize());
	item_Mem* mem_Def = inMemory->getByName(MemDex_ClassDef);
	DexClassDef* mAddr_Def = (DexClassDef*)mem_Def->Addr;
	classDef.Reset();
	for(int m_i = 0;m_i < classDef.GetSize();m_i++)
	{
		mAddr_Def->classIdx = classDef.Get()->classIdx;
		mAddr_Def->accessFlags = classDef.Get()->accessFlags;
		mAddr_Def->superclassIdx = classDef.Get()->superclassIdx;
		mAddr_Def->sourceFileIdx = classDef.Get()->sourceFileIdx;
		if(classDef.Get()->interfacesOff == 0)
		{
			mAddr_Def->interfacesOff = 0;
		}else{
			mAddr_Def->interfacesOff = pMDex->pTypeIds->getListFileOffset(classDef.Get()->interfacesOff);
		}
		if(classDef.Get()->annotationsOff == 0)
		{
			mAddr_Def->annotationsOff = 0;
		}else{
			mAddr_Def->annotationsOff = pMDex->pAnnot->getAnnotDIRFileOffset(classDef.Get()->annotationsOff);
		}
		if(classDef.Get()->classDataOff == 0)
		{
			mAddr_Def->classDataOff = 0;
		}else{
			mAddr_Def->classDataOff = getDataFileOffset(classDef.Get()->classDataOff);
		}
		if(classDef.Get()->staticValuesOff == 0){
			mAddr_Def->staticValuesOff = 0;
		}else{
			mAddr_Def->staticValuesOff = pMDex->pEncode->getFileOffset(classDef.Get()->staticValuesOff);
		}
		mAddr_Def++;
		classDef.Next();
	}
	DEXLOG("toMemory classDef OVER");
}
/*
************************************************************
*			Reset_ClassCode
* 根据classData里面数据重新刷新ClassCode内容
*
************************************************************
*/
void MDexFile_ClassDef::Reset_ClassCode()
{
	classCode.Clear();
	classData.Reset();
	for(int m_i = 0;m_i < classData.GetSize();m_i++)
	{
		MeVertor<item_ClassMethod> mMehtod = classData.Get()->Direct;
		mMehtod.Reset();
		for(int m_i = 0; m_i < mMehtod.GetSize();m_i++)
		{
			if(mMehtod.Get()->Offset != 0)
			{
				item_ClassCode* code = initItem_ClassCode(mMehtod.Get()->method_ids,mMehtod.Get()->Offset);
				classCode.Add(code);
			}
			mMehtod.Next();
		}
		mMehtod = classData.Get()->Virtual;
		mMehtod.Reset();
		for(int m_i = 0; m_i < mMehtod.GetSize();m_i++)
		{
			if(mMehtod.Get()->Offset != 0)
			{
				item_ClassCode* code = initItem_ClassCode(mMehtod.Get()->method_ids,mMehtod.Get()->Offset);
				classCode.Add(code);
			}
			mMehtod.Next();
		}
		classData.Next();
	}
}
/*
************************************************************
*			initArray_ClassData
* 初始化ClassData列表
* 自动判断输入是偏移地址还是内存地址
* 初始化时会自动保存code,若后续代码修复，需要更新code
*
************************************************************
*/
item_ClassData* MDexFile_ClassDef::initArray_ClassData(uint inAddr)
{
	//根据输入地址判断是否为偏移地址
	void* mData = (void*)((uint)inAddr+(uint)pMDex->pBackHeader);
	//
	item_ClassData* mItem = new item_ClassData();
	mItem->DexOffset = (uint)inAddr;
	uint mStaticSize = DexUtils::readUnsignedLeb128(&mData);
	uint mInstanceSize = DexUtils::readUnsignedLeb128(&mData);
	uint mDirectSize = DexUtils::readUnsignedLeb128(&mData);
	uint mVirtualSize = DexUtils::readUnsignedLeb128(&mData);
	mItem->Static.Clear();
	mItem->Instance.Clear();
	mItem->Direct.Clear();
	mItem->Virtual.Clear();
	for(int m_i = 0;m_i < mStaticSize;m_i++)
	{
		item_ClassField* mField = new item_ClassField();
		mField->ids = DexUtils::readUnsignedLeb128(&mData);
		mField->flags = DexUtils::readUnsignedLeb128(&mData);
		mItem->Static.Add(mField);
	}
	for(int m_i = 0;m_i < mInstanceSize;m_i++)
	{
		item_ClassField* mField = new item_ClassField();
		mField->ids = DexUtils::readUnsignedLeb128(&mData);
		mField->flags = DexUtils::readUnsignedLeb128(&mData);
		mItem->Instance.Add(mField);
	}
	uint m_ids = 0;
	for(int m_i = 0;m_i < mDirectSize;m_i++)
	{
		item_ClassMethod* mMethod= new item_ClassMethod();
		mMethod->ids = DexUtils::readUnsignedLeb128(&mData);
		mMethod->flags = DexUtils::readUnsignedLeb128(&mData);
		mMethod->Offset = DexUtils::readUnsignedLeb128(&mData);
		m_ids += mMethod->ids;
		mMethod->method_ids = m_ids;
		mItem->Direct.Add(mMethod);
		if(0 != mMethod->Offset)
		{
			item_ClassCode* mClassCode = initItem_ClassCode(mMethod->method_ids,mMethod->Offset);
			classCode.Add(mClassCode);
		}
	}
	m_ids = 0;
	for(int m_i = 0;m_i < mVirtualSize;m_i++)
	{
		item_ClassMethod* mMethod = new item_ClassMethod();
		mMethod->ids = DexUtils::readUnsignedLeb128(&mData);
		mMethod->flags = DexUtils::readUnsignedLeb128(&mData);
		mMethod->Offset = DexUtils::readUnsignedLeb128(&mData);
		m_ids += mMethod->ids;
		mMethod->method_ids = m_ids;
		mItem->Virtual.Add(mMethod);
		//获取code，
		if(0 != mMethod->Offset)
		{
			item_ClassCode* mClassCode = initItem_ClassCode(mMethod->method_ids,mMethod->Offset);
			classCode.Add(mClassCode);
		}
	}
	mItem->MemSize = (uint)mData - (uint)pMDex->pBackHeader - inAddr;
	return mItem;
}
/*
************************************************************
*			MDexFile_ClassDef
* 初始化classdef列表
************************************************************
*/
MDexFile_ClassDef::MDexFile_ClassDef(MDexFile* inMDex)
{
	pMDex = inMDex;
	DexClassDef* Item =(DexClassDef*)MDexFile_Header::GetAddrByClassDef(inMDex->pBackHeader);
	classDef.Clear();
	classData.Clear();
	classCode.Clear();
	for(int m_i = 0;m_i < inMDex->pBackHeader->classDefsSize;m_i++)
	{
		item_ClassDef* mItem = new item_ClassDef();
		mItem->classIdx = Item->classIdx;
		mItem->accessFlags = Item->accessFlags;
		mItem->superclassIdx = Item->superclassIdx;
		mItem->interfacesOff = Item->interfacesOff;
		mItem->sourceFileIdx = Item->sourceFileIdx;
		mItem->annotationsOff = Item->annotationsOff;
		mItem->classDataOff = Item->classDataOff;
		mItem->staticValuesOff = Item->staticValuesOff;
		Item++;
		classDef.Add(mItem);
		if(mItem->classDataOff != 0)
		{
			item_ClassData* mClassData = initArray_ClassData(mItem->classDataOff);
			classData.Add(mClassData);
		}
	}
	DEXLOG("MDexFile_ClassDef %d",classDef.GetSize());
}
/*
 * size_t MDexFile_ClassDef::getCodeLength(uint inCodeOff)
{
	DexClassCode* mCode = (DexClassCode*)(inCodeOff +(uint)pMDex->pBackHeader);
	uint mSize= DexUtils::AlignAdd(mCode->code_size*2 + 0x10,4);
	uint m_Try = mCode->try_size;
	if(m_Try != 0)
	{
		//直接跳过固定长度的 Tries info，定位到Catch handlers info
		mSize += 0x08*m_Try;
		void* mCatch = (void*)((uint)mCode +  mSize);
		//获取到CatchBlocks
		uint mCatchBlocks = DexUtils::readUnsignedLeb128(&mCatch);
		for(int m_i = 0;m_i < mCatchBlocks;m_i++)
		{
			uint mSize_Handler = DexUtils::readUnsignedLeb128(&mCatch);
			if(mSize_Handler == 0)
			{
				DexUtils::readUnsignedLeb128(&mCatch);
			}else
			{
				int m_Handler = mSize_Handler;
				if(mSize_Handler > 0x40)
				{
					DexUtils::readUnsignedLeb128(&mCatch);
					m_Handler = (0 -mSize_Handler)&0x7F;
				}
				for(int m_j = 0;m_j < m_Handler; m_j++){
					DexUtils::readUnsignedLeb128(&mCatch);
					DexUtils::readUnsignedLeb128(&mCatch);
				}
				if(mSize_Handler > 0x40){
					DexUtils::readUnsignedLeb128(&mCatch);
				}
			}
		}
		mSize = DexUtils::Align_4((uint)mCatch - (uint)mCode);
	}
	return mSize;
}
 *
 */
