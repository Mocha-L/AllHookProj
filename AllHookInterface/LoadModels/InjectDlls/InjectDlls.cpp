#include "stdafx.h"
#include "WstringHelper.h"
#include "InjectDlls.h"
  
/***************************** 
*��������GetProcessIdByName 
*��  �ܣ����ݽ��������ҽ���ID 
*��  �Σ�const char*ProcessName�������� 
*��  �Σ��� 
*����ֵ������ID��ʧ�ܷ���-1 
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
*��������FindModelByProcessId 
*��  �ܣ����ݽ���������ģ�� 
*��  �Σ�DWORD dwProcessID,const wchar_t* pDllName,
*��  �Σ�MODULEENTRY32W &me
*����ֵ������ID��ʧ�ܷ���-1 
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
*��������DllInject 
*��  �ܣ���dllע�뵽ָ���Ľ����� 
*��  �Σ�const char*ProcessName�������� 
        const char *pDllName��dll�� 
*��  �Σ��� 
*����ֵ���ɹ�����0��ʧ�ܷ���-1 
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
    //���ݽ�������ȡ����ID  
    dwProcessID = GetProcessIdByName(pProcessName);  
    if(dwProcessID == -1)  
    {  
        OutputDebugString(stringformat(L"%sδ����", pProcessName).c_str());
        iRet = -1;
		goto _DllInject_End;
    }
	OutputDebugString(stringformat(L"%s����IDΪ%d", pProcessName,dwProcessID).c_str());

    //���ݽ���ID��ȡ���̾��  
    hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);  
    if(hProcessHandle == NULL)  
    {  
        OutputDebugString(L"OpenProcess��ȡ���̾��ʧ��\n");  
		iRet = -1;
		goto _DllInject_End;
    }

#ifdef UNICODE
	dwSize = (lstrlen(pDllName) + 1) * 2; //���ַ�ռ��2���ֽ�
#else
	dwSize = lstrlen(pDllName) + 1;
#endif

    //��VirtualAllocEx�ڽ����������ڴ�  
    pAddrStart = VirtualAllocEx(hProcessHandle,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);  
    if (pAddrStart == NULL)  
    {
        OutputDebugString(L"�����ڴ�����ʧ��!\n");  
		iRet = -1;
		goto _DllInject_End; 
    }
  
    //����Ҫ���е�dll��д��������ڴ��ַ  
    bSuccess = WriteProcessMemory(hProcessHandle,pAddrStart,pDllName,dwSize,0);  
    if(!bSuccess)  
    {  
        OutputDebugString(L"WriteProcessMemoryʧ�ܣ�\n");  
		iRet = -1;
		goto _DllInject_End;
    }  
    //printf("memory of pAddrStart is:%s",pAddrStart);  
  
    //ע��,��"LoadLibraryW"��������dll  
    hThreadHandle = CreateRemoteThread(hProcessHandle,  
                                       0,  
                                       0,  
                                       (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"),"LoadLibraryW"),//����LoadLibraryW�ĵ�ַ  
                                       pAddrStart,//dll
                                       0,  
                                       0);  
    if(hThreadHandle == NULL)  
    {
		OutputDebugString(stringformat(L"�ڽ���%s��ע��%sʧ�� Getlasterror:%d",pProcessName,pDllName,GetLastError()).c_str());
		iRet = -1;
		goto _DllInject_End;
    }  
    WaitForSingleObject(hThreadHandle,INFINITE);  
    //�������Ѿ����dll�ļ��ؼ�ע���ˣ�ͨ��dll����ִ������Ҫ��ɵ�����   
    
_DllInject_End:

	//�ͷ� 
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
*��������DllFree 
*��  �ܣ�ж��ע�뵽�����е�dll 
*��  �Σ�const char*ProcessName�������� 
        const char *pDllName��dll�� 
*��  �Σ��� 
*����ֵ���ɹ�����0��ʧ�ܷ���-1 
*****************************/  
int DllFree(const wchar_t *pProcessName, const wchar_t *pDllName)  
{
	int iRet = 0;
	HANDLE hProcessHandle=NULL;  
	HANDLE hThreadHandle=NULL; 
    BOOL bSuccess = FALSE;
	DWORD dwHandle = 0;
	DWORD dwSize = 0;

    //���ݽ�������ȡ����ID
	DWORD dwProcessID=0;
    dwProcessID = GetProcessIdByName(pProcessName);  
    if(dwProcessID == -1)  
    {  
        OutputDebugString(stringformat(L"%sδ����", pProcessName).c_str());
        iRet = -1;
		goto _DllFree_End;
    }  
	OutputDebugString(stringformat(L"%s����IDΪ%d", pProcessName,dwProcessID).c_str());

	MODULEENTRY32W me;
	me.dwSize = sizeof(me);
	BOOL bFound = FindModelByProcessId(dwProcessID,pDllName,me);
	if (!bFound)
	{
		OutputDebugString(L"û�ҵ�ָ��ģ��");
		iRet = -1;
		goto _DllFree_End;
	}

	//���ݽ���ID��ȡ���̾��  
	hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);  
	if(hProcessHandle == NULL)  
	{
		OutputDebugString(L"OpenProcess��ȡ���̾��ʧ��\n");
		iRet = -1;
		goto _DllFree_End;
	}

    //��FreeLibraryע�뵽���̣��ͷ�ע���DLL
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

//�޸Ľ���Ȩ��  
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