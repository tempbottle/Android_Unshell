#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <ctype.h>
#include <jni.h>
#include "Common.H"
namespace Utils{
/*
************************************************************
*			getpidName
* 获取进程名字
************************************************************
*/
char* getpidName(pid_t inpid){
	char proc_pid_path[1024];
	char buf[1024];
	char* outname = NULL;
	memset(buf,0,1024);
	memset(proc_pid_path,0,1024);
	sprintf(proc_pid_path, "/proc/%d/status", inpid);
	FILE* fp = fopen(proc_pid_path, "r");
	if(NULL != fp){
		if( fgets(buf, 1023, fp)== NULL ){
			fclose(fp);
		}
		fclose(fp);
		outname = (char*)malloc(1024);
		memset(outname,0,1024);
		sscanf(buf, "%*s %s", outname);
	}
	return outname;
}
/*
************************************************************
*				trim
*	去控制符
************************************************************
*/
 void trim(char *s)
{
	int len = strlen(s);
	/* trim trailing whitespace */
	while ( len > 0 && isspace(s[len-1]))
	      s[--len]='\0';
	/* trim leading whitespace */
	memmove(s, &s[strspn(s, " \n\r\t\v")], len);
}
/*
************************************************************
*			getSelfCmdline
* 获取</proc/self/status> 内容
************************************************************
*/
char* getSelfCmdline(){
	FILE* fd =fopen("/proc/self/status","r");
	if(fd == NULL)return NULL;
	char* mBuf = (char*)malloc(1024);	
	memset(mBuf,0,1024);
	fgets(mBuf,1024,fd);
	trim(mBuf);
	return mBuf;
}
/*
************************************************************
*			getSelfName
* 获取</proc/self/status> 名字呢看内容
************************************************************
*/
char* getSelfName(){
	FILE* fd =fopen("/proc/self/status","r");
	if(fd == NULL)return NULL;
	char* mBuf = (char*)malloc(1024);	
	memset(mBuf,0,1024);
	fgets(mBuf,1024,fd);
	if((strlen(mBuf) == 0)||(strstr(mBuf,":") == NULL)){
		free(mBuf);
		return NULL;
	}
	char* mOut = strdup(strstr(mBuf,":")+1);
	free(mBuf);
	trim(mOut);
	return mOut;
}
/*
************************************************************
*				getStartAddrByName
* 在Self/maps中读取加载地址
************************************************************
*/
unsigned long getStartAddrByName(char* inName){
	FILE* fd =fopen("/proc/self/maps","r");
	if(NULL == fd)return 0;
	char* mBuf = (char*)malloc(1024);
	memset(mBuf,0,1024);
	unsigned long mStartAddr = 0;
	while(NULL != fgets(mBuf,1024,fd)){
		if(NULL != strstr(mBuf,inName)){
			mStartAddr = strtoul(mBuf,0,0x10);
			if(0 != mStartAddr)
				break;
		}
	}
	free(mBuf);
	return mStartAddr;
}
/*
************************************************************
*				getEndAddrByName
* 在Self/maps中读取结束地址
************************************************************
*/
unsigned long getEndAddrByName(char* inName){
	FILE* fd =fopen("/proc/self/maps","r");
	if(NULL == fd)return 0;
	char* mBuf = (char*)malloc(1024);
	memset(mBuf,0,1024);
	unsigned long mEndAddr = 0;
	while(NULL != fgets(mBuf,1024,fd)){
		if(NULL != strstr(mBuf,inName)){
			char* pEnd = strstr(mBuf,"-")+1;
			mEndAddr = strtoul(pEnd,0,0x10);
			if(0 != mEndAddr)
				break;
		}
	}
	free(mBuf);
	return mEndAddr;
}
/*
************************************************************
*					getStartAddrByName
* 在Self/maps中读取加载地址，根据RWX区分
************************************************************
*/
unsigned long getStartAddrByName(char* inName,char* inRW){
	FILE* fd =fopen("/proc/self/maps","r");
	if(NULL == fd)return 0;
	char* mBuf = (char*)malloc(1024);
	memset(mBuf,0,1024);
	unsigned long mStartAddr = 0;
	while(NULL != fgets(mBuf,1024,fd)){
		if((NULL != strstr(mBuf,inName))&&(NULL != strstr(mBuf,inRW))){
			mStartAddr = strtoul(mBuf,0,0x10);
			if(0 != mStartAddr)
				break;
		}
	}
	free(mBuf);
	return mStartAddr;
}
/*
************************************************************
*				getEndAddrByName
* 在Self/maps中读取结束地址，根据RWX区分
************************************************************
*/
unsigned long getEndAddrByName(char* inName,char* inRW){
	FILE* fd =fopen("/proc/self/maps","r");
	if(NULL == fd)return 0;
	char* mBuf = (char*)malloc(1024);
	memset(mBuf,0,1024);
	unsigned long mEndAddr = 0;
	while(NULL != fgets(mBuf,1024,fd)){
		if((NULL != strstr(mBuf,inName))&&(NULL != strstr(mBuf,inRW))){
			char* pEnd = strstr(mBuf,"-")+1;
			mEndAddr = strtoul(pEnd,0,0x10);
			if(0 != mEndAddr)
				break;
		}
	}
	free(mBuf);
	return mEndAddr;
}
/*
************************************************************
*			jstringTostring
*	将jstring转化成char*
************************************************************
*/
char* jstringTostring(JNIEnv* env, jstring jstr)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray) env->CallObjectMethod( jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}
/*
************************************************************
*			pcharToJstring
*	将 char*转化成jstring
************************************************************
*/
jstring pcharToJstring(JNIEnv* env, const char* pat, const char* encoding)
{
	jstring res;
    jstring jencoding;
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    jclass  gStringClass= (env)->FindClass("java/lang/String");
    jmethodID  gmidStringInit= (env)->GetMethodID(gStringClass, "<init>", "([BLjava/lang/String;)V");
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    if (encoding == NULL)
        jencoding= (env)->NewStringUTF("utf-8");
    else
        jencoding=(env)->NewStringUTF(encoding);
    res = (jstring)(env)->NewObject(gStringClass, gmidStringInit, bytes, jencoding);

    env->DeleteLocalRef(gStringClass);
    env->DeleteLocalRef(bytes);
    env->DeleteLocalRef(jencoding);
    return res;
}
/*
************************************************************
*
************************************************************
*/
void SaveFile(char* appName,char* inFile,void* inAddr,size_t inLen){
	char* filePath = (char*)malloc(1024);
	memset(filePath,0,1024);
	sprintf(filePath,"/sdcard/DumpDex/%s",inFile);
	int fd = syscall(5,filePath,O_RDWR|O_CREAT);
//	FILE* mfd=fopen(filePath,"wb");
	//查找可写文件路径
	if(!fd){
		DEXLOG("[Dump]Fail SD Path:%s",filePath);
		DEXLOG("[Dump]SD Open file fail!");
		memset(filePath,0,1024);
		if(appName != NULL){
			sprintf(filePath,"/data/data/%s/%s",appName,inFile);
			fd = syscall(5,filePath,O_RDWR|O_CREAT);
			if(!fd){
				DEXLOG("[Dump]Fail SD Path:%s",filePath);
				DEXLOG("[Dump]SD Open file fail!");
				free(filePath);
				return;
			}
		}else{
			DEXLOG("[Dump]Fail appName Is Null");
			free(filePath);
			return ;
		}
	}
	syscall(4,fd,inAddr,inLen);
	syscall(6,fd);
	DEXLOG("[Dump]File Path:%s",filePath);
	free(filePath);
}
/*
************************************************************
*			GetTimeName
*获取实时时间名字
* @param	inName ->	名字前缀
************************************************************
*/
	char*  GetTimeName(char* inName){
		struct timeval tv;
		gettimeofday(&tv,NULL);
		int m_HookS = tv.tv_sec%60;
		int m_HookM = (tv.tv_sec/60)%60;
		int m_HookH = (tv.tv_sec/3600)%24;
		char* mName = (char*)malloc(1024);
		memset(mName,0,1024);
		sprintf(mName,"%s_%02d_%02d_%02d",inName,m_HookH,m_HookM,m_HookS);
		return mName;
	}
};

