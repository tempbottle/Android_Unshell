/*
 * SubHook.H
 *
 *  Created on: 2015年12月3日
 *      Author: LsMouse
 */

#ifndef SUBHOOK_H_
#define SUBHOOK_H_
//

#include "Substrate.H"
#include "Hook_Name.H"
//
extern int SubHook(char* inSoName,char* inSym, void *replace, void **result);
extern int SubHookAddr(void* inAddr, void *replace, void **result);


#endif /* SUBHOOK_H_ */
