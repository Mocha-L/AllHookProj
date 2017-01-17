#pragma once
#include "stdafx.h"

#ifdef _WIN64
#include "MinHook.h"
#pragma  comment(lib,"libMinHook.x64.lib")

#define myDetourStart() MH_Initialize()
#define myDetourTransactionBegin()
#define myDetourAttach(OrgFun, OrgToCall, NewFun) \
{\
	MH_STATUS bRe = MH_CreateHook(OrgFun, &NewFun, reinterpret_cast<void**>(OrgToCall));\
	bRe = MH_EnableHook(OrgFun);\
}
#define myDetourDetach(OrgFun,NewFun) {MH_STATUS bRe = MH_DisableHook(OrgFun);}
#define myDetourUpdateThread(hThread)
#define myDetourTransactionCommit()
#define myDetourEnd() MH_Uninitialize()

#else

#include "detours.h"
#pragma comment(lib,"detours.lib")

#define myDetourStart()
#define myDetourTransactionBegin() DetourTransactionBegin()
#define myDetourAttach(OrgFun, OrgToCall, NewFun) DetourAttach((PVOID*)OrgToCall, NewFun)
#define myDetourDetach(OrgFun,NewFun) DetourDetach(OrgFun,NewFun)
#define myDetourTransactionCommit() DetourTransactionCommit()
#define myDetourUpdateThread(hThread) DetourUpdateThread(hThread)
#define myDetourEnd()

#endif

int StartHook();
int StopHook();
BOOL InstallAPIHook();
BOOL UnInstallAPIHook();