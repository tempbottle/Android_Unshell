/*
 * SubHook.cpp
 *
 *  Created on: 2015年12月3日
 *      Author: LsMouse
 */
#include "stdio.h"
#include "SubHook.H"
/*
************************************************************
*				SubHook
*substate Hook工具，直接传入参数直接Hook
*对substate提供API进行封装
************************************************************
*/
int SubHook(char* inSoName,char* inSym, void *replace, void **result)
{
	MSImageRef image = MSGetImageByName(inSoName);
	if(NULL != image)
	{
		void* mFun = MSFindSymbol(image,inSym);
		if(NULL != mFun)
		{
			MSHookFunction(mFun,replace,result);
		}else
			return -1;
	}else
		return -1;
	return 0;
}
int SubHookAddr(void* inAddr, void *replace, void **result)
{
	if(inAddr != NULL)
	{
		MSHookFunction(inAddr,replace,result);
	}

	return  0;
}
