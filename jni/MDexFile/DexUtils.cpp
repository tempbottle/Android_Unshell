/*
 * DexUtils.cpp
 *
 *  Created on: 2015年12月4日
 *      Author: LsMouse
 */

#include "DexUtils.h"
/*
************************************************************
*			readUnsignedLeb128
*	读取无符号leb128数据
************************************************************
*/
static int DexUtils::readUnsignedLeb128(void** pStream) {
	u_char* ptr = *pStream;
	int result = *(ptr++);
	if (result > 0x7f) {
		int cur = *(ptr++);
		result = (result & 0x7f) | ((cur & 0x7f) << 7);
		if (cur > 0x7f) {
			cur = *(ptr++);
			result |= (cur & 0x7f) << 14;
			if (cur > 0x7f) {
				cur = *(ptr++);
				result |= (cur & 0x7f) << 21;
				if (cur > 0x7f) {
					/*
					 * Note: We don't check to see if cur is out of
					 * range here, meaning we tolerate garbage in the
					 * high four-order bits.
					 */
					cur = *(ptr++);
					result |= cur << 28;
				}
			}
		}
	}
	*pStream = ptr;
	return result;
}
/*
************************************************************
*			readSignedLeb128
*	读取有符号leb128数据
************************************************************
*/
static int DexUtils::readSignedLeb128(void** pStream) {
	u_char* ptr = *pStream;
	int result = *(ptr++);
    if (result <= 0x7f) {
        result = (result << 25) >> 25;
    } else {
        int cur = *(ptr++);
        result = (result & 0x7f) | ((cur & 0x7f) << 7);
        if (cur <= 0x7f) {
            result = (result << 18) >> 18;
        } else {
            cur = *(ptr++);
            result |= (cur & 0x7f) << 14;
            if (cur <= 0x7f) {
                result = (result << 11) >> 11;
            } else {
                cur = *(ptr++);
                result |= (cur & 0x7f) << 21;
                if (cur <= 0x7f) {
                    result = (result << 4) >> 4;
                } else {
                    /*
                     * Note: We don't check to see if cur is out of
                     * range here, meaning we tolerate garbage in the
                     * high four-order bits.
                     */
                    cur = *(ptr++);
                    result |= cur << 28;
                }
            }
        }
    }
    *pStream = ptr;
    return result;
}
/*
************************************************************
*			writeUnsignedLeb128
*	写无符号leb128数据
************************************************************
*/
static void* DexUtils::writeUnsignedLeb128(void* inptr, unsigned int data)
{
	unsigned char *ptr = (unsigned char *)inptr;
	while (true)
	{
		unsigned char out = data & 0x7f;
		if (out != data)
		{
			*ptr++ = out | 0x80;
			data >>= 7;
		} else {
			*ptr++ = out;
			break;
		}
	}
	return (void*)ptr;
}

/*
************************************************************
*			NEXTLeb128
* @param	inAddr -> 数据起始地址
* @return	result -> 返回下一个数据地址
************************************************************
*/
static u_char* DexUtils::NEXTLeb128(void* inAddr){
	u_char* in_Addr =(u_char*)inAddr;
	int m_i;
	for(m_i = 0;m_i < 5;m_i++){
		if(*(in_Addr+m_i) <= 0x7f){
			return in_Addr+m_i+1;
		}
	}
	return in_Addr+m_i+1;
}
/*
************************************************************
*			readLeb128
* @param	inAddr ->	数据起始地址
* @return	result ->	读取readLeb128(unsigned leb128)数据
************************************************************
*/
static int DexUtils::readLeb128(void* inAddr){
	u_char* in_Addr = (u_char*)inAddr;
	int result = 0;
	for(int m_i = 0;m_i < 5;m_i++){
		u_char m_byte = *(in_Addr + m_i);
		result |= (m_byte&0x7F)<<(m_i*7);
		if(m_byte <= 0x7f){
			return result;
		}
	}
	return result;
}
/*
************************************************************
*			readSLeb128
* @param	inAddr ->	数据起始地址
* @return	result ->	读取readLeb128(signed leb128)数据
************************************************************
*/
static uint DexUtils::readSLeb128(u_char* inAddr){
	uint	result_2 = 0xFFFFFFFF;
	uint	result_1 = 0x00000000;
	for(int m_i = 0;m_i < 5;m_i++){
		char m_byte = *(inAddr + m_i);
		result_2 &= ~(0x7F<<(m_i*7));
		result_1 |= (m_byte&0x7F)<<(m_i*7);
		result_2 |= (m_byte&0x7F)<<(m_i*7);
		if((m_byte & 0x80) == 0)
		{
			if((m_byte & 0x40)){
				return result_2;
			}
			return result_1;
		}
	}
	return result_1;
}
/*
************************************************************
*			readLeb128
* @param	inAddr ->	数据起始地址
* @param	inDeta ->	数据指针—存放数据
* @return	result ->	返回下一个数据地址
************************************************************
*/
static u_char* DexUtils::readLeb128(void* inAddr,uint* inDeta){
	*inDeta = readLeb128(inAddr);
	return NEXTLeb128(inAddr);
}
/*
************************************************************
*			readSLeb128
* @param	inAddr ->	数据起始地址
* * @param	inDeta ->	数据指针—存放数据
* @return	result ->	返回下一个数据地址
************************************************************
*/
static u_char* DexUtils::readSLeb128(u_char* inAddr,uint* inDeta){
	*inDeta = readSLeb128(inAddr);
	return NEXTLeb128(inAddr);
}
/*
************************************************************
*			readLimitLeb128
* @param	inAddr ->	数据起始地址
* @return	result ->	返回Leb128数据宽度
************************************************************
*/
static uint DexUtils::readLimitLeb128(unsigned char* inAddr){
	int m_i = 0;
	for(m_i = 0;m_i < 5;m_i++){
		if(*(inAddr+m_i) <= 0x7f){
			return  m_i+1;
		}
	}
	return  m_i+1;
}
/*
************************************************************
*			writeL128
* @param	inAddr ->	数据起始地址
* @param	mData ->	数据
* @return	result ->	返回Leb128数据宽度
************************************************************
*/
static u_char* DexUtils::writeL128(u_char* inAddr,uint mData){
	int m_i;
	for(m_i = 0;m_i< 5;m_i++){
		u_char mByte =	mData &0x7F;
		mData >>= 7;
		if(mData != 0){
			*(inAddr+m_i) = mByte|0x80;
		}else{
			*(inAddr+m_i) = mByte&0x7F;
			return NEXTLeb128(inAddr);
		}
	}
	return NEXTLeb128(inAddr);
}
static u_char* DexUtils::writeSL128(u_char* inAddr,uint mData){
	if((mData&0x80000000) == 0){
		writeL128(inAddr,mData);
	}else{
		uint m_Data = 0xFFFFFFFF;
		int m_i;
		for(m_i = 0;m_i< 5;m_i++){
			u_char mByte = (mData>>(m_i*7));
			m_Data >>= 7;
			mData >>= 7;
			if((m_Data^mData) == 0){
				*(inAddr+m_i) = mByte&0x7F;
				return NEXTLeb128(inAddr);

			}else{
				*(inAddr+m_i) = mByte|0x80;
			}
		}
	}
	return NEXTLeb128(inAddr);
}
/*
************************************************************
*			writeAdd128_Limit
*会根据原先数据宽度设置数据
* @param	inAddr ->	数据起始地址
* @param	mData ->	数据
* @return	result ->	返回Leb128数据宽度
************************************************************
*/
static int DexUtils::writeAdd128_Limit(u_char* inAddr,uint inData){
	uint mLength = readLimitLeb128(inAddr);
	for(int m_i = 0;m_i< mLength;m_i++){
		u_char mByte = (inData>>(m_i*7))&0x7F;
		if((m_i+1)!= mLength){
			*(inAddr+m_i) = mByte|0x80;
		}else{
			*(inAddr+m_i) = mByte;
		}
	}
	return 0;
}
/*
************************************************************
*			Alloc
*获取已清零内存
* @param	inSize ->	内存宽度
* @return	out ->		内存指针
************************************************************
*/
static void* DexUtils::Alloc(size_t inSize){
	void* out = malloc(inSize);
	memset(out,0,inSize);
	return out;
}
/*
************************************************************
*			Align
*对齐设置
* @param	inAddr ->	结束地址
* @param	inAli ->	对齐宽度
* @return	out ->		对齐需要宽度
************************************************************
*/
static size_t DexUtils::Align(size_t inAddr,size_t inAli){
	size_t mSzie = inAli - inAddr&(inAli-1);
	return  mSzie;
}

/*
************************************************************
*			AlignAdd
*对齐设置
* @param	inAddr ->	结束地址
* @param	inAli ->	对齐宽度
* @return	out ->		对齐需要宽度
************************************************************
*/
static size_t DexUtils::AlignAdd(size_t inAddr,size_t inAli){
	size_t mSzie = (inAddr+inAli-1)&(~(inAli-1));
	return  mSzie;
}
static size_t DexUtils::Align_4(size_t inAddr){
	size_t mSzie = (inAddr+3)&(~3);
	return  mSzie;
}
/*
************************************************************
*			isDex
*判断是否是Dex文件
* @param	inAddr ->	Dex起始地址
* @return	bool ->		0->不是Dex或ODex文件
* 						1->存在Dex或ODex的Magic
************************************************************
*/
static bool DexUtils::isDex(void* inAddr){
	if((0 == memcmp(inAddr,"dex",3))||(0 == memcmp(inAddr,"dey",3))){
		return true;
	}
	return false;
}
/*
************************************************************
*			SaveFile
*将内存数据保存成文件
* @param	addr ->		内存起始地址
* @param	inLen->		内存宽度
* @param	appName ->	应用名字
* @param	inFile ->	文件名字
************************************************************
*/
static void DexUtils::SaveFile(void* addr,size_t inLen,char* appName,char* inFile){
	char* filePath = (char*)malloc(1024);
	memset(filePath,0,1024);
	sprintf(filePath,"/sdcard/DumpDex/%s_%s",inFile,appName);
	FILE* mfd=fopen(filePath,"wb");
	//查找可写文件路径
	if(!mfd){
		memset(filePath,0,1024);
		if(appName != NULL){
			sprintf(filePath,"/data/data/%s/%s",appName,inFile);
			mfd=fopen(filePath,"wb");
			if(!mfd){
				free(filePath);
				return;
			}
		}else{
			free(filePath);
			return ;
		}
	}
	fwrite(addr,1,inLen,mfd);
	fclose(mfd);
	free(filePath);
}
/*
************************************************************
*			GetTimeName
*获取实时时间名字
* @param	inName ->	名字前缀
************************************************************
*/
static char*  DexUtils::GetTimeName(char* inName){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	int m_HookS = tv.tv_sec%60;
	int m_HookM = (tv.tv_sec/60)%60;
	int m_HookH = (tv.tv_sec/3600)%24;
	char* mName = (char*)DexUtils::Alloc(1024);
	sprintf(mName,"%s_%02d_%04d",inName,m_HookS,tv.tv_usec);
	return mName;
}
/*
************************************************************
*				GetBase
* 获取Dex头如果是odex开始，则偏移0x28位
************************************************************
*/
static void* DexUtils::GetBase(void* inAddr){
	u_char* mAddr =(u_char*)inAddr;
	if(0 == memcmp(mAddr,"dey",3)){
		return mAddr+0x28;
	}else if(0 == memcmp(mAddr,"dex",3)){
		return mAddr;
	}
	return NULL;
}
/*
************************************************************
*
*
************************************************************
*/
