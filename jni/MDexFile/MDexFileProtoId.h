/*
 * MDexFileProtoId.h
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#ifndef MDEXFILEPROTOID_H_
#define MDEXFILEPROTOID_H_
#include "MDexFile.h"
typedef struct item_ProtoIds{
	u4	shortyIdx;          /* index into stringIds for shorty descriptor */
	u4	returnTypeIdx;      /* index into typeIds list for return type */
	u4	parametersOff;      /* file offset to type_list for parameter types */
}item_ProtoIds;


class MDexFile_ProtoId {
public:
	//
	MeVertor<item_ProtoIds> protoIds;
	MDexFile* pMDEX;
	//
	void initMemory_IDS(ModMemory* inMemory);
	void toMemory(ModMemory* inMemory);
	//
	MDexFile_ProtoId(MDexFile* inMDex);
};

#endif /* MDEXFILEPROTOID_H_ */
