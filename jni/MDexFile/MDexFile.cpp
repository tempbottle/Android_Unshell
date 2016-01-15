/*
 *	MDexFile.cpp
 *
 *
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */
#include "MDexFile.h"
#include "Utils/syscall.H"
/*
************************************************************
*			getClassDefSize
* 获取classdef个数
************************************************************
*/
size_t MDexFile::getClassDefSize()
{
	if((NULL == pClassDefs)||(NULL == pClassDefs))
		return 0;
	return pClassDefs->classDef.GetSize();
}
/*
************************************************************
*			Parse
* 通过索引对整个Dex进行解析*
* 1.解析 String 段数据
* 2.解析 TypeList
************************************************************
*/
void MDexFile::Parse()
{
	if(isErr)
		return ;
	pStringIds->ParseStringCode();
	pTypeIds->MDexFile_TypeList(pMap->getOffset(kDexTypeTypeList),pMap->getSize(kDexTypeTypeList));
	pEncode = new MDexFile_Encode(this);
	pAnnot = new MDexFile_Annot(this);
}
/*
************************************************************
*			MDexFile
* 直接根据获取到的数据对文件进行解析。*
************************************************************
*/
void MDexFile::LoadAllClass()
{


}
/*
************************************************************
*			getClassDescriptor
* 直接根据获取到的数据对文件进行解析。*
************************************************************
*/
char* MDexFile::getClassDescriptor(int inNo)
{
	item_ClassDef* mclassdef = pClassDefs->classDef.Get(inNo);
	if(NULL != mclassdef)
	{
		uint m_TypeIdx = mclassdef->classIdx;
		return pStringIds->getStringByIds(pTypeIds->typeIds.Get(m_TypeIdx)->type);
	}
	return NULL;
}
/*
************************************************************
*			realignment
* 根据保存数的数据重新排列DEX
* 1.根据文件各个段大小分配内存，设置段偏移
* 2.根据段偏移重新设置数据
*
* *
************************************************************
*/
void MDexFile::realignment()
{
	//根据不同段进行布局
	Dexmemory = new ModMemory();
	//布局IDS
	pHeader->initMemory_IDS(Dexmemory);
	pStringIds->initMemory_IDS(Dexmemory);
	pTypeIds->initMemory_IDS(Dexmemory);
	pProtoIds->initMemory_IDS(Dexmemory);
	pFieldIds->initMemory_IDS(Dexmemory);
	pMethodIds->initMemory_IDS(Dexmemory);
	pClassDefs->initMemory_IDS(Dexmemory);
	pMap->initMemory_IDS(Dexmemory);
	//布局CODE
	pStringIds->initMemory_Code(Dexmemory);
	pTypeIds->initMemory_List(Dexmemory);
	pEncode->initMemory(Dexmemory);
	pAnnot->initMemory(Dexmemory);
	pClassDefs->initMemory_Data(Dexmemory);
	pClassDefs->initMemory_Code(Dexmemory);
	//重新设置数据,
	pHeader->toMemory(Dexmemory);
	pMap->toMemory(Dexmemory);
	pStringIds->toMemory(Dexmemory);
	pTypeIds->toMemory(Dexmemory);
	pProtoIds->toMemory(Dexmemory);
	pFieldIds->toMemory(Dexmemory);
	pMethodIds->toMemory(Dexmemory);
	pEncode->toMemory(Dexmemory);
	pAnnot->toMemory(Dexmemory);
	pClassDefs->toMemory(Dexmemory);
	//
	if(pOptHeader != NULL)
	{
		pOptHeader->initMemory_IDS(Dexmemory);
		pOptHeader->initMemory_Code(Dexmemory);
		pOptHeader->toMemory(Dexmemory);
	}
}
void MDexFile::Save_BySVC(char* appName,char* inFile)
{
	//获取保存路径
	char* filePath = (char*)malloc(1024);
	memset(filePath,0,1024);
	sprintf(filePath,"/sdcard/DumpDex/%s_%s",inFile,appName);
	int fd = syscall(5,filePath,O_RDWR|O_CREAT);
	//查找可写文件路径
	if(fd == -1){
		DEXLOG("[Dump]Fail SD Path:%s",filePath);
		DEXLOG("[Dump]SD Open file fail!");
		memset(filePath,0,1024);
		if(appName != NULL){
			sprintf(filePath,"/data/data/%s/%s",appName,inFile);
			fd=  syscall(5,filePath,O_RDWR|O_CREAT);
			if(fd == -1){
				DEXLOG("[Dump]Fail Data Path:%s",filePath);
				free(filePath);
				return;
			}
		}else{
			DEXLOG("[Dump]Fail appName Is Null");
			free(filePath);
			return ;
		}
	}
	//
	if(pOptHeader != NULL)
	{
		item_Mem* mMod = Dexmemory->getByName(MemDex_OPTHeader);
		if(mMod != NULL)
		{
			DEXLOG("[Dump]正在下载: %s ,offset: %p ,Len: %p",mMod->name,mMod->Offset,mMod->Length);
			sys_write(fd,mMod->Addr,mMod->Length);
		}
	}
	item_Mem* mMod = Dexmemory->Memory;
	while(mMod != NULL)
	{
		if((strcmp(mMod->name,MemDex_OPTHeader) == 0)||(strcmp(mMod->name,MemDex_OPTData) == 0)){

		}else if(mMod->name != NULL){
			DEXLOG("[Dump]正在下载: %s ,offset: %p ,Len: %p",mMod->name,mMod->Offset,mMod->Length);
			syscall(4,fd,mMod->Addr,mMod->Length);
		}
		mMod = mMod->next;
	}
	if(pOptHeader != NULL)
	{
		item_Mem* mMod = Dexmemory->getByName(MemDex_OPTData);
		if(mMod != NULL)
		{
			DEXLOG("[Dump]正在下载: %s ,offset: %p ,Len: %p",mMod->name,mMod->Offset,mMod->Length);
			syscall(4,fd,mMod->Addr,mMod->Length);
		}
	}
	//释放至于资源
	syscall(6,fd);
	DEXLOG("[Dump]Dump File Path:%s",filePath);
}
/*
************************************************************
*			SavetoFile
*将数据保存成dex文件
*	先将文件排列成功后重写偏移
*	按照段保存数据
* *
************************************************************
*/
void MDexFile::SaveDex(char* appName,char* inFile)
{
	if(isErr)
		return;
	realignment();
	Save_BySVC(appName,inFile);
	DEXLOG("SavetoFile oVER");
}
/*
************************************************************
*			MDexFile
* 直接根据获取到的数据对文件进行解析。
* 1.解析Header
* 2.解析string索引
* 3.解析type索引
* 4.解析Field索引
* 5.解析Method索引
* 6.解析Proto索引
* 7.解析classdef
* 8.解析MAP
* 9.解析EncodeArray
* 10.解析Annot
* 11.解析Debug Info
* *
************************************************************
*/
MDexFile::MDexFile(void* inDexFile,size_t inLength)
{
	LoadAddr = inDexFile;
	Length = inLength;
	isErr = false;
	//判断是否为Dex
	if(!DexUtils::isDex(inDexFile))
	{
		DEXLOG("传入参数出现错误，非Dex格式文件!");
		isErr = true;
		return ;
	}
	DexHeader* pDexHeader = (DexHeader*)DexUtils::GetBase(inDexFile);
	pBackHeader = pDexHeader;
	if(pDexHeader != inDexFile)
	{
		DEXLOG("ODex格式文件!");
		pOptHeader = new MDexFile_OPT(this);
	}else
	{
		pOptHeader = NULL;
	}
	//接下来对DexFile文件索引开始解析、(索引数据不会错，只会存在被清空）
	pHeader = new MDexFile_Header(this);
	pMap = new MDexFile_Map(this);
	pStringIds = new MDexFile_StringId(this);
	pTypeIds = new MDexFile_TypeId(this);
	pFieldIds = new MDexFile_FieldId(this);
	pMethodIds = new MDexFile_MethodId(this);
	pProtoIds = new MDexFile_ProtoId(this);
	pClassDefs = new MDexFile_ClassDef(this);
	DEXLOG("MDexFile oVER!");
}
/*
************************************************************
*			MDexFile
* 初始化模块数据
************************************************************
*/
MDexFile::MDexFile()
{
	pOptHeader = NULL;
	pHeader = NULL;
	pStringIds = NULL;
	pTypeIds = NULL;
	pFieldIds = NULL;
	pMethodIds = NULL;
	pProtoIds = NULL;
	pClassDefs = NULL;
	pMap = NULL;
	pEncode = NULL;
	pAnnot = NULL;
}
