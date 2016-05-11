
// ServerDlg.h : 头文件
//

#pragma once

#include "MySocketS.h"
#include <map>
#include "afxwin.h"

class MySocketS;
using std::map;

// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	MySocketS *m_ListenSocket;
	map<int, MySocketS> m_ServerSocket;


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

	void OnReceive();
	void OnClose();
	void OnAccept();
	void SocketReset();
};
