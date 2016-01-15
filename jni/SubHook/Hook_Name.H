/*
 * Hook_Name.H
 *
 *  Created on: 2015年12月3日
 *      Author: LsMouse
 */

#ifndef HOOK_NAME_H_
#define HOOK_NAME_H_
//SO库
#define libdvm				"/system/lib/libdvm.so"
#define libc				"/system/lib/libc.so"
#define linker				"/system/lib/linker"
#define libdl				"/system/lib/libdl.so"
//libdvm symbol
#define dvm_LoadNativeCode			"_Z17dvmLoadNativeCodePKcP6ObjectPPc"
#define dvm_dexFileParse			"_Z12dexFileParsePKhji"
#define dvm_ResolveClass			"dvmResolveClass"
#define dvm_DexFileOpenFromFd 		"_Z20dvmDexFileOpenFromFdiPP6DvmDex"
#define dvm_DexFileOpenPartial 		"_Z21dvmDexFileOpenPartialPKviPP6DvmDex"
#define	 dvm_RawDexFileOpen			"_Z17dvmRawDexFileOpenPKcS0_PP10RawDexFileb"
#define dvm_HashTableLookup			"_Z18dvmHashTableLookupP9HashTablejPvPFiPKvS3_Eb"
//libc symbol
#define libc_dlsym			"dlsym"
#define libc_open			"open"
#define libc_fopen			"fopen"
#define libc_dlopen			"dlopen"
#define libc_dlsym			"dlsym"
#define libc_mmap			"mmap"
#define libc_kill			"kill"
#define libc_ptrace			"ptrace"
#define libc_free			"free"
#define libc_munmap			"munmap"
#define libc_strlen			"strlen"
#define libc_memcpy			"memcpy"
#define libc_strcmp			"strcmp"
#define libc_fgets			"fgets"

//
#define Final_JNI_OnLoad		"JNI_OnLoad"
//

#endif /* HOOK_NAME_H_ */
