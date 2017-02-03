#include "stdafx.h"
#include "WstringHelper.h"
#include "InjectDlls.h"
  
/***************************** 
*函数名：GetProcessIdByName 
*功  能：根据进程名查找进程ID 
*入  参：const char*ProcessName，进程名 
*出  参：无 
*返回值：进程ID，失败返回-1 
*****************************/  
DWORD GetProcessIdByName(const wchar_t*ProcessName)  
{  
    PROCESSENTRY32 stProcess;  
    HANDLE hProcessShot;  
    stProcess.dwSize=sizeof(PROCESSENTRY32);  
    hProcessShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
    Process32First(hProcessShot,&stProcess);  
  
    do  
    {
        if(!_tcscmp(ProcessName,stProcess.szExeFile))
		{
			CloseHandle(hProcessShot);  
            return stProcess.th32ProcessID;
		}
    }while(Process32Next(hProcessShot,&stProcess));  
  
    CloseHandle(hProcessShot);  
    return -1;  
}

/***************************** 
*函数名：FindModelByProcessId 
*功  能：根据进程名查找模块 
*入  参：DWORD dwProcessID,const wchar_t* pDllName,
*出  参：MODULEENTRY32W &me
*返回值：进程ID，失败返回-1 
*****************************/  
BOOL FindModelByProcessId(DWORD dwProcessID,const wchar_t* pDllName,MODULEENTRY32W &me)
{
	HANDLE hthSnapshot = NULL;
	hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	if (hthSnapshot == NULL)
	{
		return FALSE;
	}

	BOOL bFound = FALSE;
	BOOL bMoreMods = Module32FirstW(hthSnapshot, &me);
	for (; bMoreMods; bMoreMods = Module32NextW(hthSnapshot, &me))
	{
		bFound = (lstrcmpiW(me.szModule, pDllName) == 0) || (lstrcmpiW(me.szExePath, pDllName) == 0);
		if (bFound)
		{
			break;
		}
	}
	CloseHandle(hthSnapshot);
	return bFound;
}
  
/***************************** 
*函数名：DllInject 
*功  能：将dll注入到指定的进程中 
*入  参：const char*ProcessName，进程名 
        const char *pDllName，dll名 
*出  参：无 
*返回值：成功返回0，失败返回-1 
*****************************/  
int DllInject(const wchar_t *pProcessName, const wchar_t *pDllName)  
{
	int iRet = 0;
	DWORD dwProcessID=0;  
	HANDLE hProcessHandle=NULL;  
	LPVOID pAddrStart=NULL;  
	HANDLE hThreadHandle=NULL; 
    BOOL bSuccess = FALSE;
	DWORD dwSize = 0;
    //根据进程名获取进程ID  
    dwProcessID = GetProcessIdByName(pProcessName);  
    if(dwProcessID == -1)  
    {  
        OutputDebugString(stringformat(L"%s未运行", pProcessName).c_str());
        iRet = -1;
		goto _DllInject_End;
    }
	OutputDebugString(stringformat(L"%s进程ID为%d", pProcessName,dwProcessID).c_str());

    //根据进程ID获取进程句柄  
    hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);  
    if(hProcessHandle == NULL)  
    {  
        OutputDebugString(L"OpenProcess获取进程句柄失败\n");  
		iRet = -1;
		goto _DllInject_End;
    }

#ifdef UNICODE
	dwSize = (lstrlen(pDllName) + 1) * 2; //宽字符占用2个字节
#else
	dwSize = lstrlen(pDllName) + 1;
#endif

    //用VirtualAllocEx在进程内申请内存  
    pAddrStart = VirtualAllocEx(hProcessHandle,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);  
    if (pAddrStart == NULL)  
    {
        OutputDebugString(L"进程内存申请失败!\n");  
		iRet = -1;
		goto _DllInject_End; 
    }
  
    //将需要运行的dll名写入申请的内存地址  
    bSuccess = WriteProcessMemory(hProcessHandle,pAddrStart,pDllName,dwSize,0);  
    if(!bSuccess)  
    {  
        OutputDebugString(L"WriteProcessMemory失败！\n");  
		iRet = -1;
		goto _DllInject_End;
    }  
    //printf("memory of pAddrStart is:%s",pAddrStart);  
  
    //注入,即"LoadLibraryW"函数加载dll  
    hThreadHandle = CreateRemoteThread(hProcessHandle,  
                                       0,  
                                       0,  
                                       (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"),"LoadLibraryW"),//函数LoadLibraryW的地址  
                                       pAddrStart,//dll
                                       0,  
                                       0);  
    if(hThreadHandle == NULL)  
    {
		OutputDebugString(stringformat(L"在进程%s中注入%s失败 Getlasterror:%d",pProcessName,pDllName,GetLastError()).c_str());
		iRet = -1;
		goto _DllInject_End;
    }  
    WaitForSingleObject(hThreadHandle,INFINITE);  
    //到这里已经完成dll的加载即注入了，通过dll函数执行我们要完成的任务   
    
_DllInject_End:

	//释放 
	if (pAddrStart != NULL)
	{
		VirtualFreeEx(hProcessHandle,pAddrStart,0,MEM_RELEASE);
		pAddrStart = NULL;
	}
	if (hThreadHandle != NULL)
	{
		 CloseHandle(hThreadHandle);
	}
   if (hProcessHandle != NULL)
   {
	   CloseHandle(hProcessHandle);  
   }

    return iRet;  
}  
  
/***************************** 
*函数名：DllFree 
*功  能：卸载注入到进程中的dll 
*入  参：const char*ProcessName，进程名 
        const char *pDllName，dll名 
*出  参：无 
*返回值：成功返回0，失败返回-1 
*****************************/  
int DllFree(const wchar_t *pProcessName, const wchar_t *pDllName)  
{
	int iRet = 0;
	HANDLE hProcessHandle=NULL;  
	HANDLE hThreadHandle=NULL; 
    BOOL bSuccess = FALSE;
	DWORD dwHandle = 0;
	DWORD dwSize = 0;

    //根据进程名获取进程ID
	DWORD dwProcessID=0;
    dwProcessID = GetProcessIdByName(pProcessName);  
    if(dwProcessID == -1)  
    {  
        OutputDebugString(stringformat(L"%s未运行", pProcessName).c_str());
        iRet = -1;
		goto _DllFree_End;
    }  
	OutputDebugString(stringformat(L"%s进程ID为%d", pProcessName,dwProcessID).c_str());

	MODULEENTRY32W me;
	me.dwSize = sizeof(me);
	BOOL bFound = FindModelByProcessId(dwProcessID,pDllName,me);
	if (!bFound)
	{
		OutputDebugString(L"没找到指定模块");
		iRet = -1;
		goto _DllFree_End;
	}

	//根据进程ID获取进程句柄  
	hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);  
	if(hProcessHandle == NULL)  
	{
		OutputDebugString(L"OpenProcess获取进程句柄失败\n");
		iRet = -1;
		goto _DllFree_End;
	}

    //把FreeLibrary注入到进程，释放注入的DLL
	LPVOID pFun = GetProcAddress(GetModuleHandle(L"kernel32.dll"),"FreeLibrary");
    hThreadHandle=CreateRemoteThread(hProcessHandle,  
                                       0,  
                                       0,  
                                       (LPTHREAD_START_ROUTINE)pFun,  
                                       me.modBaseAddr,
                                       0,  
                                       0);
	WaitForSingleObject(hThreadHandle,INFINITE);

_DllFree_End:
	if (hThreadHandle != NULL)
	{
		CloseHandle(hThreadHandle);
	}
	if (hProcessHandle != NULL)
	{
		CloseHandle(hProcessHandle);
	}

    return iRet;
}

//修改进程权限  
BOOL EnablePrivilege()
{  
	BOOL bRet = false;
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if(!::OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return bRet;

	if(!::LookupPrivilegeValue( NULL, SE_DEBUG_NAME,  &sedebugnameValue))
	{
		::CloseHandle(hToken);
		return bRet;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	bRet = ::AdjustTokenPrivileges( hToken, FALSE,  & tkp,  sizeof (tkp) , NULL, NULL);
	if ( !bRet )
	{
		::CloseHandle( hToken );
	}
	return bRet;  
} 