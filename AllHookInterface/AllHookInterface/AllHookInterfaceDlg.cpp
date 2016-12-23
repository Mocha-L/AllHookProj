
// AllHookInterfaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows.h"
#include "AllHookInterface.h"
#include "AllHookInterfaceDlg.h"
#include "../LoadModels/KeyBoardMouse/LoadKeyBoardMouse.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAllHookInterfaceDlg �Ի���




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
	ON_MESSAGE(WM_BTN_UNHOOKMOUSE,CAllHookInterfaceDlg::OnMyMessage)
	ON_MESSAGE(WM_SENDWORDCODE,CAllHookInterfaceDlg::OnMyMessage2)
END_MESSAGE_MAP()


// CAllHookInterfaceDlg ��Ϣ��������

BOOL CAllHookInterfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	m_HookKeyBrd.ShowWindow(SW_SHOW);
	m_UNHookKeyBoard.ShowWindow(SW_HIDE);
	m_HookMouse.ShowWindow(SW_SHOW);
	m_UnHookMouse.ShowWindow(SW_HIDE);

	if(!KeyboardMouse_Hook::LoadKeyBoardMouse())
	{
		AfxMessageBox(L"Load dll fail!");
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAllHookInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: Add your control notification handler code herewoshi���Ǹ�����
	m_HookMouse.ShowWindow(SW_SHOW);
	m_UnHookMouse.ShowWindow(SW_HIDE);
	KeyboardMouse_Hook::SetHook(UNHOOK|SET_MOUSE,m_hWnd);
}

LRESULT CAllHookInterfaceDlg::OnMyMessage(WPARAM wParam,LPARAM lParam)
{
	OnBnClickedButtonUnhookmouse();
	return 0;
}
LRESULT CAllHookInterfaceDlg::OnMyMessage2(WPARAM wParam,LPARAM lParam)
{
	TCHAR szKeyName[20] = {0};
	CString strEditText = L"";
	GetKeyNameText(lParam,szKeyName,10);
	wstring strWord = szKeyName;
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

