/*
 * DDConfig.cpp
 *
 *  Created on: 2015年11月30日
 *      Author: LsMouse
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include "DDConfig.H"
#include "Common.H"
namespace Utils{
//
int DDApp_Mode = 0;
char* DDApp_Info = NULL;
char* DDApp_List = NULL;
/*
************************************************************
*			getDDAppConfig
*	读取配置文件
************************************************************
*/
void getDDAppConfig()
{
	FILE* fd = fopen(DDConfigPATH,"r");
	if(NULL == fd)
	{
		DEXLOG("DDConfig getDDConfig open fail");
		return ;
	}
	if(DDApp_Info != NULL)
		free(DDApp_Info);
	//读取项内容
	DDApp_Info = (char*)malloc(10240);
	fgets(DDApp_Info,10240,fd);
	fclose(fd);
	//读取Mode，
	char *delim = ",";
	char* mInfo = (char*)malloc(10240);
	memcpy(mInfo,DDApp_Info,10240);
	char *p  =  strtok(mInfo, delim);
	while(p != NULL){
		if(strstr(p,"MODE:CLOSE") != NULL){
			DDApp_Mode = DDAMODE_CLOSE;
		}else if(strstr(p,"MODE:DEBUG") != NULL){
			DDApp_Mode = DDAMODE_DEBUG;
		}else if(strstr(p,"MODE:TestDump") != NULL){
			DDApp_Mode = DDAMODE_TestDump;
		}else if(strstr(p,"MODE:AutoDump") != NULL){
			DDApp_Mode = DDAMODE_AutoDump;
		}
		p = strtok(NULL, delim);
	}
	free(mInfo);
}
/*
************************************************************
*			DDAppTarget
*	判断是否是目标进程
************************************************************
*/
char* DDAppTarget(char* inNAME)
{
	char* result = NULL;
	char* mInfo = (char*)malloc(10240);
	memcpy(mInfo,DDApp_Info,10240);
	char *delim = ",";
	char *p  =  strtok(mInfo, delim);
	while(p != NULL){
		if(strstr(p,"APP:") != NULL)
		{
			p += strlen("APP:");
			if(strstr(inNAME,p) != NULL)
			{
				result = strdup(p);
				break;
			}
		}
		p = strtok(NULL, delim);
	}
	free(mInfo);
	return result;
}
/*
************************************************************
*			DDAppContains
*	判断当前名字是否在DDApp之中
************************************************************
*/
bool DDAppContains(char* inName)
{
	if(NULL == DDApp_Info)
	{
		getDDAppConfig();
		if(NULL == DDApp_Info)
			return false;
	}
	bool result = false;
	char* mInfo = (char*)malloc(10240);
	memcpy(mInfo,DDApp_Info,10240);
	const char *delim = ",";
	char *p  =  strtok(mInfo, delim);
	if(p != NULL){
		do{
			if(strstr(inName,p) != NULL){
				p = strdup(p);
				result = true;
				break;
			}
		}while((p = strtok(NULL, delim)) != NULL);
	}
	free(mInfo);
	return result;
}
/*
************************************************************
*			getDDAppMode
*	获取当前配置文件的运行模块
************************************************************
*/
int getDDAppMode()
{
	return DDApp_Mode;
}
};



