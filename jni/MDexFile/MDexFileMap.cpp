/*
 * MDexFileMap.cpp
 *
 *  Created on: 2015年12月5日
 *      Author: LsMouse
 */
#include "MDexFile.h"
/*
************************************************************
*			getOffset
* 获取Map中对应Type的Offset
************************************************************
*/
item_Map* MDexFile_Map::getMap(uint inType)
{
	uint mSize = map.GetSize();
	map.Reset();
	for(int m_i=0;m_i<mSize;m_i++)
	{
		if((map.Get() != NULL)&&(inType == map.Get()->type))
		{
			return map.Get();
		}
		map.Next();
	}
	return NULL;
}
/*
************************************************************
*			getOffset
* 获取Map中对应Type的Offset
************************************************************
*/
uint MDexFile_Map::getOffset(uint inType)
{
	item_Map* mMap = getMap(inType);
	if(mMap == NULL)
		return 0;
	return mMap->offset;
}
/*
************************************************************
*			MDexFile_Map
* 获取Map中对应Type的Size
************************************************************
*/
uint MDexFile_Map::getSize(uint inType)
{
	item_Map* mMap = getMap(inType);
	if(mMap == NULL)
		return 0;
	return mMap->size;
}
/*
************************************************************
*				initMemory_IDS
*	申请内存，存放索引数据
************************************************************
*/
void MDexFile_Map::initMemory_IDS(ModMemory* inMemory)
{
	item_Mem* item = inMemory->newNameMem(MemDex_MAP,0x20*sizeof(struct DexMapId)+4);
	DEXLOG("MDexFile_Map Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
* 实现数据重排列
************************************************************
*/
void MDexFile_Map::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_Map toMemory");
	item_Map* mMap;
	item_Mem* mMem;
	if(((mMap = getMap(kDexTypeStringIdItem))!= NULL)&&
		((mMem = inMemory->getByName(MemDex_StringIds))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeTypeIdItem))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_TypeIds))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeProtoIdItem))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_ProtoIds))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeFieldIdItem))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_FieldIds))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeMethodIdItem))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_MethoIds))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeClassDefItem))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_ClassDef))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeMapList))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_MAP))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeTypeList))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_TypeList))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeAnnotationSetRefList))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_AnnotSRL))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeAnnotationSetItem))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeClassDataItem))!= NULL)&&
				((mMem = inMemory->getByName(MemDex_AnnotSet))!= NULL))
	{
		mMem = inMemory->getByName(MemDex_ClassData);
		mMap->offset = mMem->Offset;
		mMap->size = pMDex->pClassDefs->classData.GetSize();
	}else{
		mMem = inMemory->getByName(MemDex_ClassData);
		item_Map* mitem = new item_Map();
		mitem->type = kDexTypeClassDataItem;
		mitem->offset = mMem->Offset;
		mitem->size = pMDex->pClassDefs->classData.GetSize();

		map.Add(mitem);
	}
	if(((mMap = getMap(kDexTypeCodeItem))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_ClassCode))!= NULL))
	{
		mMap->size = pMDex->pClassDefs->classCode.GetSize();
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeStringDataItem))!= NULL)&&
				((mMem = inMemory->getByName(MemDex_StringCode))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeAnnotationItem))!= NULL)&&
			((mMem = inMemory->getByName(MemDex_Annot))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeEncodedArrayItem))!= NULL)&&
				((mMem = inMemory->getByName(MemDex_EncodeArray))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	if(((mMap = getMap(kDexTypeAnnotationsDirectoryItem))!= NULL)&&
				((mMem = inMemory->getByName(MemDex_AnnotDIR))!= NULL))
	{
		mMap->offset = mMem->Offset;
	}
	//保存数据
	u4* mAddr_Map = (u4*)inMemory->getByName(MemDex_MAP)->Addr;
	*mAddr_Map++ =map.GetSize();
	DexMapId* mDexitem =(DexMapId*)(mAddr_Map);
	map.Reset();
	for(int m_i=0;m_i < map.GetSize();m_i++)
	{
		mDexitem->type = map.Get()->type;
		mDexitem->unused = map.Get()->unused;
		mDexitem->size = map.Get()->size;
		mDexitem->offset = map.Get()->offset;
		mDexitem++;
		map.Next();
	}
}


/*
************************************************************
*			MDexFile_Map
* 初始化MDexFile_Map
************************************************************
*/
MDexFile_Map::MDexFile_Map(MDexFile* inMDex)
{
	pMDex = inMDex;
	uint* mapbase = (uint*)MDexFile_Header::GetAddrByMap(inMDex->pBackHeader);
	uint mapSize = *mapbase;
	DexMapId* mDexitem =(DexMapId*)((uint)mapbase+4);
	map.Clear();
	for(int m_i=0;m_i < mapSize;m_i++)
	{
		item_Map* mItem = new item_Map();
		mItem->type = mDexitem->type;
		mItem->unused = mDexitem->unused;
		mItem->size = mDexitem->size;
		mItem->offset = mDexitem->offset;
		map.Add(mItem);
		mDexitem++;
	}
}
