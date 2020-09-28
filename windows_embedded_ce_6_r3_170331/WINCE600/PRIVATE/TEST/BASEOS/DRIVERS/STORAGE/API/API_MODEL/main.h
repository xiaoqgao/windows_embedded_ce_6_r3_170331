//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft shared
// source or premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license agreement,
// you are not authorized to use this source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the SOURCE.RTF on your install media or the root of your tools installation.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <diskio.h>
#include <atapi.h>
#include <tchar.h>
#include <katoex.h>
#include <tux.h>
#include <storemgr.h>
#include <devutils.h>
#include <clparse.h>
#include "IoctlTestBase.h"
#include "DISK_IOCTL_GETINFO.h"
#include "IOCTL_DISK_GETINFO.h"
#include "DISK_IOCTL_SETINFO.h"
#include "IOCTL_DISK_SETINFO.h"
#include "DISK_IOCTL_READ.h"
#include "IOCTL_DISK_READ.h"
#include "DISK_IOCTL_FORMAT_MEDIA.h"
#include "IOCTL_DISK_FORMAT_MEDIA.h"
#include "DISK_IOCTL_GETNAME.h"
#include "IOCTL_DISK_GETNAME.h"
#include "IOCTL_DISK_GET_STORAGEID.h"
#include "IOCTL_DISK_DEVICE_INFO.h"
#include "DISK_IOCTL_WRITE.h"
#include "IOCTL_DISK_WRITE.h"
#include "IOCTL_DISK_FLUSH_CACHE.h"
//
// global macros
//
#define INVALID_HANDLE(X)   (INVALID_HANDLE_VALUE == X || NULL == X)
#define VALID_HANDLE(X)     (INVALID_HANDLE_VALUE != X && NULL != X)

#ifdef DEBUG
#define NKDBG               NKDbgPrintfW
#else
#define NKDBG               (VOID)
#endif

#define NKMSG               NKDbgPrintfW

#define MAX_DISK            10 // maximum # of disks to check (e.g. DSK1, DSK2)

#define MAX_BUFFER_SIZE     (1024 * 1024) // 1MB buffer size
#define READ_LOCATIONS      1000 // tests will hit every 1/READ_LOCATIONS position on disk

#define DEF_MAX_SECTORS		128

#define DI_TOTAL_SECTORS 25
#define DI_BYTES_PER_SECT 255
#define DI_CYLINDERS 1
#define DI_HEADS 2
#define DI_SECTORS 255000
#define DI_FLAGS 0x1008
#define ATAPI_HD_DISK_NAME _T("Hard Disk")
#define ATAPI_CF_DISK_NAME _T("PC Card ATA Disk")
#define ATAPI_HD_DISK_PROFILE _T("HDProfile");
#define ATAPI_CF_DISK_PROFILE _T("PCMCIA");

BOOL SetDiskInfo(HANDLE hDisk,LPWSTR pszDisk,DISK_INFO *pDiskInfo);
BOOL GetDiskInfo(HANDLE hDisk,LPWSTR pszDisk,DISK_INFO *pDiskInfo);
// getopt.cpp
INT WinMainGetOpt(LPCTSTR, LPCTSTR);

// disk.cpp
HANDLE OpenDiskHandle(VOID);

BOOL FormatMedia(HANDLE);

BOOL DeleteSectors(HANDLE, DWORD, DWORD);

BOOL ReadWriteDisk(HANDLE, DWORD, PDISK_INFO, DWORD, INT, TCHAR*); 
BOOL ReadWriteDisk( HANDLE hDisk,DWORD ctlCode, PSG_REQ pSgReq);
BOOL ReadWriteDiskMulti(HANDLE, DWORD, PDISK_INFO, PSG_REQ);

BOOL ReadDiskSg(HANDLE, PDISK_INFO, DWORD, DWORD, PBYTE, DWORD);


//
// test procedures
//
TESTPROCAPI	tst_DISK_IOCTL_GETINFO (UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI	tst_DISK_IOCTL_SETINFO (UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_DISK_IOCTL_READ   (UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_DISK_IOCTL_WRITE   (UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_DISK_IOCTL_INITIALIZED(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_DISK_IOCTL_FORMAT_MEDIA(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_DISK_IOCTL_GETNAME(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_DEVICE_INFO(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_GETINFO(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_SETINFO(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_READ(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_WRITE(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_INITIALIZED(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_FORMAT_MEDIA(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_GETNAME(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_GET_STORAGEID(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_READ_CDROM(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_WRITE_CDROM(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_DELETE_SECTORS(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_GET_SECTOR_ADDR(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_COPY_EXTERNAL_START(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_COPY_EXTERNAL_COMPLETE(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_GETPMTIMINGS(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_SECURE_WIPE(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_SET_SECURE_WIPE_FLAG(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_FILE_WRITE_GATHER(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_FILE_READ_SCATTER(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_FILE_GET_VOLUME_INFO(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_FORMAT_VOLUME(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_SCAN_VOLUME(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
TESTPROCAPI tst_IOCTL_DISK_FLUSH_CACHE(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);

TESTPROCAPI RunIOCTLCommon(PDRIVER_TEST pMyTest);
//
// tuxmain.cpp
//
extern CKato            *g_pKato;
extern SPS_SHELL_INFO   *g_pShellInfo;
extern TCHAR             g_szDiskName[];
extern TCHAR 			 g_szProfile[];
extern HANDLE            g_hDisk;
extern DISK_INFO         g_diskInfo;
extern BOOL              g_fOpenAsStore;
extern DWORD			 g_dwMaxSectorsPerOp;
extern DWORD		g_dwBreakAtTestCase;
extern DWORD  g_dwBreakOnFailure;
extern STORAGEDEVICEINFO g_storageDeviceInfo;
#endif // __MAIN_H__