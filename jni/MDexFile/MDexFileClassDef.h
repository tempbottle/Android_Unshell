/*
 * MDexFileClassDef.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef MDEXFILECLASSDEF_H_
#define MDEXFILECLASSDEF_H_
//
typedef struct item_ClassDef{
	u4  classIdx;           /* index into typeIds for this class */
	u4  accessFlags;
	u4  superclassIdx;      /* index into typeIds for superclass */
	u4  interfacesOff;      /* file offset to DexTypeList */
	u4  sourceFileIdx;      /* index into stringIds for source file name */
	u4  annotationsOff;     /* file offset to annotations_directory_item */
	u4  classDataOff;       /* file offset to class_data_item */
	u4  staticValuesOff;    /* file offset to DexEncodedArray */
}item_ClassDef;
typedef struct DexClassCode{
	u2	reg_size;
	u2	inarg_size;
	u2 	outarg_size;
	u2 	try_size;
	u4	debug_info;
	u4	code_size;
}DexClassCode;

//
typedef struct item_ClassField{
	uint ids;
	uint flags;
}item_ClassField;
typedef struct item_ClassMethod{
	uint ids;
	uint flags;
	uint Offset;
	uint method_ids;
}item_ClassMethod;
typedef struct item_ClassData{
	uint DexOffset;
	uint FileOffset;
	uint MemSize;

	MeVertor<item_ClassField> Static;
	MeVertor<item_ClassField> Instance;
	MeVertor<item_ClassMethod>  Direct;
	MeVertor<item_ClassMethod>  Virtual;
}item_ClassData;
//
typedef struct item_ClassCode{
	uint DexOffset;
	uint FileOffset;
	uint MemSize;
	uint method_ids;
	void* data;
}item_ClassCode;


class MDexFile_ClassDef {
public:
	//
	MeVertor<item_ClassDef>  classDef;
	MeVertor<item_ClassData>  classData;
	MeVertor<item_ClassCode>  classCode;
	MDexFile* pMDex = NULL;
	//
	size_t getCodeLength(uint inCodeOff);
	item_ClassCode* initItem_ClassCode(uint inIdx,uint inCodeOff);
	uint getCodeFileOffset(uint inDexOffset);
	uint getDataFileOffset(uint inDexOffset);
	item_ClassMethod* getClassMethod(uint inMethodIdx);
	void Reset_ClassCode();
	void Clear_DebugInfo();
//
	item_ClassData* initArray_ClassData(uint inAddr);
	uint getClassDataMethod(item_ClassData* inItem,uint inNo);
	//
	void initMemory_IDS(ModMemory* inMemory);
	void initMemory_Data(ModMemory* inMemory);
	void initMemory_Code(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
	//
	MDexFile_ClassDef(MDexFile* inMDex);
};

#endif /* MDEXFILECLASSDEF_H_ */
