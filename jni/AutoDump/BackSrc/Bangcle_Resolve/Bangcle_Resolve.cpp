/*
 * Bangcle.cpp
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <jni.h>
#include <pthread.h>
#include "SubHook/SubHook.H"
#include "Bangcle_Resolve.H"
#include "Utils/Utils.H"
#include "MDexFile/MDexFile.h"
#include "sys/mman.h"
//声明变量
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static char* Dex_Parse = NULL;
static unsigned char* Dex_Des = NULL;
static size_t Dex_DesLen = 0;
static MDexFile* MDex;
static Res_Class Bangcle_ResClass = NULL;
static Res_Des Bangcle_ResDes = NULL;
/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	if(strstr(inName,Bangcle_secexe) != NULL)
	{
		return true;
	}else if(strstr(inName,Bangcle_DexHelper) != NULL)
	{
		return true;
	}
	return false;
}
void SetAppName(void* inAppName)
{
	AppName = (char*)inAppName;
}
void SetJavaVM(void* inJavaVM)
{
	GVM = (JavaVM*)inJavaVM;
}
/*
************************************************************
*				new$Bangcle_ResDes
@typedef unsigned char * (*Res_Des)(void *des, int len);
*用来Hook来进行需要解密数据
************************************************************
*/
//备份数据
char* (*$Res_Des)(void *des, int len);
char* New$Res_Des(void *des, int len)
{
	Dex_Des = des;
	Dex_DesLen = len;
	char* result = $Res_Des(des,len);
	DEXLOG("new$Res_Des des %p len %p result %p",des,len,result);
	ModMemory::Save_BySVC(AppName,DexUtils::GetTimeName("Des"),des,len);
	unsigned char* mBackDes = (char*)malloc(len);
	memcpy(mBackDes,des,len);
	DecodeDesHeader(mBackDes,Dex_DesLen);
	BangBangFixDex(mBackDes,Dex_DesLen);
	ModMemory::Save_BySVC(AppName,"FixDex",MDex->LoadAddr,MDex->Length);
	return result;
}
/*
************************************************************
*				new$dexFileParse
@DexFile* dexFileParse(const u1* data, size_t length, int flags)
*用来Hook来进行获取JavaVM
************************************************************
*/
void* (*$dexFileParse)(char* inAddr,size_t length, int flags) = NULL;
void* New$dexFileParse(char* inAddr,size_t length, int flags)
{
	void* result =$dexFileParse(inAddr,length,flags);
	DEXLOG("new$dexFileParse %p %p ", inAddr,length);
	if(Dex_Parse == NULL)
	{
		Dex_Parse = (char*)malloc(length);
		memcpy(Dex_Parse,inAddr,length);
		ModMemory::Save_BySVC(AppName,"Parse", inAddr,length);
		MDex = new MDexFile(Dex_Parse,length);
		MDex->Parse();
	}
	return result;
}
/*
************************************************************
*				Match_Code
* 查找关键函数点
************************************************************
*/
void* Match_Code(void* insrc,char* dest,size_t inLeng,size_t inLength2)
{
	char* mFind;
	for (int m_i = 0; m_i < inLeng;m_i++)
	{
		mFind = (char*)insrc + m_i;
		int m_j;
		for (m_j = 0; m_j < inLength2; m_j++)
		{
			char m_data = *(mFind + m_j);
			if (m_data != *(dest+ m_j))
			{
				m_j = inLength2 + 1;
			}
		}
		if (m_j == inLength2)
		{
			return mFind;
		}
	}
	return NULL;
}
//进行特征码查询,不同MCU采用的代码不一样
char Matchclass_armeabi_0[8] =  { 0xF0, 0xB5, 0x1E, 0x1C, 0x11, 0x4B, 0x14, 0x1C };
char Matchclass_armeabiv7a_0[8] = { 0x2D,0xE9,0xF0,0x41,0x1E,0x46,0x11,0x4B };
//
char Matchclass_armeabi_1[8] = { 0xF0, 0xB5, 0x1D, 0x1C, 0x31, 0x4B, 0x14, 0x1C };
char Matchclass_armeabiv7a_1[8] = { 0x2D,0xE9,0xF0,0x41,0x1C,0x46,0x2A,0x4B };
void* Find_encclass(void* inStart,size_t inSize)
{
	unsigned char* maddr = (unsigned char*)inStart;
	void* result;
	result = Match_Code(inStart, Matchclass_armeabi_0, inSize, sizeof(Matchclass_armeabi_0));
	if (result != NULL)
	{
		DEXLOG("Find_encclass Matchclass_armeabi_0");
		return result;
	}
	result = Match_Code(inStart, Matchclass_armeabiv7a_0, inSize, sizeof(Matchclass_armeabiv7a_0));
	if (result != NULL)
	{
		DEXLOG("Find_encclass Matchclass_armeabiv7a_0");
		return result;
	}
	result = Match_Code(inStart, Matchclass_armeabi_1, inSize,sizeof(Matchclass_armeabi_1));
	if (result != NULL)
	{
		DEXLOG("Find_encclass Matchclass_armeabi_1");
		return result;
	}
	result = Match_Code(inStart, Matchclass_armeabiv7a_1, inSize, sizeof(Matchclass_armeabiv7a_1));
	if (result != NULL)
	{
		DEXLOG("Find_encclass Matchclass_armeabiv7a_1");
		return result;
	}
	return NULL;
}
//进行特征码查询,不同MCU采用的代码不一样
char Matchdex_armeabiv_0[8] = { 0xF0, 0xB5, 0x66, 0x4A, 0x66, 0x4B, 0xCD, 0xB0 };
char Matchdex_armeabiv7a_0[8] = { 0x65, 0x4A, 0x2D, 0xE9, 0xF0, 0x47, 0x7A, 0x44 };
//
char Matchdex_armeabi[8] = { 0x10, 0xB5, 0x05, 0x4B, 0x0A, 0x1C, 0x05, 0x49};
char Matchdex_armeabi_1_2_5[8] = { 0xF0, 0xB5, 0x66, 0x4A, 0x66, 0x4B, 0xCD, 0xB0 };
char Matchdex_armeabiv7a_1_2_5[8] = { 0x83,0x4B,0x84,0x4A,0x7B,0x44,0x2D,0xE9 };
void* Find_encdex(void* inStart, size_t inSize)
{
	unsigned char* maddr = (unsigned char*)inStart;
	void* result;
	result = Match_Code(inStart, Matchdex_armeabi, inSize, sizeof(Matchdex_armeabi));
	if (result != NULL)
	{
		DEXLOG("Find_encdex Matchclass_armeabi_0");
		return result;
	}
	result = Match_Code(inStart, Matchdex_armeabiv_0, inSize, sizeof(Matchdex_armeabiv_0));
	if (result != NULL)
	{
		return result;
	}
	result = Match_Code(inStart, Matchdex_armeabiv7a_0, inSize, sizeof(Matchdex_armeabiv7a_0));
	if (result != NULL)
	{
		return result;
	}
	result = Match_Code(inStart, Matchdex_armeabi_1_2_5, inSize, sizeof(Matchdex_armeabi_1_2_5));
	if (result != NULL)
	{
		return result;
	}
	result = Match_Code(inStart, Matchdex_armeabiv7a_1_2_5, inSize, sizeof(Matchdex_armeabiv7a_1_2_5));
	if (result != NULL)
	{
		return result;
	}
	return NULL;
}
/**/
void* SetBef_LoadNative(char* pathName)
{
	if((Bangcle_ResClass != NULL)&&(Bangcle_ResDes != NULL))
		return pathName;
	if(strstr(pathName,Bangcle_DexHelper) != NULL)
	{
		void* mHander = dlopen(pathName,RTLD_NOW);
		void* mFunc_Start = dlsym(mHander,FINAL_sm4_setkey_enc);
		if(mFunc_Start == NULL)
		{
			mFunc_Start = dlsym(mHander,Final_JNI_OnLoad);
		}
		Bangcle_ResClass = (Res_Class)dlsym(mHander,MD5_Res_Class);
		Bangcle_ResDes = (Res_Des)dlsym(mHander,MD5_Res_Des);
		//SO 库已经自解密，现在直接查找程序修复数据，
		if(Bangcle_ResClass == NULL)
		{
			if(mFunc_Start == NULL)
			{
				DEXLOG("%s is no %s",pathName,FINAL_sm4_setkey_enc);
				return pathName;
			}
			//函数没有导出，则使用特征位查找
			void* mFind = Find_encclass(mFunc_Start,0x3000);
			if (mFind != NULL)
			{
				Bangcle_ResClass = (Res_Class)((unsigned int)mFind + 0x1);
			}
		}
		DEXLOG("Bangcle_ResClass %p",Bangcle_ResClass);
		if(Bangcle_ResDes == NULL)
		{
			if(mFunc_Start == NULL)
			{
				DEXLOG("%s is no %s",pathName,FINAL_sm4_setkey_enc);
				return pathName;
			}
			//函数没有导出，则使用特征位查找
			void* mFind = Find_encdex(mFunc_Start,0x30000);
			if (mFind != NULL) {
				Bangcle_ResDes = (Res_Des)((unsigned int)mFind + 0x1);
			}
		}
		DEXLOG("Bangcle_ResDes %p",Bangcle_ResDes);
		SubHookAddr(Bangcle_ResDes,(void*)&New$Res_Des,(void**)&$Res_Des);
	}
	return pathName;
}
void* SetAft_LoadNative(char* pathName)
{

	if(strstr(pathName,Bangcle_DexHelper) != NULL)
	{
		//加完DexHelper,可以进行，可以进行修复

	}
	return pathName;
}
/*
************************************************************
*				GetMethodData
* 从Des获取Method数据。
************************************************************
*/
DexCode* GetMethodData(unsigned char *pDesData, size_t len, unsigned int debugInfoOff, int &nDatalen)
{
	int i;
	static int j = 0;
	DesHeader *m_pDesHeader = (DesHeader *)pDesData;
	//	保存数据区起始地址
	unsigned char *m_pDesInsntData = pDesData + m_pDesHeader->insntDataOffset_r;
	//	整理下引导区信息
	DesIdxData *m_pDesIdxData = (DesIdxData *)(pDesData + m_pDesHeader->classDataOffset_r);
	unsigned int m_DesMethodCount = m_pDesHeader->classDataSize_r / (sizeof(DesIdxData));	//	这个可以计算出Method数量
	for (i = 0; i < m_DesMethodCount; i ++)
	{
		if (m_pDesIdxData[i].DebugInfoOff == debugInfoOff)
		{
			j ++;
			DexCode *pDexCode = (DexCode *)(m_pDesInsntData + m_pDesIdxData[i].InsntOffset);
			nDatalen = m_pDesIdxData[i].InsntSize;
			return pDexCode;
		}
	}
	return NULL;
}
/*
************************************************************
*				DecodeDesHeader
* 解析解密出来的文件数据
************************************************************
*/
unsigned long _byteswap_ulong(unsigned long i)
{
	unsigned int j;
	j =  (i << 24);
	j += (i <<  8) & 0x00FF0000;
	j += (i >>  8) & 0x0000FF00;
	j += (i >> 24);
	return j;
}
bool DecodeDesHeader(unsigned char* pDesData, unsigned len)
{
	DesHeader *m_pDesHeader = (DesHeader *)pDesData;
	m_pDesHeader->fileSize_r = _byteswap_ulong(m_pDesHeader->fileSize_r);
	m_pDesHeader->Md5_val_r = _byteswap_ulong(m_pDesHeader->Md5_val_r);
	m_pDesHeader->classDataOffset_r = _byteswap_ulong(m_pDesHeader->classDataOffset_r);
	m_pDesHeader->classDataSize_r = _byteswap_ulong(m_pDesHeader->classDataSize_r);
	m_pDesHeader->insntDataOffset_r = _byteswap_ulong(m_pDesHeader->insntDataOffset_r);
	m_pDesHeader->insntDataSize_r = _byteswap_ulong(m_pDesHeader->insntDataSize_r);

	//	保存数据区起始地址
	unsigned char *m_pDesInsntData = pDesData + m_pDesHeader->insntDataOffset_r;

	//	整理下引导区信息
	DesIdxData *m_pDesIdxData = (DesIdxData *)(pDesData + m_pDesHeader->classDataOffset_r);
	int i = 0;
	unsigned int *uTmp = (unsigned int *)m_pDesIdxData;

	unsigned int m_DesMethodCount = m_pDesHeader->classDataSize_r / (sizeof(DesIdxData));	//	这个可以计算出Method数量
	for (i = 0; i < m_pDesHeader->classDataSize_r / 4; i ++)
	{
		uTmp[i] = _byteswap_ulong(uTmp[i]);
	}
	for (i = 0; i < m_DesMethodCount; i ++)
	{
		DexCode *pDexCode = (DexCode *)(m_pDesInsntData + m_pDesIdxData[i].InsntOffset);

		if (m_pDesIdxData[i].DebugInfoOff >= 0x10000000)	//	如果是大于0x10000000则进行解密
		{
			void *pNewBuf = NULL;
			Bangcle_ResClass(m_pDesIdxData[i].DebugInfoOff, m_pDesInsntData + m_pDesIdxData[i].InsntOffset, m_pDesIdxData[i].InsntSize, &pNewBuf);
			if (pNewBuf)
			{
				memcpy(m_pDesInsntData + m_pDesIdxData[i].InsntOffset, pNewBuf, m_pDesIdxData[i].InsntSize);
				free(pNewBuf);
			}
		}
	}
	return false;
}
/*
************************************************************
*				BangBangFixDex
* 修复数据
************************************************************
*/
bool BangBangFixDex(unsigned char *pDesData, size_t len)
{
	int i = 0;
	int j = 0;
	int nClassIdx = 0;
	int count = MDex->pClassDefs->classCode.GetSize();
	DEXLOG("classCode = %08x", count);
	MDex->pClassDefs->classCode.Reset();
	for (nClassIdx = 0; nClassIdx < count; nClassIdx ++)
	{
		item_ClassCode* code =MDex->pClassDefs->classCode.Get();
		DexCode* pCode = (DexCode *)code->data;
		if (pCode->debugInfoOff >= 0x10000000)
		{
			DEXLOG("debugInfoOff: %p ",pCode->debugInfoOff);
			int nDatalen = 0;
			DexCode *pDexCode = GetMethodData(pDesData, len, pCode->debugInfoOff, nDatalen);
			if (pDexCode && nDatalen)
			{
				char* mCode = code->DexOffset +(char*)MDex->pHeader->pHeader;
				DEXLOG("pCode: %p ,pDexCode: %p ",pCode,pDexCode);
				DEXLOG("code->DexOffset: %p",code->DexOffset);
				DEXLOG("code->pHeader: %p",(char*)MDex->pHeader->pHeader);
				DEXLOG("mCode: %p",mCode);
				pDexCode->debugInfoOff = 0;
				memcpy(mCode, pDexCode, 0x10 + 2 * pCode->insnsSize);
			}
			pCode->debugInfoOff = 0;
		}
		MDex->pClassDefs->classCode.Next();
	}
	DEXLOG("BangBangFixDex Over");
	return true;
}

/*
************************************************************
*				StartDump
* 开始进行Dump处理
* Hook关键点
* $JNI_OnLoad -> 获取GVM
* $dexFileParse -> 获取记载Dex
************************************************************
*/
void StartDump(char* inName)
{
	SubHook(libdvm,dvm_dexFileParse,(void*)&New$dexFileParse,(void**)&$dexFileParse);
//	SubHook(libdvm,dvm_ResolveClass,(void*)&New$dvmResolveClass,(void**)&$dvmResolveClass);
}
