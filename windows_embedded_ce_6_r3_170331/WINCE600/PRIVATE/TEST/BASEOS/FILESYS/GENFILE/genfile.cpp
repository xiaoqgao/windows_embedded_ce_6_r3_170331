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
#include "main.h"
#include "utility.h"
#include <windows.h>
#include <math.h>
#include <time.h>

const DWORD		DO_NOT_CHECK			= -1;
const unsigned	ERROR_UNDEFINED			= DO_NOT_CHECK;

extern	const	wchar_t * sfile; 
extern	const	wchar_t * sinfile; 
extern	const	wchar_t * soutfile; 
extern	const	wchar_t * sfailifexist; 
extern	const	wchar_t * sprogress; 
extern	const	wchar_t * scancel; 
extern	const	wchar_t * screate; 
extern	const	wchar_t * sdelete; 
extern	const	wchar_t * sbytes; 
extern	const	wchar_t * stoolong; 
extern	const	wchar_t * sretval;
extern	const	wchar_t * sqmark;
extern	const	wchar_t * spassalways; 
extern	const	wchar_t * ssystem; 
extern	const	wchar_t * shidden; 
extern	const	wchar_t * ssystem; 
extern	const	wchar_t * sreadonly; 
extern	const	wchar_t * snormal; 
extern	const	wchar_t * sarchive; 
extern	const	wchar_t * swhackwhackqmarkwhack;


bool	CreateFile(const wchar_t * s, unsigned ccb, DWORD dwAttr = 0);
DWORD	GetFlagsAndOptionsFromCmdLine(const wchar_t * s);

void Usage(void)
{
	NKDbgPrintfW(TEXT("GENFILE: A utility to generate or delete files of specific size for testing"));
    NKDbgPrintfW(TEXT("Usage:"));
	NKDbgPrintfW(TEXT("%s [file_name | %s], \n\twhere file_name is a file name, %s means use a too-long (generated) name, leave empty to use an empty string for a file name"), sfile, stoolong, stoolong);
	NKDbgPrintfW(TEXT("[%s] means delete the file, [%s] means create"), sdelete, screate);
	NKDbgPrintfW(TEXT("[%s][%s][%s][%s][%s] are flags for file attributes in a created file"), sarchive, shidden, ssystem, sreadonly, snormal);
	NKDbgPrintfW(TEXT("[%s <bytes>] sets the size of the file, default is %d"), sbytes, DEFAULT_FILE_SIZE);
	NKDbgPrintfW(TEXT("[%s 0 | 1] means always return PASS (if 1) or actual return (if 0), default is 1"), spassalways);
	NKDbgPrintfW(TEXT("Of course [%s] brings up this screen"), sqmark);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCmdLine, int nCmdShow)
{
	
	bool bDelete = false;
	bool bCreate = false;

	unsigned	cchCmdLine = 0; 

	int		nRet = TPR_PASS;
	
	if(FlagIsPresentOnCmdLine(szCmdLine, sqmark))
		Usage();


	SYSTEMTIME st = {0,};
	GetSystemTime(&st);
	srand(st.wMilliseconds);

	//snag file names from command line ... 
	wchar_t		* sFile = 0; 
	wchar_t		sFileBuf[MAX_PATH] = {0,}, sOutFileBuf[MAX_PATH] = {0,}; 
	
	if(!GetStringFromCmdLine(szCmdLine, sFileBuf, CHARCOUNT(sFileBuf), sfile))
		sFileBuf[0] = 0; //use empty string


	//TODO: generate too-long names here if need be ...
	
	wchar_t * sLongFile = 0;
	if(!wcsicmp(sFileBuf, stoolong))
	{
		sLongFile = AllocAndFillLongName(); 
		sFile = sLongFile; 
	}
	else
		sFile = sFileBuf; 
	
	bDelete = FlagIsPresentOnCmdLine(szCmdLine, sdelete);
	bCreate = FlagIsPresentOnCmdLine(szCmdLine, screate);
	
	//get copy flags, default is 0
	unsigned		dwBytes		= 0;

	//does not touch dwDesiredReturn if fail, OK
	GetULIntFromCmdLine(szCmdLine, sbytes, &dwBytes); 

	if(bDelete)
		nRet = DeleteFile(sFile) ? TPR_PASS : TPR_FAIL;

	if(bCreate)
	{
		DWORD dwAttr = 0;
		nRet = CreateFile(sFile, dwBytes, GetFlagsAndOptionsFromCmdLine(szCmdLine)) ? TPR_PASS : TPR_FAIL;
		dwAttr = GetFileAttributes(sFile); 
		if(dwAttr == -1) 
		{
			NKDbgPrintfW(TEXT("Unable to get attributes for %s after creation succeeded, GetLastError() says %d"), sFile, GetLastError());
			nRet = TPR_FAIL; 
		}
		else
			NKDbgPrintfW(TEXT("Attributes for %s after creation succeeded are 0x%x (0%o))"), sFile, dwAttr, dwAttr);


	}

	
	if(sLongFile) free(sLongFile); 
	
	unsigned ulPassAlways = 0;
	GetULIntFromCmdLine(szCmdLine, spassalways, &ulPassAlways); 

	if(ulPassAlways)
		return TPR_PASS; 

	return nRet; 
}


bool	CreateFile(const wchar_t * s, unsigned ccb, DWORD dwAttr)
{
	unsigned char c; 
	DWORD	cbWritten;

	HANDLE  h = CreateFile(
		s, 
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		0, 
		CREATE_ALWAYS,
		dwAttr,
		0);

	if(h == INVALID_HANDLE_VALUE)
		return false; 

	for(unsigned idx = 0; idx < ccb; ++idx)
	{
		c = rand() % 0xff;	//never an EOF char this way ... 
		if(!WriteFile(h, &c, 1, &cbWritten, 0))
		{
			CloseHandle(h); 
			return false; 
		}
	}

	CloseHandle(h);
	return true;
}


DWORD GetFlagsAndOptionsFromCmdLine(const wchar_t * s)
{
	DWORD dwRet = 0;

	if(!s) return 0; 
	if(!*s) return 0; 

	if(FlagIsPresentOnCmdLine(s, ssystem))
		dwRet |= FILE_ATTRIBUTE_SYSTEM; 

	if(FlagIsPresentOnCmdLine(s, sreadonly))
		dwRet |= FILE_ATTRIBUTE_READONLY; 

	if(FlagIsPresentOnCmdLine(s, sarchive))
		dwRet |= FILE_ATTRIBUTE_ARCHIVE; 

	if(FlagIsPresentOnCmdLine(s, shidden))
		dwRet |= FILE_ATTRIBUTE_HIDDEN; 

	if(FlagIsPresentOnCmdLine(s, snormal))
		dwRet |= FILE_ATTRIBUTE_NORMAL; 

	return dwRet;

}