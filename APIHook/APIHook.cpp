// APIHook.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "APIHook.h"
#include "../helper/string/WstringHelper.h"

//orgCreateFileW
HANDLE (WINAPI* orgCreateFileW)(
								__in     LPCWSTR lpFileName,
								__in     DWORD dwDesiredAccess,
								__in     DWORD dwShareMode,
								__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								__in     DWORD dwCreationDisposition,
								__in     DWORD dwFlagsAndAttributes,
								__in_opt HANDLE hTemplateFile
								) = CreateFileW;

HANDLE WINAPI myCreateFileW(
							__in     LPCWSTR lpFileName,
							__in     DWORD dwDesiredAccess,
							__in     DWORD dwShareMode,
							__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							__in     DWORD dwCreationDisposition,
							__in     DWORD dwFlagsAndAttributes,
							__in_opt HANDLE hTemplateFile
							)
{
	if (dwCreationDisposition == CREATE_NEW || dwCreationDisposition == CREATE_ALWAYS)
	{
		OutputDebugString(L"myCreateFileW Enter!");
		HANDLE hCreateFile = NULL;
		wchar_t szSrcPath[MAX_PATH] = {0};
		lstrcpyW(szSrcPath, lpFileName);
		hCreateFile = orgCreateFileW(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
		if (hCreateFile != INVALID_HANDLE_VALUE)
		{
			//
		}
		return hCreateFile;
	}
	else
	{
		return orgCreateFileW(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
	}
}

BOOL (WINAPI*orgCreateDirectoryW)(
								  __in     LPCWSTR lpPathName,
								  __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
								  ) = CreateDirectoryW;

BOOL WINAPI myCreateDirectoryW(
							   __in     LPCWSTR lpPathName,
							   __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
							   )
{
	OutputDebugString(L"myCreateDirectoryW Enter!");
	BOOL bRet = FALSE;
	bRet = orgCreateDirectoryW(lpPathName,lpSecurityAttributes);
	if (bRet != FALSE)
	{
		wchar_t szSrcPath[MAX_PATH] = {0};
		lstrcpyW(szSrcPath, lpPathName);
		//
	}
	return bRet;
}

int (WINAPI* orgMessageBoxW)(
			__in_opt HWND hWnd,
			__in_opt LPCWSTR lpText,
			__in_opt LPCWSTR lpCaption,
			__in UINT uType) = MessageBoxW;

int WINAPI myMessageBoxW(
							 __in_opt HWND hWnd,
							 __in_opt LPCWSTR lpText,
							 __in_opt LPCWSTR lpCaption,
							 __in UINT uType)
{
	OutputDebugString(L"myMessageBoxW enter!");
	OutputDebugString(stringformat(L"text:%s,caption:%s",lpText,lpCaption).c_str());
	lpText = L"this messagebox is hooked!";
	return orgMessageBoxW(hWnd,lpText,lpCaption,uType);
}

BOOL InstallAPIHook()
{
	OutputDebugString(L"InstallAPIHook enter!");
	myDetourStart();
	myDetourTransactionBegin();
	myDetourUpdateThread(GetCurrentThread());

	myDetourAttach(&CreateFileW, &orgCreateFileW, myCreateFileW);
	myDetourAttach(&CreateDirectoryW, &orgCreateDirectoryW, myCreateDirectoryW);
	//其他APIhook按照该示例添加即可，该实例是hook explorer的新建文件(夹),示例仅在xp下可用,win7及以上请使用com hook。
	//TO DO : add ur hook like : 'messagebox'
	myDetourAttach(&MessageBox,&orgMessageBoxW,myMessageBoxW);

	myDetourTransactionCommit();

	return TRUE;
}

BOOL UnInstallAPIHook()
{
	OutputDebugString(L"UnInstallAPIHook enter!");
	myDetourTransactionBegin();
	myDetourUpdateThread(GetCurrentThread());

	myDetourDetach(&(PVOID&)orgCreateFileW, myCreateFileW);
	myDetourDetach(&(PVOID&)orgCreateDirectoryW, myCreateDirectoryW);

	myDetourTransactionCommit();
	myDetourEnd();

	return TRUE;
}

int StartHook()
{
	OutputDebugString(L"StartHook enter!");
	//wchar_t strFileName[MAX_PATH]	= {0};
	//GetModuleFileName(NULL,strFileName,MAX_PATH);
	//if (wcsstr(strFileName,L"explorer.exe"))
	//{
		InstallAPIHook();
	//}
	return 0;
}

int StopHook()
{
	OutputDebugString(L"StopHook enter!");
	UnInstallAPIHook();
	return 0;
}