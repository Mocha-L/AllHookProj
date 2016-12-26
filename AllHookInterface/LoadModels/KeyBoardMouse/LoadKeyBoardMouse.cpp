#include "stdafx.h"
#include "LoadKeyBoardMouse.h"
using namespace KeyboardMouse_Hook;

namespace KeyboardMouse_Hook
{
	static HINSTANCE s_hKeyBoardMouse = NULL;
	INIT_FUNCTION(SetHook);

	bool LoadKeyBoardMouse(const wchar_t* cDllPath)
	{
		if (NULL != s_hKeyBoardMouse)
		{
			return false;
		}

		s_hKeyBoardMouse = ::LoadLibrary(cDllPath);
		if (NULL == s_hKeyBoardMouse)
		{
			return false;
		}

		GET_FUNCTINADDR(s_hKeyBoardMouse, SetHook);

		return true;
	}
	bool ReleaseKeyBoardMouse()
	{
		if(s_hKeyBoardMouse)
		{
			::FreeLibrary(s_hKeyBoardMouse);
			s_hKeyBoardMouse = NULL;
		}
		return true;
	}
}