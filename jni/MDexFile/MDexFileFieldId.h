/*
 * MDexFileFieldId.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef MDEXFILEFIELDID_H_
#define MDEXFILEFIELDID_H_
#include "MDexFile.h"
//由于FieldIds数据和位置无关，只要保存数据就可以了
typedef struct item_FieldIds{
	uint classIdx;           /* index into typeIds list for defining class */
	uint typeIdx;            /* index into typeIds for field type */
	uint nameIdx;            /* index into stringIds for field name */
}item_FieldIds;
class MDexFile_FieldId {
public:
	MeVertor<item_FieldIds> fieldIds;
	MDexFile* pMDex;
	//
	void initMemory_IDS(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
	//
	MDexFile_FieldId(MDexFile* inMDex);
};

#endif /* MDEXFILEFIELDID_H_ */
