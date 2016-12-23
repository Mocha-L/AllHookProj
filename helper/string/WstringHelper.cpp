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