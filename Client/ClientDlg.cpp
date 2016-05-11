
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool Connected = false;//连接状态，用于设置按钮状态


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg 对话框


CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, DomainString(_T("localhost"))
	, Port(52535)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, DomainControl);
	DDX_Control(pDX, IDC_EDIT2, PortControl);
	DDX_Text(pDX, IDC_EDIT1, DomainString);
	DDX_Text(pDX, IDC_EDIT2, Port);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedExit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ClientSocket = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClientDlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Connected == false)
	{
		if (!AfxSocketInit())
		{
			MessageBox("winsocket initial failed", "Receive", MB_ICONSTOP);
			return;
		}
		m_ClientSocket = new MySocket;
		m_ClientSocket->GetDlg(this);
		m_ClientSocket->Create();
		UpdateData();
		struct addrinfo hints, *res;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_STREAM;
		if (!getaddrinfo(DomainString, NULL, &hints, &res))
		{
			struct sockaddr_in  *sockaddr_ipv4;
			sockaddr_ipv4 = (struct sockaddr_in *) res->ai_addr;
			char addr_in_p[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, addr_in_p, INET_ADDRSTRLEN);
			m_ClientSocket->Connect(addr_in_p, Port);
			//m_ListWords.AddString(addr_in_p);
			//m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
		}
		else
		{
			AfxMessageBox("Get Address Failed!");
			return;
		}
	}
	else
	{
		SocketReset();
		Connected = false;
		CWnd* MyWnd = GetDlgItem(IDC_BUTTON1);
		if (Connected == false)
		{
			MyWnd->SetWindowTextA("Connect");
		}
	}
}


void CClientDlg::OnReceive()
{
	char szTemp[200];
	int n = m_ClientSocket->Receive(szTemp, 200);
	szTemp[n] = '\0';	
	//
}

void CClientDlg::OnClose()
{
	SocketReset();
	Connected = false;
	CWnd* MyWnd = GetDlgItem(IDC_BUTTON1);
	if (Connected == false)
	{
		MyWnd->SetWindowTextA("Connect");
	}
}

void CClientDlg::OnConnect(int nErrorCode)
{
	if (nErrorCode)
	{
		AfxMessageBox("connect error!");
		return;
	}
	else
	{
		Connected = true;
		CWnd* MyWnd = GetDlgItem(IDC_BUTTON1);
		if (Connected == true)
		{
			MyWnd->SetWindowTextA("Close");
			SetTimer(1, 1000, NULL);
		}
	}
}

void CClientDlg::SocketReset()
{
	if (m_ClientSocket != NULL)
	{
		delete m_ClientSocket;
		m_ClientSocket = NULL;
	}
}

void CClientDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	SocketReset();
	CDialogEx::OnCancel();
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
	CTime curtime;
	curtime = CTime::GetCurrentTime();
	//m_sWords = curtime.Format(_T("%Y%m%d%H%M%S"));
	UpdateData(false);
}
