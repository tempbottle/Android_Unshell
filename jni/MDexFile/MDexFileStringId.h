/*
 * MDexFileStringId.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */
#ifndef MDEXFILESTRINGID_H_
#define MDEXFILESTRINGID_H_
#include "MDexFile.h"
/*
 *	IdsValue -> 解析时保存的索引值
 *	segshift -> 段偏移，表示距离StringCode距离
 *	stringSize -> 表示字符数据长度
 *	string 	-> 字符数据
 */
typedef struct item_Dexstring{
//基本结构说明
	uint DexOffset;
	uint FileOffset;
	uint stringSize;
	char* string;
}item_Dexstring;


class MDexFile_StringId {
public:
	MeVertor<item_Dexstring> stringIds;
	MDexFile* pMDex = NULL;
	//
	void* getStringCodeByOffset(uint inOffset);
	char* getStringByIds(uint inStringIds);
	int getIds(char* inStr);
	//
	void ParseStringCode();
	size_t StringCodeSize();
	//
	void initMemory_IDS(ModMemory* inMemory);
	void initMemory_Code(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
	//构造函数
	MDexFile_StringId(MDexFile* inMDex);
};

#endif /* MDEXFILESTRINGID_H_ */
