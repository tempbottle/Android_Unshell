/*
 * ModMemory.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef MODMEMORY_H_
#define MODMEMORY_H_

#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

typedef struct item_Mem{
	struct item_Mem* next;		//下一个内存分割模块
	unsigned char*	Addr;		//数据内存地址
	unsigned char*	LoadBase;	//加载基本地址
	char* 	name;				//内存名字
	size_t 	Length;				//内存宽度
	size_t	Offset;				//文件偏移地址
	size_t	Base;				//
}item_Mem;


class ModMemory {
public:
	item_Mem* Memory;
//
	uint Size();
	item_Mem* getByName(const char* inName);
	item_Mem* Last();
	item_Mem* newMem(size_t in_Length);
	item_Mem* newMem(void* inAddr,size_t in_Length);
	item_Mem* newNameMem(const char* inName,size_t in_Length);
	size_t getCountBlock();

	void Save_ByFILE(char* appName,char* inFile);
	static void Save_BySVC(char* appName,char* inFile,void* inAddr,size_t inLen);

//
	ModMemory();
	virtual ~ModMemory();
};

#endif /* MODMEMORY_H_ */
