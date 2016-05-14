
// ClientDlg.h : 头文件
//

#pragma once
#include "MySocket.h"
#include "afxwin.h"
#include "zip_helper.h"
#include "Mycryptopp.h"

class MySocket;

/*
消息协议头
*/
typedef struct tagHeader
{
	char type;//消息类型
	UINT length;//消息长度
	char name[27] = { 0 };//文件名称
}Header, *pHeader;

#define RSAKEY  1
#define AESKEY  2
#define DATA    3
#define COMMOND 4
#define CONNECT 5

#define START 0xc0
#define STOP  0x80

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	MySocket *m_ClientSocket;
	void OnReceive();
	void OnClose();
	void OnConnect(int nErrorCode);
	void SocketReset();
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

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
