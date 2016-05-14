
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "MySocket.h"
#include "afxwin.h"
#include "zip_helper.h"
#include "Mycryptopp.h"

class MySocket;

/*
��ϢЭ��ͷ
*/
typedef struct tagHeader
{
	char type;//��Ϣ����
	UINT length;//��Ϣ����
	char name[27] = { 0 };//�ļ�����
}Header, *pHeader;

#define RSAKEY  1
#define AESKEY  2
#define DATA    3
#define COMMOND 4
#define CONNECT 5

#define START 0xc0
#define STOP  0x80

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
	float datagather[9] = { 0 };
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool SendFile(string FilePath);
};
