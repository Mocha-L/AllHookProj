#include <windows.h>
#include <tlhelp32.h>

DWORD GetProcessIdByName(const wchar_t*ProcessName);
int DllInject(const wchar_t *pProcessName, const wchar_t *pDllName);
int DllFree(const wchar_t *pProcessName, const wchar_t *pDllName);
BOOL FindModelByProcessId(DWORD dwProcessID,const wchar_t* pDllName,MODULEENTRY32W &me);
BOOL EnablePrivilege();