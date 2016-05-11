
// ServerDlg.h : ͷ�ļ�
//

#pragma once

#include "MySocketS.h"
#include <map>
#include "afxwin.h"

class MySocketS;
using std::map;

// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	MySocketS *m_ListenSocket;
	map<int, MySocketS> m_ServerSocket;


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };

#endif

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
	CEdit PortControl;
	int Port;
	CEdit CommondControl;
	CString CommondString;
	afx_msg void OnBnClickedStartListen();

	void OnReceive();
	void OnClose();
	void OnAccept();
	void SocketReset();
};
