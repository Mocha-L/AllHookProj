
// AllHookInterfaceDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define HOOK 0x10
#define UNHOOK 0x20
#define SET_MOUSE		0x01
#define SET_KEYBOARD	0x02

#define WM_BTN_UNHOOKMOUSE (WM_USER+10)
#define WM_SENDWORDCODE (WM_USER+11)
// CAllHookInterfaceDlg 对话框
class CAllHookInterfaceDlg : public CDialog
{
// 构造
public:
	CAllHookInterfaceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ALLHOOKINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_HookKeyBrd;
	CButton m_UNHookKeyBoard;
	CButton m_HookMouse;
	afx_msg void OnBnClickedButtonHookkeyboard();
	afx_msg void OnBnClickedButtonUnhookkeyboard();
	afx_msg void OnBnClickedButtonHookmouse();
	afx_msg void OnBnClickedButtonUnhookmouse();
	afx_msg LRESULT OnMyMessage_UnHookMouse(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMyMessage_HandleInput(WPARAM wParam,LPARAM lParam);
	CButton m_UnHookMouse;
	CEdit m_EditOutput;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnClose();

protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonApihook();
	CEdit m_editProcName;
};
