/*
 * DexUtils.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef DEXUTILS_H_
#define DEXUTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
//定义Memory 名字
#define MemDex_OPTHeader 		"OPTHeader"
#define MemDex_OPTData			"OPTData"
#define MemDex_Header 			"Header"
#define MemDex_StringIds		"StringIDS"
#define MemDex_StringCode		"StringCode"
#define MemDex_TypeIds			"TypeIDS"
#define MemDex_TypeList			"TypeList"
#define MemDex_ProtoIds			"ProtoIDS"
#define MemDex_FieldIds 		"FieldIDS"
#define MemDex_MethoIds			"MethoIDS"
#define MemDex_ClassDef			"ClassDef"
#define MemDex_ClassData		"ClassData"
#define MemDex_ClassCode		"ClassCode"
#define MemDex_EncodeArray		"EncodeArray"
#define MemDex_Annot			"Annot"
#define MemDex_AnnotSet			"AnnotSet"
#define MemDex_AnnotSRL			"AnnotSRL"
#define MemDex_AnnotDIR			"AnnotDIR"
#define MemDex_MAP				"Map"
//声明类
class MDexFile;
//
class DexUtils {
public:
	static int readUnsignedLeb128(void** pStream);
	static int readSignedLeb128(void** pStream);
	static void* writeUnsignedLeb128(void* inptr, unsigned int data);
	//
	static u_char* NEXTLeb128(void* inAddr);
	static int readLeb128(void* inAddr);
	static uint readSLeb128(u_char* inAddr);
	static u_char* readLeb128(void* inAddr,uint* inDeta);
	static u_char* readSLeb128(u_char* inAddr,uint* inDeta);
	static uint readLimitLeb128(unsigned char* inAddr);
	static u_char* writeL128(u_char* inAddr,uint mData);
	static u_char* writeSL128(u_char* inAddr,uint mData);
	static int writeAdd128_Limit(u_char* inAddr,uint inData);
	static void* Alloc(size_t inSize);
	static size_t Align(size_t inAddr,size_t inAli);
	static size_t AlignAdd(size_t inAddr,size_t inAli);
	static size_t Align_4(size_t inAddr);
	static bool isDex(void* inAddr);
	static void SaveFile(void* addr,size_t inLen,char* appName,char* inFile);
	static char* GetTimeName(char* inName);
	static void* GetBase(void* inAddr);
//
};

#endif /* DEXUTILS_H_ */
