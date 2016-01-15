/*
 * MDexFileDebug.h
 *
 *  Created on: 2015年12月24日
 *      Author: LsMouse
 */

#ifndef MDEXFILEDEBUG_H_
#define MDEXFILEDEBUG_H_
#include "MDexFile.h"


class MDexFile_Debug {
public:
	uint DexOffset;
	uint FileOffset;
	uint MemSize;
	char* data;
	MDexFile* pMDex = NULL;
	//构造函数
	MDexFile_Debug(MDexFile* inMDex);

};

#endif /* MDEXFILEDEBUG_H_ */
