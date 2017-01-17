#include "stdafx.h"
#include "WstringHelper.h"

std::wstring  stringformat(const wchar_t* fmt,   ...)   
{  
	std::wstring s=L"";
	try
	{
		va_list   argptr;   
		va_start(argptr, fmt);   
#pragma warning( push )
#pragma warning( disable : 4996 )
		int   bufsize = _vsnwprintf(NULL, 0, fmt, argptr) + 2;   
#pragma warning( pop )
		wchar_t* buf=new wchar_t[bufsize];   
		_vsnwprintf_s(buf, bufsize, _TRUNCATE, fmt, argptr);   
		s=buf;   
		delete[] buf;       
		va_end(argptr);     
	}
	catch(...)
	{
		s=L"TryError!";
	}
	return   s;   
}

std::wstring Path_GetCurrent(HMODULE hModule)
{
	std::wstring strPath=L"";
	wchar_t cPath[MAX_PATH];
	wchar_t *pLast = NULL;
	memset(cPath,0,sizeof(cPath));
	GetModuleFileName(hModule,cPath,MAX_PATH);
	pLast=wcsrchr(cPath,L'\\');
	if (NULL!=pLast)
	{
		*pLast=0;
	}
	strPath=cPath;
	strPath+=L"\\";
	return strPath;
}