/*
 * Baidu Native.cpp
 *
 *  Created on: 2015年12月22日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <jni.h>
#include <pthread.h>
#include "SubHook/SubHook.H"
#include "Baidu_Native.H"
#include "Utils/Utils.H"
#include "MDexFile/MDexFile.h"
#include "sys/mman.h"
//声明变量
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static char* Dex_Parse = NULL;
static MDexFile* MDex;

/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	return true;
}
void SetAppName(void* inAppName)
{
	AppName = (char*)inAppName;
}
void SetJavaVM(void* inJavaVM)
{
	GVM = (JavaVM*)inJavaVM;
}
 /*
 ************************************************************
 *				New$JNI_OnLoad
 *用来Hook来进行获取JavaVM
 @JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
 ************************************************************
 */
 static void* (*$JNI_OnLoad)(JavaVM* vm, void *reserved) = NULL;
 static void* New$JNI_OnLoad(JavaVM* vm, void *reserved)
 {
 	//通过Hook获取到JavaVM，保存JavaVM
	DEXLOG("Baidu Native New$JNI_OnLoad");
	GVM = vm;



	return $JNI_OnLoad(vm,reserved);
 }
 static void Hook_Jni_OnLoad(char* inName)
 {
	dlopen(inName,RTLD_NOW);
 	SubHook(inName,Final_JNI_OnLoad,(void*)&New$JNI_OnLoad,(void**)&$JNI_OnLoad);
 }
 /*
 ************************************************************
 *				New$dexFileParse
 *用来Hook来进行获取JavaVM
 @JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
 ************************************************************
 */
void* (*$dexFileParse)(char* inAddr,size_t length, int flags) = NULL;
void* New$dexFileParse(char* inAddr,size_t length, int flags)
{
	void* result =$dexFileParse(inAddr,length,flags);
	DEXLOG("new$dexFileParse %p %p ", inAddr,length);
	MDex = new MDexFile(inAddr,length);
	MDex->Parse();
	MDex->SaveDex(AppName,"MDex");
	//Start Java Hook
	UnShell_Hook();
	return result;
}
/**/
void* SetBef_LoadNative(char* pathName)
{

	return pathName;
}
void* SetAft_LoadNative(char* pathName)
{
	return pathName;
}
/*
************************************************************
 *				getListByBaiduNative
 *
************************************************************
 */
int* getListByBaiduNative()
{
	void* mHandler = dlopen("libbaiduprotect.so",RTLD_NOW);
	if(NULL == mHandler)
	{
		DEXLOG("New$Native dlopen is Fail");
		return NULL;
	}
	void* mJni_OnLoad = dlsym(mHandler,Final_JNI_OnLoad);
	if(NULL == mJni_OnLoad)
	{
		DEXLOG("New$Native mJni_OnLoad is Fail");
		return NULL;
	}
//	int* mp_Native = (int*)((char*)mJni_OnLoad - 0x304D + 0x59BD4);
	int* mp_Native = (int*)((char*)mJni_OnLoad - 0x304D + 0x61DF4);
	mp_Native = (int*)(*mp_Native + 0x18);
	mp_Native = (int*)(*mp_Native + 0x24);
	if(*mp_Native == NULL)
	{
		DEXLOG("mp_Native is NULL");
	}
	mp_Native = *mp_Native;
	DEXLOG("New$Native mp_Native: %p",mp_Native);
	return mp_Native;
}
/*
************************************************************
 *					MatchCode
 * 匹配数据
 * 用来匹配虚拟机指令
 *		const                           v1, 0
 *		const                           v3, 1
 *		以下未匹配
 *		new-array                       v0, v3, <t: Object[]>
 *		const                           v3, 0
 *		check-cast                      p0, <t: Object>
 *		aput-object                     p0, v0, v3
 *		invoke-static                   {v1, this, v0}, <void A.V(int, ref, ref) imp. @ _def_A_V@VILL>
 *		locret:
 *		return-void
************************************************************
 */
int MatchCode[12] ={	0x14,0x01,0x100,0x100,0x100,0x100,
						0x14,0x03,0x01,0x00,0x00,0x00};
/**
 * 				GetNaga_JavaIds
 * 根据onCreate参数 获取到列表位置
 * -1:不匹配
 * Other: 链表位置
 */
int Get_JavaIds(char* inCode)
{
	int mIds = 0;
	//直接跳到指令
	inCode += 0x10;
	for(int m_i = 0;m_i < (sizeof(MatchCode)/sizeof(int));m_i++)
	{
		char m_data = *(inCode+m_i);
		if((MatchCode[m_i] != 0x100)&&(m_data != MatchCode[m_i]))
		{
		//	DEXLOG("m_i %02x MatchCode[m_i] %02x m_data %02x",m_i,MatchCode[m_i],m_data);
			return -1;
		}
		if(m_i == 2)
		{
			mIds = m_data;
		}else if(m_i == 3){
			mIds += m_data<< 8;
		}
		else if(m_i == 4){
			mIds += m_data<< 16;
		}
		else if(m_i == 5){
			mIds += m_data<< 24;
		}
	}
	return mIds;
}
/**
 * 				修复OpCode
 * 将OpCode修改成之前的
 */
void Res_OpCode(char* inCode,size_t inCodeLen)
{
	for(int m_i =  0;m_i < inCodeLen;)
	{
		int mBaidu_OpCode = *(inCode+m_i);
		int m_OpCode = Res_OpBaidu[mBaidu_OpCode];
		int m_OpLen = Res_OpLen[m_OpCode];
		if(m_OpLen == 0xFF)
		{
			DEXLOG("m_i: %p",m_i);
			DEXLOG("mBaidu_OpCode: %p",mBaidu_OpCode);
			DEXLOG("m_OpCode: %p",m_OpCode);
			DEXLOG("m_OpLen: %p",m_OpLen);
			return ;
		}
		*(inCode+m_i) = m_OpCode;
		m_i += m_OpLen;
	}
}
/*
************************************************************
*				New$Native
* 对解密程序进行Hook,然后获取到数据指针，直接修复代码
************************************************************
*/
void (*$Native)(JNIEnv*,jobject,jobject,jobject,jobject) = NULL;
void New$Native(JNIEnv *env, jobject obj,jint inList,jobject inthiz,jobject inNewObject)
{
	DEXLOG("New$Native inList:%d",inList);
	//这里获取到数据模式，
	int Ids_onCreate = MDex->pStringIds->getIds("onCreate");
	MeVertor<item_ClassCode> ClassCode = MDex->pClassDefs->classCode;
	//获取列表指针
	int* mp_Native = getListByBaiduNative();
	//进行Hook 获取原本数据地址
	//开始搜索代码，判断是否有代码匹配
	ClassCode.Reset();
	for(int m_i;m_i < ClassCode.GetSize();m_i++)
	{
		int mIds;
		item_ClassCode* mItem = ClassCode.Get();
		DexClassCode* mDexCode =mItem->data;
		mDexCode->debug_info = 0;
		if((mItem->MemSize == 0x38)&&
			((mIds = Get_JavaIds((char*)mItem->data))!= -1))
		{
			DEXLOG("FileOffset: %p",mItem->FileOffset);
		//	DEXLOG("mIds: %p mNative: %p",mIds,*(mp_Native+mIds));
			//尝试获取原本数据，然后开始进行修复
			//直接提取数据，将数据
			Baidu_NaInfo* mInfo = *(mp_Native+mIds);
			DEXLOG("Code: %p %p",mInfo->SizeCode,mInfo->Code);
			mItem->MemSize = DexUtils::Align_4(mInfo->SizeCode*2 + 0x10);
			void* m_Code = malloc(mItem->MemSize);
			void* m_ResCode =  malloc( DexUtils::Align_4(mInfo->SizeCode*2));
			memcpy(m_ResCode,mInfo->Code,mInfo->SizeCode*2);
			Res_OpCode((char*)m_ResCode,mInfo->SizeCode*2);

			memset(m_Code,0,mItem->MemSize);
			memcpy(m_Code,mItem->data,0x10);
			memcpy((char*)m_Code+0x10,m_ResCode,mInfo->SizeCode*2);
			//修正数据
			mDexCode =m_Code;
			mDexCode->debug_info = 0;
			mDexCode->reg_size = mInfo->SizeREG;
			mDexCode->code_size = mInfo->SizeCode;
			//用修复数据替换之前数据
			mItem->data = m_Code;
		}
		ClassCode.Next();
	}
	MDex->SaveDex(AppName,"MDex_NoRes");
//这里开始获取
	$Native(env,obj,inList,inthiz,inNewObject);
//
}
void UnShell_Hook()
{
	JNIEnv* env = NULL;
	if(NULL == GVM)
	{
		DEXLOG("Baidu UnShell_Hook GVM is NULL!");
		return ;
	}
	if(GVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		DEXLOG("Baidu UnShell_Hook GetEnv fail！");
		return ;
	}

	jclass cls_protect = env->FindClass("com/baidu/protect/A");
	if(cls_protect == NULL)
	{
		DEXLOG("Baidu UnShell com/baidu/protect/A class no find！");
		return ;
	}
	jmethodID method_native = env->GetStaticMethodID(cls_protect,"V","(ILjava/lang/Object;[Ljava/lang/Object;)V");
	if(method_native == NULL)
	{
		DEXLOG("Baidu UnShell method_native no find！");
		return ;
	}
	MSJavaHookMethod(env,cls_protect,method_native,(void*)&New$Native,(void**)&$Native);
}
/*
************************************************************
*				StartDump
* 开始进行Dump处理
* Hook关键点
* $JNI_OnLoad -> 获取GVM
* $dexFileParse -> 获取记载Dex
************************************************************
*/
void StartDump(char* inName)
{
	DEXLOG("Baidu Native StartDump");
	SubHook(libdvm,dvm_dexFileParse,(void*)&New$dexFileParse,(void**)&$dexFileParse);
	Hook_Jni_OnLoad(inName);
}
