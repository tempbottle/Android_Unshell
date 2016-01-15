/*
 * MDexFileAnnot.cpp
*	关系对应图
*	Annotation Dir(kDexTypeAnnotationsDirectoryItem)(级别大)
*		指向
*		annotation set ref list(kDexTypeAnnotationSetRefList)
*			指向
* 			annotation set(kDexTypeAnnotationSetItem)
* 				指向
*				annotation(kDexTypeAnnotationItem)(级别小)
*					指向
* 					annotation element
 *  Created on: 2015年12月5日
 *      Author: LsMouse
 */
#include "MDexFile.h"
/*
************************************************************
*				getValueLen
*获取值宽度
************************************************************
*/
static size_t getValueLen(int inType){
	int mTypeClass = inType&kDexAnnotationValueTypeMask;
	int mTypeLen = inType >>kDexAnnotationValueArgShift;
	switch (mTypeClass){
	case kDexAnnotationNull:
	case kDexAnnotationBoolean:
		return 0;
	case kDexAnnotationArray:
		return kDexAnnotationArray;
	}
	return mTypeLen+1;
}
/*
************************************************************
*				getValueLen
*获取值宽度
*
************************************************************
*/
void* MDexFile_Annot::Element(void* inAddr,size_t inSize){
	//获取基本信息
	u_char* mAddr =(u_char*)inAddr;
	for(int m_i = 0;m_i < inSize;m_i++)
	{
		int Type = DexUtils::readSignedLeb128((void**)&mAddr);
		if(Type == kDexAnnotationAnnotation)
		{
			int j_Type = DexUtils::readUnsignedLeb128((void**)&mAddr);
			int j_Size = DexUtils::readUnsignedLeb128((void**)&mAddr);
			for(int m_k  = 0;m_k < j_Size;m_k++)
			{
				int	k_data = DexUtils::readSignedLeb128((void**)&mAddr);
				int	k_Type =  *mAddr++;
				if(kDexAnnotationArray == k_Type)
				{
					int	k_Size = DexUtils::readSignedLeb128((void**)&mAddr);
					if(k_Size != 0)
					{
						mAddr = Element(mAddr,k_Size);
					}
				}else{
					mAddr += getValueLen(k_Type);
				}
			}
		}else
		{
			int mValueLen=getValueLen(Type);
			mAddr+= mValueLen;
		}
	}
	return mAddr;
}
/*
************************************************************
*				getAnnotFileOffset
*				getAnnotSetFileOffset
*				getAnnotSRLFileOffset
*				getAnnotDIRFileOffset
*				getFileOffset
************************************************************
*/
uint MDexFile_Annot::getAnnotFileOffset(uint inDexOffset)
{
	if(inDexOffset == 0)
		return 0;
	annot.Reset();
	for(int m_i = 0;m_i < annot.GetSize();m_i++)
	{
		if(annot.Get()->DexOffset == inDexOffset)
		{
			return annot.Get()->FileOffset;
		}
		annot.Next();
	}
	return inDexOffset;
}
uint MDexFile_Annot::getAnnotSetFileOffset(uint inDexOffset)
{
	if(inDexOffset == 0)
		return 0;
	annotSet.Reset();
	for(int m_i = 0;m_i < annotSet.GetSize();m_i++)
	{
		if(annotSet.Get()->DexOffset == inDexOffset)
		{
			return annotSet.Get()->FileOffset;
		}
		annotSet.Next();
	}
	return inDexOffset;
}
uint MDexFile_Annot::getAnnotSRLFileOffset(uint inDexOffset)
{
	if(inDexOffset == 0)
		return 0;
	annotSRL.Reset();
	for(int m_i = 0;m_i < annotSRL.GetSize();m_i++)
	{
		if(annotSRL.Get()->DexOffset == inDexOffset)
		{
			return annotSRL.Get()->FileOffset;
		}
		annotSRL.Next();
	}
	return inDexOffset;
}
uint MDexFile_Annot::getAnnotDIRFileOffset(uint inDexOffset)
{
	if(inDexOffset == 0)
		return 0;
	annotDIR.Reset();
	for(int m_i = 0;m_i < annotDIR.GetSize();m_i++)
	{
		if(annotDIR.Get()->DexOffset == inDexOffset)
		{
			return annotDIR.Get()->FileOffset;
		}
		annotDIR.Next();
	}
	return inDexOffset;
}
uint MDexFile_Annot::getFileOffset(uint inDexOffset)
{
	if(inDexOffset == 0)
		return 0;
	uint mOffset = getAnnotFileOffset(inDexOffset);
	if(mOffset != inDexOffset)
		return mOffset;
	mOffset = getAnnotSetFileOffset(inDexOffset);
	if(mOffset != inDexOffset)
			return mOffset;
	mOffset = getAnnotSRLFileOffset(inDexOffset);
	if(mOffset != inDexOffset)
			return mOffset;
	mOffset = getAnnotDIRFileOffset(inDexOffset);

	return mOffset;
}
/*
************************************************************
*				initMemory
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_Annot::initMemory(ModMemory* inMemory)
{
	//MemDex_Annot
	uint mCodeSize = 0;
	annot.Reset();
	for(int m_i = 0;m_i < annot.GetSize();m_i++)
	{
		mCodeSize +=annot.Get()->MemSize;
		annot.Next();
	}
	item_Mem* item = inMemory->newNameMem(MemDex_Annot,DexUtils::AlignAdd(mCodeSize,4));
	DEXLOG("MemDex_Annot Offset : %p ,Length : %p ",item->Offset,item->Length);
	//MemDex_AnnotSet
	mCodeSize = 0;
	annotSet.Reset();
	for(int m_i = 0;m_i < annotSet.GetSize();m_i++)
	{
		mCodeSize +=annotSet.Get()->MemSize;
		annotSet.Next();
	}
	item = inMemory->newNameMem(MemDex_AnnotSet,DexUtils::AlignAdd(mCodeSize,4));
	DEXLOG("MemDex_AnnotSet Offset : %p ,Length : %p ",item->Offset,item->Length);
	//MemDex_AnnotSRL
	mCodeSize = 0;
	annotSRL.Reset();
	for(int m_i = 0;m_i < annotSRL.GetSize();m_i++)
	{
		mCodeSize +=annotSRL.Get()->MemSize;
		annotSRL.Next();
	}
	item = inMemory->newNameMem(MemDex_AnnotSRL,DexUtils::AlignAdd(mCodeSize,4));
	DEXLOG("MemDex_AnnotSRL Offset : %p ,Length : %p ",item->Offset,item->Length);
	//
	mCodeSize = 0;
	annotDIR.Reset();
	for(int m_i = 0;m_i < annotDIR.GetSize();m_i++)
	{
		mCodeSize +=annotDIR.Get()->MemSize;
		annotDIR.Next();
	}
	item = inMemory->newNameMem(MemDex_AnnotDIR,DexUtils::AlignAdd(mCodeSize,4));
	DEXLOG("MemDex_AnnotDIR Offset : %p ,Length : %p ",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
* 这里直接将StringIds和StringCode数据填充完毕
* 实现数据重排列
************************************************************
*/
void MDexFile_Annot::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_Annot toMemory");
	//Annot
	item_Mem* mem_Annot = inMemory->getByName(MemDex_Annot);
	char* mAddr_Annot = (char*)mem_Annot->Addr;
	annot.Reset();
	for(int m_i = 0;m_i < annot.GetSize();m_i++)
	{
		annot.Get()->FileOffset =(uint)mAddr_Annot - (uint)mem_Annot->Addr + mem_Annot->Offset;
		memcpy(mAddr_Annot,annot.Get()->data,annot.Get()->MemSize);
		mAddr_Annot+= annot.Get()->MemSize;
		annot.Next();
	}
	//mem_AnnotSet
	item_Mem* mem_AnnotSet = inMemory->getByName(MemDex_AnnotSet);
	uint* mAddr_AnnotSet = (uint*)mem_AnnotSet->Addr;
	annotSet.Reset();
	for(int m_i = 0;m_i < annotSet.GetSize();m_i++)
	{
		annotSet.Get()->FileOffset =(uint)mAddr_AnnotSet - (uint)mem_AnnotSet->Addr + mem_AnnotSet->Offset;
		MeVertor<item_AnnotOffset> item = annotSet.Get()->item;
		*mAddr_AnnotSet++ =item.GetSize();
		item.Reset();
		for(int m_j = 0;m_j < item.GetSize();m_j++)
		{
			*mAddr_AnnotSet++ = getAnnotFileOffset(item.Get()->Offset);
			item.Next();
		}
		mAddr_Annot+= annot.Get()->MemSize;
		annotSet.Next();
	}
	//AnnotSRL
	item_Mem* mem_AnnotSRL = inMemory->getByName(MemDex_AnnotSRL);
	uint* mAddr_AnnotSRL = (uint*)mem_AnnotSRL->Addr;
	annotSRL.Reset();
	for(int m_i = 0;m_i < annotSRL.GetSize();m_i++)
	{
		annotSRL.Get()->FileOffset =(uint)mAddr_AnnotSRL - (uint)mem_AnnotSRL->Addr + mem_AnnotSRL->Offset;
		MeVertor<item_AnnotOffset> item = annotSRL.Get()->item;
		*mAddr_AnnotSRL++ =item.GetSize();
		item.Reset();
		for(int m_j = 0;m_j < item.GetSize();m_j++)
		{
			*mAddr_AnnotSRL++ = getFileOffset(item.Get()->Offset);
			item.Next();
		}
		annotSRL.Next();
	}
	//AnnotDIR
	item_Mem* mem_AnnotDIR = inMemory->getByName(MemDex_AnnotDIR);
	uint* mAddr_AnnotDIR = (uint*)mem_AnnotDIR->Addr;
	annotDIR.Reset();
	for(int m_i = 0;m_i < annotSRL.GetSize();m_i++)
	{
		item_AnnotDIR* item = annotDIR.Get();
		item->FileOffset =(uint)mAddr_AnnotDIR - (uint)mem_AnnotDIR->Addr + mem_AnnotDIR->Offset;

		*mAddr_AnnotDIR++ = getFileOffset(item->Offset_Class);
		*mAddr_AnnotDIR++ = item->Size_Feilds;
		*mAddr_AnnotDIR++ = item->Size_Methods;
		*mAddr_AnnotDIR++ =	item->Size_Params;
		item->item.Reset();
		for(int m_j = 0;m_j < item->item.GetSize();m_j++)
		{
			*mAddr_AnnotDIR++ = item->item.Get()->Idx;
			*mAddr_AnnotDIR++ = getFileOffset(item->item.Get()->Offset_Annot);
			item->item.Next();
		}
		annotDIR.Next();
	}
}
/*
************************************************************
*			Init_Annot
* 初始化使用
************************************************************
*/
void MDexFile_Annot::Init_Annot(MDexFile* inMDex)
{
	uint moffset =inMDex->pMap->getOffset(kDexTypeAnnotationItem);
	uint msize=inMDex->pMap->getSize(kDexTypeAnnotationItem);
	DEXLOG("MDexFile_Annot Init_Annot moffset: %p ,msize: %d ",moffset,msize);
	annot.Clear();
	if((moffset == 0)||(msize == 0))
		return ;
	char* mAddr_Annot = (char*)pMDex->pBackHeader + moffset;
	for(int m_i = 0;m_i < msize;m_i++)
	{
//		DEXLOG("MDexFile_Annot Init_Annot m_i: %p ,mAddr_Annot %p",m_i, (uint)mAddr_Annot - (uint)pMDex->pBackHeader);

		item_Annot* mAnnot = new item_Annot();
		mAnnot->DexOffset = (uint)mAddr_Annot - (uint)pMDex->pBackHeader;
		int mVisibility = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
		int mType = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
		int mCount = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
//		DEXLOG("MDexFile_Annot mCount %p",mCount);
		for(int m_j = 0;m_j < mCount;m_j++)
		{
//			DEXLOG("MDexFile_Annot %p", (uint)mAddr_Annot - (uint)pMDex->pBackHeader);
			int mdata = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
			int mEType = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
		//	int mEType = *mAddr_Annot++;
			if(kDexAnnotationAnnotation == mEType)
			{
				int j_Type = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
				int j_Size = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
				for(int m_k  = 0;m_k < j_Size;m_k++)
				{
					int	k_data = DexUtils::readSignedLeb128((void**)&mAddr_Annot);
					int	k_Type =  *mAddr_Annot++;
					if(kDexAnnotationArray == k_Type)
					{
						int	k_Size = DexUtils::readSignedLeb128((void**)&mAddr_Annot);
						if(k_Size != 0)
						{
							mAddr_Annot = Element(mAddr_Annot,k_Size);
						}
					}else{
						mAddr_Annot += getValueLen(k_Type);
					}
				}
			}else if(kDexAnnotationArray == mEType)
			{//Array
				int mESize = DexUtils::readUnsignedLeb128((void**)&mAddr_Annot);
//				if(m_i == 0x33b)
//					DEXLOG("MDexFile_Annot mESize %p",mESize);
//				if(m_i == 0x33b)
//					DEXLOG("MDexFile_Annot mAddr_Annot %p",mAddr_Annot - (uint)pMDex->pBackHeader);
				mAddr_Annot = Element(mAddr_Annot,mESize);
//				if(m_i == 0x33b)
//					DEXLOG("MDexFile_Annot mAddr_Annot1 %p",mAddr_Annot - (uint)pMDex->pBackHeader);
			}else
			{//NoArray
				int mValueType = getValueLen(mEType);
				mAddr_Annot += mValueType;
			}
		}
		mAnnot->MemSize = (uint)mAddr_Annot - (uint)pMDex->pBackHeader - mAnnot->DexOffset;
		mAnnot->data = malloc(mAnnot->MemSize);
		memcpy(mAnnot->data,(char*)pMDex->pBackHeader+mAnnot->DexOffset,mAnnot->MemSize);
		annot.Add(mAnnot);
	}
	//
}
/*
************************************************************
*			Init_AnnotSet
* 初始化使用
************************************************************
*/
void MDexFile_Annot::Init_AnnotSet(MDexFile* inMDex)
{
	uint moffset =inMDex->pMap->getOffset(kDexTypeAnnotationSetItem);
	uint msize=inMDex->pMap->getSize(kDexTypeAnnotationSetItem);
	DEXLOG("MDexFile_Annot Init_AnnotSet moffset:%p,msize:%d",moffset,msize);
	annotSet.Clear();
	if((moffset == 0)||(msize == 0))
		return ;
	uint* mAddr_Set = (uint*)((char*)pMDex->pBackHeader + moffset);
	for(int m_i = 0;m_i < msize;m_i++)
	{
		item_AnnotSet* mItem = new item_AnnotSet();
		mItem->DexOffset = (uint)mAddr_Set - (uint)pMDex->pBackHeader;
		mItem->item.Clear();
		int mSize = *mAddr_Set++;
		for(int m_j = 0;m_j < mSize;m_j++)
		{
			item_AnnotOffset* mdata = new item_AnnotOffset();
			mdata->Offset = *mAddr_Set++;
			mItem->item.Add(mdata);
		}
		mItem->MemSize =(uint)mAddr_Set - (uint)pMDex->pBackHeader - mItem->DexOffset;
		annotSet.Add(mItem);
	}
}
/*
************************************************************
*			Init_AnnotSRL
* 初始化使用
************************************************************
*/
void MDexFile_Annot::Init_AnnotSRL(MDexFile* inMDex)
{
	uint moffset =inMDex->pMap->getOffset(kDexTypeAnnotationSetRefList);
	uint msize=inMDex->pMap->getSize(kDexTypeAnnotationSetRefList);
	DEXLOG("MDexFile_Annot Init_AnnotSRL moffset:%p,msize:%d",moffset,msize);
	annotSRL.Clear();
	if((moffset == 0)||(msize == 0))
		return ;
	uint* mAddr_SRL = (uint*)((char*)pMDex->pBackHeader + moffset);
	for(int m_i = 0;m_i < msize;m_i++)
	{
		item_AnnotSRL* mItem = new item_AnnotSRL();
		mItem->DexOffset = (uint)mAddr_SRL - (uint)pMDex->pBackHeader;
		mItem->item.Clear();
		int mSize = *mAddr_SRL++;
		for(int m_j = 0;m_j < mSize;m_j++)
		{
			item_AnnotOffset* mdata = new item_AnnotOffset();
			mdata->Offset = *mAddr_SRL++;
			mItem->item.Add(mdata);
		}
		mItem->MemSize =(uint)mAddr_SRL - (uint)pMDex->pBackHeader - mItem->DexOffset;
		annotSRL.Add(mItem);
	}
}
/*
************************************************************
*			Init_AnnotDIR
* 初始化使用
************************************************************
*/
void MDexFile_Annot::Init_AnnotDIR(uint inoffset,size_t inSize)
{
	annotDIR.Clear();
	if((inoffset == 0)||(inSize == 0))
		return ;
	uint* mAddr_DIR = (uint*)((char*)pMDex->pBackHeader + inoffset);
	for(int m_i = 0;m_i < inSize;m_i++)
	{
		item_AnnotDIR* mItem = new item_AnnotDIR();
		mItem->DexOffset = (uint)mAddr_DIR - (uint)pMDex->pBackHeader;
		mItem->Offset_Class = *mAddr_DIR++;
		mItem->Size_Feilds = *mAddr_DIR++;
		mItem->Size_Methods = *mAddr_DIR++;
		mItem->Size_Params = *mAddr_DIR++;
		size_t mSize = mItem->Size_Feilds+mItem->Size_Methods+mItem->Size_Params;
		mItem->item.Clear();
		for(int m_j = 0;m_j< mSize;m_j++){
			item_AnnotDirItem* j_Item = new item_AnnotDirItem();
			j_Item->Idx =*mAddr_DIR++;
			j_Item->Offset_Annot = *mAddr_DIR++;
			mItem->item.Add(j_Item);
		}
		mItem->MemSize = (uint)mAddr_DIR - (uint)pMDex->pBackHeader - mItem->DexOffset;
		annotDIR.Add(mItem);
	}
}
/*
************************************************************
*			MDexFile_Annot
* 初始化类数据
************************************************************
*/
MDexFile_Annot::MDexFile_Annot(MDexFile* inMDex)
{
	pMDex = inMDex;
	Init_Annot(pMDex);
	Init_AnnotSet(pMDex);
	Init_AnnotSRL(pMDex);
	DEXLOG("MDexFile_Annot Init_AnnotDIR");
	Init_AnnotDIR(pMDex->pMap->getOffset(kDexTypeAnnotationsDirectoryItem),pMDex->pMap->getSize(kDexTypeAnnotationsDirectoryItem));

}

