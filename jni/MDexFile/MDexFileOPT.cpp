/*
 * MDexFileOPT.cpp
 *
 *  Created on: 2015年12月25日
 *      Author: LsMouse
 */

#include "MDexFile.h"
/*
************************************************************
*				initMemory_IDS
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_OPT::initMemory_IDS(ModMemory* inMemory)
{
	inMemory->newNameMem(MemDex_OPTHeader,0x28);
}
/*
************************************************************
*				initMemory_IDS
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_OPT::initMemory_Code(ModMemory* inMemory)
{
	DEXLOG("MDexFile_OPT initMemory_Code %p",DexUtils::AlignAdd(optHeader->depsLength+optHeader->auxLength,0x10));
	item_Mem* item = inMemory->newNameMem(MemDex_OPTData,DexUtils::AlignAdd(optHeader->depsLength+optHeader->auxLength,0x10));
	memcpy(item->Addr,Odex_data,DexUtils::AlignAdd(optHeader->depsLength+optHeader->auxLength,0x10));
}
/*
************************************************************
*				initMemory_IDS
*	申请内存到inMemory模块
************************************************************
*/
void MDexFile_OPT::toMemory(ModMemory* inMemory)
{
	item_Mem* item = inMemory->getByName(MemDex_OPTData);
	optHeader->dexOffset = 0x28;
	optHeader->dexLength = pMDex->pHeader->pBackHeader->fileSize;
	optHeader->depsOffset = item->Offset;
	optHeader->auxOffset =DexUtils::AlignAdd(optHeader->depsOffset +optHeader->depsLength,0x10);
	item = inMemory->getByName(MemDex_OPTHeader);
	memcpy(item->Addr,optHeader,0x28);
}
/*
************************************************************
*			MDexFile_OPT
*
************************************************************
**/
MDexFile_OPT::MDexFile_OPT(MDexFile* inMDex)
{
	pMDex = inMDex;
	optHeader = (DexOptHeader*)malloc(0x28);
	memcpy(optHeader,pMDex->LoadAddr,0x28);
	//
	Odex_data = malloc(DexUtils::AlignAdd(optHeader->depsLength+optHeader->auxLength,0x10));
	memcpy(Odex_data,(void*)((uint)pMDex->LoadAddr+optHeader->depsOffset),DexUtils::AlignAdd(optHeader->depsLength+optHeader->auxLength,0x10));
}
