
// AllHookInterface.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAllHookInterfaceApp:
// �йش����ʵ�֣������ AllHookInterface.cpp
//

class CAllHookInterfaceApp : public CWinAppEx
{
public:
	CAllHookInterfaceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAllHookInterfaceApp theApp;