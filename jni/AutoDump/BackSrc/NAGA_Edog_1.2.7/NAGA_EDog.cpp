/*
 * NAGA_EDog.cpp
 * 脱壳说明：
 * 一.娜迦加固版本： 1.2.7
 *
 * 二.加固逻辑说明：
 *		1.程序使用动态加载，但是加载的DEX被挖空方法。
 *		2.动态注册修复函数
 *		3.在程序类的<clinit>下修复，修复函数为静态函数所以必须使用参数区分修复项。
 *
 * 三.脱壳逻辑声明：
 *		1.程序使用反射来对程序解密
 *		2.等待加固SO加载,必须运行JNI_OnLoad（动态注册）
 *		3.然后使用JAVA Hook 直接Hook Lcom/edog/Elibrary;->i1(Ljava/lang/String;)V
 *		4.在自定义中将全部数据修复
 *		5.Dump DEX
 *
 *  Created on: 2015年12月11日
 *      Author: LsMouse
 */
#include "NAGA_EDog.H"
#include <sys/mman.h>
#include "Utils/Utils.H"
#include "SubHook/SubHook.H"
#include "MDexFile/MDexFile.h"
//
static char* AppName = NULL;
static JavaVM* GVM = NULL;
static MDexFile* mdex = NULL;
static bool isRepair = false;
/**
 * 		isTarget
 *	判断是否为目标进程
 *	这里只是判断加固SO是否匹配
 *	版本这里不会进行比较
 */
bool isTarget(char* inName)
{
	if(strstr(inName,LIB_NAGEEDOG) != NULL)
	{
		return true;
	}
	return false;
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
 *					MatchCode
 * 匹配数据
 * 用来匹配虚拟机指令
 *		invoke-static                   {}, <ref ELibrary.getLibrary() imp. @ _def_ELibrary_getLibrary@L>
 *		move-result-object              v0
 *		const-string                    v1, aNte0 # "NTE0"
 *		invoke-virtual                  {v0, v1}, <void ELibrary.i1(ref) imp. @ _def_ELibrary_i1@VL>
 */
int MatchCode[0x12] ={	0x71,0x00,0x100,0x100,0x00,0x00,
						0x0C,0x00,
						0x1A,0x01,0x100,0x100,
						0x6E,0x20,0x100,0x100,0x10,0x00};
/**
 * 				GetNaga_JavaIds
 *  根据clinit指令来获取 调用的stringIds索引
 */
uint GetNaga_JavaIds(uint inCodeOff)
{
	if(inCodeOff == 0)return 0;
	uint mIds = 0;
	char* mAddr = (char*)((uint)mdex->pBackHeader + inCodeOff + 0x10);
	for(int m_i = 0;m_i < sizeof(MatchCode);m_i++)
	{
		char m_data = *(mAddr+m_i);
		if((MatchCode[m_i] != 0x100)&&(m_data != MatchCode[m_i]))
		{
			DEXLOG("m_i %02x MatchCode[m_i] %02x m_data %02x",m_i,MatchCode[m_i],m_data);
			return 0;
		}
		if(m_i == 10)
		{
			mIds = m_data;
		}else if(m_i == 11){
			mIds += m_data<< 8;
			break;
		}
	}
	return mIds;
}
//-------------------------------------------------------------------------------------------------------
void (*$EDog_i1)(JNIEnv *env,jobject obj, jstring inJstring);
//将传入参数，直接解密
static void NAGARestoreCode(JNIEnv* env,jobject obj,char* pstr){
	char* mstr = (char*)malloc(1024);
	//组建字符串
	memset(mstr,0,1024);
	strcpy(mstr,pstr);
	DEXLOG("RestoreCode %s ", mstr);
	jstring mJsrt = Utils::pcharToJstring(env,mstr,NULL);
	$EDog_i1(env,obj,mJsrt);
	free(mstr);
	env->DeleteLocalRef(mJsrt);
}
//
void new$EDog_i1(JNIEnv *env,jobject obj, jstring inJstring){
	if(isRepair)return;
	isRepair = true;
	char* inStr = Utils::jstringTostring(env,inJstring);
	DEXLOG("new$EDog_i1 %p %p %s ",env,inJstring,inStr);
	char* mName = DexUtils::GetTimeName("Resolve_Before.dex");
	DexUtils::SaveFile(mdex->LoadAddr,mdex->Length*0x10,AppName,"Resolve_Before.dex");
	free(mName);

//开始还原全部的，读取ClassDef中获取method
//method的name 获取 是否为clinit
//是则直接获取code off 然后获取字符串
	if(NULL == mdex)
	{
		DEXLOG("new$EDog_i1 mdex is NULL, exit");
		return ;
	}
	//获取clinit字符串索引值
	uint mClinitIds = mdex->pStringIds->getIds("<clinit>");
	DEXLOG("new$EDog_i1 mClinitIds 0x%08x",mClinitIds);
	//直接进行修复
	for(int m_i = 0;m_i < mdex->getClassDefSize();m_i++)
	{
		item_ClassDef* mItemDef = mdex->pClassDefs->classDef.Get();
		if((mItemDef != NULL)&&(mItemDef->classDataOff != 0))
		{
			item_ClassData* mData = mdex->pClassDefs->initArray_ClassData(mItemDef->classDataOff);
			if(mData->Direct.GetSize() != 0)
			{
				uint mMethodIds = mData->Direct.Get(0)->ids;
				item_DexMethodIds* mMethod =  mdex->pMethodIds->methodIds.Get(mMethodIds);
				if(mMethod != NULL)
				{
					uint mStringIds = mMethod->nameIdx;
					if(mClinitIds == mStringIds)
					{
						//直接获地址 判断是否是还原指令，还原指令则直接提取
						uint mCodeOffet = mData->Direct.Get(0)->Offset;
						uint mIds = 0;
						mIds = GetNaga_JavaIds(mCodeOffet);
						DEXLOG("mMethodIds : 0x%08x 0x%08x 0x%08x",mMethodIds,mCodeOffet,mIds);
						if(mIds != 0)
						{
							char* mString =mdex->pStringIds->getStringByIds(mIds);
							//指令还原
							NAGARestoreCode(env,obj,mString);
							//清空代码
							if(0 != mCodeOffet)
							{
								void* mCode = (void*)(mCodeOffet+(char*)mdex->pBackHeader+0x10);
								mprotect(mCode,0x1000, PROT_READ | PROT_WRITE);
								memset(mCode,0x0,0x12);
							}

						}
					}
				}
			}
		}
	}
	mName = DexUtils::GetTimeName("Resolve_Before.dex");
	DexUtils::SaveFile(mdex->LoadAddr,mdex->Length*0x10,AppName,"Resolve_After.dex");
	free(mName);
}
/**
 *
 *
 */
void Hook_EDog_i1()
{
	JNIEnv* env = NULL;
	if(GVM == NULL)
	{
		DEXLOG("Hook_EDog_i1 GVM is NULL ");
		return ;
	}
	if($EDog_i1 != NULL)return ;
	if(GVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){
		DEXLOG("Hook_EDog_i1 GetEnv fail！");
		return ;
	}
	jclass cls_protect = findAppClass(env,"com/edog/ELibrary");
	if(cls_protect == NULL){
		DEXLOG("Hook_EDog_i1 com/edog/ELibrary no find！");
		return ;
	}
	jmethodID method_enc = env->GetMethodID(cls_protect,"i1","(Ljava/lang/String;)V");
	if(NULL == method_enc)
	{
		DEXLOG("Hook_EDog_i1 or method_enc no find！");
		return ;
	}
	MSJavaHookMethod(env,cls_protect,method_enc,(void*)&new$EDog_i1,(void**)&$EDog_i1);
	DEXLOG("Hook_EDog_i1 is oVER！");
	return ;
}
/*
************************************************************
*				$new$JNI_OnLoad
*用来Hook来进行获取JavaVM
@JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
************************************************************
*/
void* (*$JNI_OnLoad)(JavaVM* vm, void *reserved) = NULL;
void* new$JNI_OnLoad(JavaVM* vm, void *reserved)
{
	//通过Hook获取到JavaVM，保存JavaVM
	DEXLOG("new$JNI_OnLoad GVM is get!");
	GVM = vm;
	return $JNI_OnLoad(vm,reserved);
}
/*
************************************************************
*				$new$JNI_OnLoad
*这里使用 Hook进行获取数据
@void* dexFileParse(char* inAddr,size_t length, int flags)
************************************************************
*/
void* (*$dexFileParse)(char* inAddr,size_t length, int flags) = NULL;
void* new$dexFileParse(char* inAddr,size_t length, int flags)
{
	char* mname = DexUtils::GetTimeName("Parse_Before.dex");
	DexUtils::SaveFile(inAddr,length,AppName,mname);
	free(mname);
	void* result =$dexFileParse(inAddr,length,flags);
	DEXLOG("new$dexFileParse %p %p ", inAddr,length);
	mdex = new MDexFile(inAddr,length);
	mdex->Parse();
	Hook_EDog_i1();
	return result;
}
/*
************************************************************
*				StartDump
* 开始进行Dump处理
* Hook关键函数点进行定点
************************************************************
*/
void StartDump(char* inPath)
{
	//直接进行Hook
	if($dexFileParse == NULL)
	{
		SubHook(libdvm,dvm_dexFileParse,(void*)&new$dexFileParse,(void**)&$dexFileParse);
	}
	//直接将类加载进去，然后Hook JNI_OnLoad
	dlopen(inPath,RTLD_LAZY);
	if(NULL == $JNI_OnLoad)
	{
		SubHook(inPath,Final_JNI_OnLoad,(void*)&new$JNI_OnLoad,(void**)&$JNI_OnLoad);
	}
}




