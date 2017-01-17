
// AllHookInterfaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "windows.h"
#include "AllHookInterface.h"
#include "AllHookInterfaceDlg.h"
#include "../LoadModels/KeyBoardMouse/LoadKeyBoardMouse.h"
#include "../LoadModels/InjectDlls/InjectDlls.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAllHookInterfaceDlg 对话框




CAllHookInterfaceDlg::CAllHookInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllHookInterfaceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAllHookInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_HOOKKEYBOARD, m_HookKeyBrd);
	DDX_Control(pDX, IDC_BUTTON_UNHOOKKEYBOARD, m_UNHookKeyBoard);
	DDX_Control(pDX, IDC_BUTTON_HOOKMOUSE, m_HookMouse);
	DDX_Control(pDX, IDC_BUTTON_UNHOOKMOUSE, m_UnHookMouse);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_TEXT, m_EditOutput);
}

BEGIN_MESSAGE_MAP(CAllHookInterfaceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_HOOKKEYBOARD, &CAllHookInterfaceDlg::OnBnClickedButtonHookkeyboard)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOKKEYBOARD, &CAllHookInterfaceDlg::OnBnClickedButtonUnhookkeyboard)
	ON_BN_CLICKED(IDC_BUTTON_HOOKMOUSE, &CAllHookInterfaceDlg::OnBnClickedButtonHookmouse)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOKMOUSE, &CAllHookInterfaceDlg::OnBnClickedButtonUnhookmouse)
	ON_MESSAGE(WM_BTN_UNHOOKMOUSE,CAllHookInterfaceDlg::OnMyMessage_UnHookMouse)
	ON_MESSAGE(WM_SENDWORDCODE,CAllHookInterfaceDlg::OnMyMessage_HandleInput)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CAllHookInterfaceDlg::OnBnClickedButtonClear)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAllHookInterfaceDlg 消息处理程序

BOOL CAllHookInterfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	EnablePrivilege();

	m_HookKeyBrd.ShowWindow(SW_SHOW);
	m_UNHookKeyBoard.ShowWindow(SW_HIDE);
	m_HookMouse.ShowWindow(SW_SHOW);
	m_UnHookMouse.ShowWindow(SW_HIDE);

	if(!KeyboardMouse_Hook::LoadKeyBoardMouse())
	{
		AfxMessageBox(L"Load KeyBoardMouse Dll fail!");
	}
	wstring strDllPath = stringformat(L"%s%s",Path_GetCurrent().c_str(),L"APIHook.dll");
	if (DllInject(L"MFC_test.exe",strDllPath.c_str()) != 0)
	{
		AfxMessageBox(L"explorer.exe Load APIHook Dll fail!");
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAllHookInterfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAllHookInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAllHookInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAllHookInterfaceDlg::OnBnClickedButtonHookkeyboard()
{
	// TODO: Add your control notification handler code here
	m_HookKeyBrd.ShowWindow(SW_HIDE);
	m_UNHookKeyBoard.ShowWindow(SW_SHOW);
	KeyboardMouse_Hook::SetHook(HOOK|SET_KEYBOARD,m_hWnd);
}

void CAllHookInterfaceDlg::OnBnClickedButtonUnhookkeyboard()
{
	// TODO: Add your control notification handler code here
	m_HookKeyBrd.ShowWindow(SW_SHOW);
	m_UNHookKeyBoard.ShowWindow(SW_HIDE);
	KeyboardMouse_Hook::SetHook(UNHOOK|SET_KEYBOARD,m_hWnd);
}

void CAllHookInterfaceDlg::OnBnClickedButtonHookmouse()
{
	// TODO: Add your control notification handler code here
	m_HookMouse.ShowWindow(SW_HIDE);
	m_UnHookMouse.ShowWindow(SW_SHOW);
	KeyboardMouse_Hook::SetHook(HOOK|SET_MOUSE,m_hWnd);
}

void CAllHookInterfaceDlg::OnBnClickedButtonUnhookmouse()
{
	// TODO: Add your control notification handler code here
	m_HookMouse.ShowWindow(SW_SHOW);
	m_UnHookMouse.ShowWindow(SW_HIDE);
	KeyboardMouse_Hook::SetHook(UNHOOK|SET_MOUSE,m_hWnd);
}

LRESULT CAllHookInterfaceDlg::OnMyMessage_UnHookMouse(WPARAM wParam,LPARAM lParam)
{
	OnBnClickedButtonUnhookmouse();
	return 0;
}
LRESULT CAllHookInterfaceDlg::OnMyMessage_HandleInput(WPARAM wParam,LPARAM lParam)
{
	TCHAR szKeyName[20] = {0};
	CString strEditText = L"";
	GetKeyNameText(lParam,szKeyName,10);
	m_EditOutput.GetWindowText(strEditText);
	
	if (0 == wcscmp(szKeyName,L"Space"))
	{
		strEditText += L" ";
	}
	else if (0 == wcscmp(szKeyName,L"Backspace"))
	{
		wstring strTemp = strEditText.GetString();
		strTemp = strTemp.substr(0,strTemp.length()-1);
		strEditText = strTemp.c_str();
	}
	else if (0 == wcscmp(szKeyName,L"Enter"))
	{
		strEditText += L"\r\n";
	}
	else
	{
		strEditText += szKeyName;
	}
	m_EditOutput.SetWindowText(strEditText);
	return 0;
}   



void CAllHookInterfaceDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here
	m_EditOutput.SetWindowText(L"");
}

void CAllHookInterfaceDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(!KeyboardMouse_Hook::ReleaseKeyBoardMouse())
	{
		AfxMessageBox(L"Release KeyBoardMouse Dll fail!");
	}
	wstring strDllPath = stringformat(L"%s%s",Path_GetCurrent().c_str(),L"APIHook.dll");
	if (DllFree(L"MFC_test.exe",L"APIHook.dll") != 0)
	{
		AfxMessageBox(L"explorer.exe Free APIHook Dll fail!");
	}

	CDialog::OnClose();
}

void CAllHookInterfaceDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//do nothing
}

BOOL CAllHookInterfaceDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
