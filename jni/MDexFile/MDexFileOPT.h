/*
 * MDexFileOPT.h
 *	主要保存Odex数据，将数据
 *  Created on: 2015年12月25日
 *      Author: LsMouse
 */

#ifndef MDEXFILEOPT_H_
#define MDEXFILEOPT_H_
#include "MDexFile.H"
class MDexFile_OPT {
public:
	DexOptHeader* optHeader;
	void* Odex_data;
	MDexFile* pMDex;
//
	void initMemory_IDS(ModMemory* inMemory);
	void initMemory_Code(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
//
	MDexFile_OPT(MDexFile* inMDex);


};

#endif /* MDEXFILEOPT_H_ */
