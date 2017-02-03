
// AllHookInterfaceDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define HOOK 0x10
#define UNHOOK 0x20
#define SET_MOUSE		0x01
#define SET_KEYBOARD	0x02

#define WM_BTN_UNHOOKMOUSE (WM_USER+10)
#define WM_SENDWORDCODE (WM_USER+11)
// CAllHookInterfaceDlg �Ի���
class CAllHookInterfaceDlg : public CDialog
{
// ����
public:
	CAllHookInterfaceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ALLHOOKINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
