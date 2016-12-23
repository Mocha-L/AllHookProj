#pragma once
#include "WstringHelper.h"

#define DECLARE_FUNCTION(funcName)			extern pf##funcName funcName
#define INIT_FUNCTION(funcName)		pf##funcName funcName = NULL
#define GET_FUNCTINADDR(hDll, funcName)\
	funcName = (pf##funcName)::GetProcAddress(hDll, #funcName);\
	if (NULL == #funcName) {AfxMessageBox(stringformat(L"Load %s Error!",L#funcName).c_str());}

namespace KeyboardMouse_Hook
{
	bool LoadKeyBoardMouse(const wchar_t* cDllPath = L"KeyBoardAndMouse.dll");
	bool ReleaseKeyBoardMouse();

	typedef void (__stdcall *pfSetHook)(DWORD dwParam,HWND hwnd);
	DECLARE_FUNCTION(SetHook);
}