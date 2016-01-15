/*
 * syscall.H
 *
 *  Created on: 2015年12月28日
 *      Author: LsMouse
 */

#ifndef SYSCALL_H_
#define SYSCALL_H_
int sys_open(char* inFile,int rw)
{
	asm("mov r7, #5");
	asm("svc 0");
}
int sys_write(int fd,void* buf,int len)
{
	asm("mov r7, #4");
	asm("svc 0");
}
int sys_close(int fd)
{
	asm("mov r7, #6");
	asm("svc 0");
}


#endif /* SYSCALL_H_ */
