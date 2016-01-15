/*
 * MDexFileHeader.cpp
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */
#include "MDexFile.h"
/*
************************************************************
*				GetAddrByHeader
*	获取Dex DexHeader加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByHeader(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	return pHeader;
}
/*
************************************************************
*				GetAddrByStringIds
*	获取Dex DexStringIds加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByStringIds(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexStringIds = (u_char*)pHeader+pHeader->stringIdsOff;
	return mDexStringIds;
}
/*
************************************************************
*				GetAddrByTypeIds
*	获取Dex DexTypeIds加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByTypeIds(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexTypeIds= (u_char*)pHeader+pHeader->typeIdsOff;
	return mDexTypeIds;
}
/*
************************************************************
*				GetAddrByFieldIds
*	获取Dex DexFieldIds加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByFieldIds(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexFieldIds= (u_char*)pHeader+pHeader->fieldIdsOff;
	return mDexFieldIds;
}
/*
************************************************************
*				GetAddrByMethodIds
*	获取Dex DexMethodIds加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByMethodIds(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexMethodIds= (u_char*)pHeader+pHeader->methodIdsOff;
	return mDexMethodIds;
}
/*
************************************************************
*				GetAddrByProtoIds
*	获取Dex DexProtoIds加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByProtoIds(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexProtoIds= (u_char*)pHeader+pHeader->protoIdsOff;
	return mDexProtoIds;
}
/*
************************************************************
*				GetAddrByClassDef
*	获取Dex DexClassDef加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByClassDef(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexClassDef= (u_char*)pHeader+pHeader->classDefsOff;
	return mDexClassDef;
}
/*
************************************************************
*				GetAddrByData
*	获取Dex DexData加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByData(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexData= (u_char*)pHeader+pHeader->dataOff;
	return mDexData;
}
/*
************************************************************
*				GetAddrByMap
*	获取Dex mDexMap加载地址*
************************************************************
*/
void* MDexFile_Header::GetAddrByMap(void* inHeader)
{
	if(!DexUtils::isDex(inHeader))
		return NULL;
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	void* mDexMap = (u_char*)pHeader+pHeader->mapOff;
	return mDexMap;
}
/*
************************************************************
*				Printf
*	打印头数据*
************************************************************
*/
void MDexFile_Header::Printf(void* inHeader)
{
	DEXLOG("Dex_Header@Log \n");
	if(DexUtils::GetBase(inHeader) == NULL){
		DEXLOG("Dex_Header print err!\n");
		return ;
	}else if(DexUtils::GetBase(inHeader) == inHeader){
		DEXLOG("Dex_Header Dex !\n");
	}else{
		DEXLOG("Dex_Header ODex!\n");
	}
	DexHeader* pHeader = (DexHeader*)DexUtils::GetBase(inHeader);
	DEXLOG("Dex Addr            : 0x%08x\n",pHeader);
	DEXLOG("magic               : '%.3s %.4s'\n", pHeader->magic,(char*)(pHeader->magic+4));
	DEXLOG("checksum            : %08x\n", pHeader->checksum);
	DEXLOG("signature           : %02x%02x...%02x%02x\n",
		pHeader->signature[0], pHeader->signature[1],
		pHeader->signature[kSHA1DigestLen-2],
		pHeader->signature[kSHA1DigestLen-1]);
	DEXLOG("file_size           : %d\n", pHeader->fileSize);
	DEXLOG("header_size         : %d\n", pHeader->headerSize);
	DEXLOG("link_size           : %d\n", pHeader->linkSize);
	DEXLOG("link_off            : %d (0x%06x)\n",
		pHeader->linkOff, pHeader->linkOff);
	DEXLOG("mapOff              : %d (0x%06x)\n",
				pHeader->mapOff, pHeader->mapOff);
	DEXLOG("string_ids_size     : %d\n", pHeader->stringIdsSize);
	DEXLOG("string_ids_off      : %d (0x%06x)\n",
		pHeader->stringIdsOff, pHeader->stringIdsOff);
	DEXLOG("type_ids_size       : %d\n", pHeader->typeIdsSize);
	DEXLOG("type_ids_off        : %d (0x%06x)\n",
		pHeader->typeIdsOff, pHeader->typeIdsOff);
	DEXLOG("field_ids_size      : %d\n", pHeader->fieldIdsSize);
	DEXLOG("field_ids_off       : %d (0x%06x)\n",
		pHeader->fieldIdsOff, pHeader->fieldIdsOff);
	DEXLOG("method_ids_size     : %d\n", pHeader->methodIdsSize);
	DEXLOG("method_ids_off      : %d (0x%06x)\n",
		pHeader->methodIdsOff, pHeader->methodIdsOff);
	DEXLOG("class_defs_size     : %d\n", pHeader->classDefsSize);
	DEXLOG("class_defs_off      : %d (0x%06x)\n",
		pHeader->classDefsOff, pHeader->classDefsOff);
	DEXLOG("data_size           : %d\n", pHeader->dataSize);
	DEXLOG("data_off            : %d (0x%06x)\n",
		pHeader->dataOff, pHeader->dataOff);
}
void MDexFile_Header::Printf()
{
	Printf(pHeader);
}
/*
************************************************************
*				initMemory_IDS
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_Header::initMemory_IDS(ModMemory* inMemory)
{
	item_Mem* item = inMemory->newNameMem(MemDex_Header,0x70);
	DEXLOG("MDexFile_Header Offset :%p,Length :%p",item->Offset,item->Length);
}
/*
************************************************************
*				toMemory
*	将数据输出到ModMemory中
************************************************************
*/
void MDexFile_Header::toMemory(ModMemory* inMemory)
{
	DEXLOG("MDexFile_Header toMemory");
	item_Mem* modIds =inMemory->getByName(MemDex_Header);
	//设置文件大小\拷贝数据到内存
	pBackHeader->fileSize =inMemory->Size();
	pBackHeader->stringIdsOff =inMemory->getByName(MemDex_StringIds)->Offset;
	pBackHeader->typeIdsOff =inMemory->getByName(MemDex_TypeIds)->Offset;
	pBackHeader->protoIdsOff =inMemory->getByName(MemDex_ProtoIds)->Offset;
	pBackHeader->fieldIdsOff =inMemory->getByName(MemDex_FieldIds)->Offset;
	pBackHeader->methodIdsOff =inMemory->getByName(MemDex_MethoIds)->Offset;
	pBackHeader->classDefsOff =inMemory->getByName(MemDex_ClassDef)->Offset;
	pBackHeader->mapOff = inMemory->getByName(MemDex_MAP)->Offset;
	pBackHeader->dataOff = inMemory->getByName(MemDex_MAP)->Offset;
	pBackHeader->dataSize =inMemory->Size() - pBackHeader->dataOff;/**/
	memcpy(modIds->Addr,pBackHeader,0x70);
}
/*
************************************************************
*				MDexFile_Header
*　初始化Header,将分析头数据，并且将数据备份起来*
************************************************************
*/
MDexFile_Header::MDexFile_Header(MDexFile* inMDex)
{
	pMDex = inMDex;
	DEXLOG("MDexFile_Header %p",inMDex->pBackHeader);
	pHeader = (DexHeader*)inMDex->pBackHeader;
	pBackHeader = (DexHeader*)malloc(0x70);
	memset(pBackHeader,0,0x70);
	memcpy(pBackHeader,pHeader,0x70);
}


