
// ServerDlg.h : ͷ�ļ�
//

#pragma once

#include "MySocketS.h"
#include <map>
#include "afxwin.h"
#include "zip_helper.h"
#include "Mycryptopp.h"

class MySocketS;
using std::map;
/*
��ϢЭ��ͷ
*/
typedef struct tagHeader
{
	char type;//��Ϣ����
	UINT length;//��Ϣ����
	char name[27] = { 0 };//��Ϣ����
}Header,*pHeader;

#define RSAKEY  1
#define AESKEY  2
#define DATA    3
#define COMMOND 4

#define START 0xc0
#define STOP  0x80

// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	MySocketS *m_ListenSocket;
	map<UINT, MySocketS*> m_ServerSocketMap;


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

	void OnReceive(UINT);
	void OnClose(UINT);
	void OnAccept();
	void SocketReset();
	afx_msg void OnBnClickedExit();
	bool ReceiveFile(UINT,Header FileHeader);
};
