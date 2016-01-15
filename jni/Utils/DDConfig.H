/*
 * DDConfig.H
 *
 *  Created on: 2015年11月30日
 *      Author: LsMouse
 */

#ifndef DDCONFIG_DDCONFIG_H_
#define DDCONFIG_DDCONFIG_H_
//定义路径和运行方式
#define DDConfigPATH		"/data/local/MHookDemo/Config"
#define DDAMODE_CLOSE		0
#define DDAMODE_DEBUG		1
#define DDAMODE_TestDump	2
#define DDAMODE_AutoDump	3
namespace Utils{
//声明函数
	extern void getDDAppConfig();
	extern void DDAppContains();
	extern int getDDAppMode();
	extern char* DDAppTarget(char* inNAME);
};
#endif /* DDCONFIG_DDCONFIG_H_ */
