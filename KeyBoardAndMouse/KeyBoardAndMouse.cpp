// KeyBoardAndMouse.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "WstringHelper.h"

HHOOK g_hMouse = NULL;
HHOOK g_hKeyBoard = NULL;

//#pragma data_seg("MyHook_seg")
HWND g_hWnd = NULL;
//#pragma data_seg()
//
//#pragma comment(linker,"/section:MyHook_seg,RWS")

LRESULT CALLBACK MouseProc(int iCode,WPARAM wParam,LPARAM lParam)
{
	return 1;
}

LRESULT CALLBACK KeyBordProc(int iCode,WPARAM wParam,LPARAM lParam)
{
	OutputDebugStringW(stringformat(L"iCode = %X,wParam = %X,lParam = %X",iCode,wParam,lParam).c_str());
	if(iCode == HC_ACTION && ((lParam>>31) == 0))
	{
		if (VK_F2 == wParam)
		{
			::SendMessage(g_hWnd,WM_BTN_UNHOOKMOUSE,0,0);
		}
		::SendMessage(g_hWnd,WM_SENDWORDCODE,wParam,lParam);
	}

	return CallNextHookEx(g_hKeyBoard,iCode,wParam,lParam);
}

extern "C" __declspec(dllexport) void SetHook(DWORD dwParam,HWND hwnd)
{
	g_hWnd = hwnd;
	if (dwParam&HOOK)
	{
		if(dwParam&SET_KEYBOARD && g_hKeyBoard == NULL)
		{
			g_hKeyBoard = SetWindowsHookEx(WH_KEYBOARD,KeyBordProc,GetModuleHandle(L"KeyBoardAndMouse.dll"),0);
		}
		else if(dwParam&SET_MOUSE && g_hMouse == NULL)
		{
			g_hMouse = SetWindowsHookEx(WH_MOUSE,MouseProc,GetModuleHandle(L"KeyBoardAndMouse.dll"),0);
		}
	}
	else if(dwParam&UNHOOK)
	{
		if(dwParam&SET_KEYBOARD && g_hKeyBoard != NULL)
		{
			UnhookWindowsHookEx(g_hKeyBoard);
			g_hKeyBoard = NULL;
		}
		else if(dwParam&SET_MOUSE && g_hMouse != NULL)
		{
			UnhookWindowsHookEx(g_hMouse);
			g_hMouse = NULL;
		}
	}
}


