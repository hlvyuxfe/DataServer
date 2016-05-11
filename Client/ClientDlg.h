
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "MySocket.h"
#include "afxwin.h"
#include "zip_helper.h"
#include "Mycryptopp.h"

class MySocket;

// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	MySocket *m_ClientSocket;
	void OnReceive();
	void OnClose();
	void OnConnect(int nErrorCode);
	void SocketReset();
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
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
	CEdit DomainControl;
	CEdit PortControl;
	CString DomainString;
	int Port;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
