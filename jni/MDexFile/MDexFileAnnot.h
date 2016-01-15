/*
 * MDexFileAnnot.h
 *
 *  Created on: 2015年12月5日
 *      Author: LsMouse
 */

#ifndef MDEXFILEANNOT_H_
#define MDEXFILEANNOT_H_
#include "MDexFile.h"
//Annot
typedef struct item_AnnotOffset{
	uint Offset;
};
typedef struct item_Annot{
	uint DexOffset;
	uint FileOffset;
	uint MemSize;
	void* data;
}item_Annot;
//AnnotSET
typedef struct item_AnnotSet{
	uint DexOffset;
	uint FileOffset;
	uint MemSize;
	MeVertor<item_AnnotOffset> item;
}item_AnnotSet;
//AnnotSRL
typedef struct item_AnnotSRL{
	uint DexOffset;
	uint FileOffset;
	uint MemSize;
	MeVertor<item_AnnotOffset> item;
}item_AnnotSRL;
//
typedef struct item_AnnotDirItem{
	uint	Idx;
	uint	Offset_Annot;
}item_AnnotDirItem;


typedef struct item_AnnotDIR{
	uint DexOffset;
	uint FileOffset;
	uint MemSize;
	uint Offset_Class;
	uint Size_Feilds;
	uint Size_Methods;
	uint Size_Params;
	MeVertor<item_AnnotDirItem> item;
}item_AnnotDIR;
//


class MDexFile_Annot {
public:
	MeVertor<item_Annot> annot;
	MeVertor<item_AnnotSet> annotSet;
	MeVertor<item_AnnotSRL> annotSRL;
	MeVertor<item_AnnotDIR> annotDIR;
	MDexFile* pMDex;
//
	void* Element(void* inAddr,size_t inSize);
	uint getAnnotFileOffset(uint inDexOffset);
	uint getAnnotSetFileOffset(uint inDexOffset);
	uint getAnnotSRLFileOffset(uint inDexOffset);
	uint getAnnotDIRFileOffset(uint inDexOffset);
	uint getFileOffset(uint inDexOffset);

//
	void initMemory(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
//
	void Init_Annot(MDexFile* inMDex);
	void Init_AnnotSet(MDexFile* inMDex);
	void Init_AnnotSRL(MDexFile* inMDex);
	void Init_AnnotDIR(uint inoffset,size_t inSize);
//
	MDexFile_Annot(MDexFile* inMDex);


};

#endif /* MDEXFILEANNOT_H_ */
