// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define HOOK 0x10
#define UNHOOK 0x20
#define SET_MOUSE		0x01
#define SET_KEYBOARD	0x02

#define WM_BTN_UNHOOKMOUSE (WM_USER+10)
#define WM_SENDWORDCODE (WM_USER+11)
// TODO: reference additional headers your program requires here
