/*
 * TracerPid.H
 *
 *  Created on: 2016年1月5日
 *      Author: LsMouse
 */

#ifndef TRACERPID_H_
#define TRACERPID_H_
#include "stdio.h"
#include "SubHook/SubHook.H"
/*
 ************************************************************
 *				New$fgets
 @char *fgets(char *buf, int bufsize, FILE *stream);
 ************************************************************
 */
static char* (*$fgets)(char *buf, int bufsize, FILE *stream);
static char* New$fgets(char *buf, int bufsize, FILE *stream)
 {
 	char* result = $fgets(buf,bufsize,stream);
 	if(strstr(buf,"TracerPid:") != NULL)
 	{
 		memset(buf,0,40);
 		strcpy(buf,"TracerPid:0");
 	}else{
 		return result;
 	}
 	return buf;
 }
static void Hook_TracerPid()
{
	SubHook(libc,libc_fgets,(void*)&New$fgets,(void**)&$fgets);
}
#endif /* TRACERPID_H_ */
