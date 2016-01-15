/*
 * ModMemory.cpp
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include "Common.H"
#include "ModMemory.h"
/*
************************************************************
*				Szie
*获取整个内存大小
************************************************************
*/
uint ModMemory::Size()
{
	size_t mLength = 0;
	item_Mem* mMem = Memory;
	while(mMem != NULL){
		mLength += mMem->Length;
		mMem = mMem->next;
	}
	return mLength;
}
/*
************************************************************
*根据模块名字过获取子模块指针
************************************************************
*/
item_Mem* ModMemory::getByName(const char* inName){
	item_Mem* mMem = Memory;
	while(mMem != NULL){
		if(mMem->name != NULL){
			if(strcmp(mMem->name,inName) == 0){
				return mMem;
			}
		}
		mMem = mMem->next;
	}
	return NULL;
}
/*
************************************************************
*获取最后一个内存模块列表
************************************************************
*/
item_Mem* ModMemory::Last()
{
	item_Mem* mMem = Memory;
	if(mMem == NULL)return NULL;
	while(mMem->next != NULL){
		mMem = mMem->next;
	}
	return mMem;
}
/*
************************************************************
*新建一个内存模块，并且将这内存模块添加到尾部
************************************************************
*/
item_Mem* ModMemory::newMem(size_t in_Length)
{
	item_Mem* mMem;
	//判断是否是第一次设置内存快
	//将偏移地址设置为之前宽度
	if(Memory == NULL){
		Memory = new item_Mem();
		mMem = Memory;
		mMem->Offset = 0;
	}else{
		mMem = Last();
		mMem->next = new item_Mem();
		mMem = mMem->next;
		mMem->Offset = Size();
	}
	//设置地址
	mMem->Addr = (unsigned char*)malloc(in_Length);
	memset(mMem->Addr,0,in_Length);
	mMem->Length = in_Length;
	return mMem;
}
/*
************************************************************
*新建一个内存模块，并且将这内存模块添加到尾部
************************************************************
*/
item_Mem* ModMemory::newMem(void* inAddr,size_t in_Length)
{
	item_Mem* mMem = newMem(in_Length);
	memcpy(mMem->Addr,inAddr,in_Length);
	return mMem;
}
/*
************************************************************
*新建一个内存模块，并且将这内存模块添加到尾部
************************************************************
*/
item_Mem* ModMemory::newNameMem(const char* inName,size_t in_Length)
{
	item_Mem* mMem = newMem(in_Length);
	//设置名字
	mMem->name = strdup(inName);
	return mMem;
}
/*
************************************************************
*保存成文件
************************************************************
*/
void ModMemory::Save_ByFILE(char* appName,char* inFile){
	//return;
	char* filePath = (char*)malloc(1024);
	memset(filePath,0,1024);
	sprintf(filePath,"/sdcard/DumpDex/%s_%s",inFile,appName);
	FILE* mfd=fopen(filePath,"wb");
	//查找可写文件路径
	if(!mfd){
		DEXLOG("[Dump]Fail SD Path:%s",filePath);
		DEXLOG("[Dump]SD Open file fail!");
		memset(filePath,0,1024);
		if(appName != NULL){
			sprintf(filePath,"/data/data/%s/%s",appName,inFile);
			mfd=fopen(filePath,"wb");
			if(!mfd){
				DEXLOG("[Dump]Fail SD Path:%s",filePath);
				DEXLOG("[Dump]SD Open file fail!");
				free(filePath);
				return;
			}
		}else{
			DEXLOG("[Dump]Fail appName Is Null");
			free(filePath);
			return ;
		}
	}
	item_Mem* mMod = Memory;
	while(mMod != NULL){
		if(mMod->name != NULL){
			DEXLOG("[Dump]正在下载:%s",mMod->name);
		}else{
			DEXLOG("[Dump]正在下载:NULL");
		}
		fwrite(mMod->Addr,1,mMod->Length,mfd);
		mMod = mMod->next;
	}
	//释放至于资源
	fclose(mfd);
	DEXLOG("[Dump]Dump File Path:%s",filePath);
	free(filePath);
}
/*
************************************************************
*获取有多少个内存块
************************************************************
*/
size_t ModMemory::getCountBlock(){
	size_t mCount = 0;
	item_Mem* mMod = Memory;
	while(mMod != NULL){
		mCount++;
	}
	return mCount;
}
/*
************************************************************
*获取有多少个内存块
************************************************************
*/
static void ModMemory::Save_BySVC(char* appName,char* inFile,void* inAddr,size_t inLen)
{
	char* filePath = (char*)malloc(1024);
	memset(filePath,0,1024);
	sprintf(filePath,"/sdcard/DumpDex/%s_%s",inFile,appName);
	int fd = syscall(5,filePath,O_RDWR|O_CREAT);
	//查找可写文件路径
	if(fd == -1){
		DEXLOG("[Dump]Fail SD Path:%s",filePath);
		memset(filePath,0,1024);
		if(appName != NULL){
			sprintf(filePath,"/data/data/%s/%s",appName,inFile);
			fd = syscall(5,filePath,O_RDWR|O_CREAT);
			if(fd == -1){
				DEXLOG("[Dump]Fail data Path:%s",filePath);
				free(filePath);
				return;
			}
		}else{
			DEXLOG("[Dump]Fail appName Is Null");
			free(filePath);
			return ;
		}
	}
	syscall(4,fd,inAddr,inLen);
	syscall(6,fd);
	DEXLOG("[Dump]File Path:%s",filePath);
	free(filePath);
}
//
ModMemory::ModMemory()
{
	// TODO Auto-generated constructor stub
	Memory = NULL;

}

ModMemory::~ModMemory()
{
	// TODO Auto-generated destructor stub
}

