
// ServerDlg.h : 头文件
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
消息协议头
*/
typedef struct tagHeader
{
	char type;//消息类型
	UINT length;//消息长度
	char name[27] = { 0 };//消息名称
}Header,*pHeader;

#define RSAKEY  1
#define AESKEY  2
#define DATA    3
#define COMMOND 4

#define START 0xc0
#define STOP  0x80

// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	MySocketS *m_ListenSocket;
	map<UINT, MySocketS*> m_ServerSocketMap;


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };

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
