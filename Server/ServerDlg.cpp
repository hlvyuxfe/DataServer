
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include <string>
#include <cstring>
#include <istream>
#include <ios>
#include <bitset>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//变量定义开始
using std::string;
using CryptoPP::AES;
using std::ifstream;
using std::bitset;

#define ReadSize 1024*4//文件一次读取长度

bool Listening=false;//监听状态

UINT ClientNumber=0;//客户端序号

MyRSA rsa(1024);//RSA初始化
byte iv[AES::BLOCKSIZE] = "Transmission";

//变量定义结束

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


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, Port(52535)
	, CommondString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, PortControl);
	DDX_Text(pDX, IDC_EDIT1, Port);
	DDX_Control(pDX, IDC_EDIT2, CommondControl);
	DDX_Text(pDX, IDC_EDIT2, CommondString);
	DDX_Text(pDX, IDC_DATA0, datagather[0]);
	DDX_Text(pDX, IDC_DATA1, datagather[1]);
	DDX_Text(pDX, IDC_DATA2, datagather[2]);
	DDX_Text(pDX, IDC_DATA3, datagather[3]);
	DDX_Text(pDX, IDC_DATA4, datagather[4]);
	DDX_Text(pDX, IDC_DATA5, datagather[5]);
	DDX_Text(pDX, IDC_DATA6, datagather[6]);
	DDX_Text(pDX, IDC_DATA7, datagather[7]);
	DDX_Text(pDX, IDC_DATA8, datagather[8]);
	DDX_Control(pDX, IDC_BUTTON2, SendControl);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CServerDlg::OnBnClickedStartListen)
	ON_BN_CLICKED(IDCANCEL, &CServerDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_BUTTON2, &CServerDlg::OnBnClickedSendCommond)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
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
	m_ListenSocket = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnBnClickedStartListen()
{
	// TODO: 在此添加控件通知处理程序代码
	Listening = !Listening;
	CWnd* MyWnd = GetDlgItem(IDC_BUTTON1);
	if (Listening == true)
	{
		if (!AfxSocketInit())
		{
			MessageBox("winsocket initial failed", "Receive", MB_ICONSTOP);
			return;
		}
		m_ListenSocket = new MySocketS;
		m_ListenSocket->GetDlg(this);
		CString sIP("0.0.0.0"), sP;
		UpdateData();
		sP.Format("%d", Port);
		m_ListenSocket->Create(Port, SOCK_STREAM, FD_ACCEPT, sIP);
		m_ListenSocket->Listen(5);
		MyWnd->SetWindowTextA("Stop");
		PortControl.EnableWindow(false);
		CommondControl.EnableWindow(true);
		SendControl.EnableWindow(true);
	}
	else
	{
		if (m_ListenSocket != NULL)
		{
			m_ListenSocket->Close();
			delete m_ListenSocket;
			m_ListenSocket = NULL;
		}
		MyWnd->SetWindowTextA("Start");
		PortControl.EnableWindow();
		CommondControl.EnableWindow(false);
		SendControl.EnableWindow(false);
	}
}

void CServerDlg::OnReceive(UINT ClientNumber)
{
	if (m_ServerSocketMap[ClientNumber] != NULL)
	{
		char HeaderBuff[sizeof(Header)];
		ZeroMemory(HeaderBuff, sizeof(HeaderBuff));
		m_ServerSocketMap[ClientNumber]->Receive(HeaderBuff, sizeof(Header));
		m_ServerSocketMap[ClientNumber]->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
		Header* header = (Header*)HeaderBuff;
		char type = header->type;
		UINT length = header->length;
		if (type == AESKEY)
		{
			char *aeskeybuff = new char[length];
			ZeroMemory(aeskeybuff, length);
			int n=m_ServerSocketMap[ClientNumber]->Receive(aeskeybuff, length);
			m_ServerSocketMap[ClientNumber]->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
			string chiper;
			for (size_t i = 0; i < length; i++)
			{
				chiper.append(1, aeskeybuff[i]);
			}
			string aeskey= rsa.decrypt(chiper);
			m_ServerSocketMap[ClientNumber]->aes.SetKey((byte*)aeskey.c_str(), iv, 16);
			delete[]aeskeybuff;
			byte commond=START;
			Header ComHeader;
			ComHeader.type = COMMOND;
			ComHeader.length = sizeof(byte);
			m_ServerSocketMap[ClientNumber]->Send((char*)&ComHeader, sizeof(Header));
			m_ServerSocketMap[ClientNumber]->Send(&commond, sizeof(byte));
		}
		else if (type ==DATA )
		{
			ReceiveFile(ClientNumber,*header);
			string FileName;
			FileName.assign(header->name, 0, 22);
			m_ServerSocketMap[ClientNumber]->aes.DecryptFile(header->name, FileName);
			DeleteFile(header->name);//解密后
			UnZipHelper z;
			z.OpenZip(FileName.c_str());
			FileName=z.UnZip();
			DeleteFile((FileName+".zip").c_str());
			if (m_ServerSocketMap.begin()->first == ClientNumber)
			{
				UpdateData();
				unsigned short bitswitch;
				ifstream file(FileName,std::ios::binary);
				for (auto &i : datagather)
				{
					file >> i;
				}
				char temp;
				file >> temp;
				file >> bitswitch;
				file.close();
				DeleteFile(FileName.c_str());
				UpdateData(false);
				bitset<16> bit(bitswitch);
				CWnd* MyWnd = GetDlgItem(IDC_STATIC_BITSWITCH);
				MyWnd->SetWindowText(bit.to_string().c_str());
			}
		}
		else if (type = CONNECT)
		{
			string Modulus = rsa.getModulus();
			UINT ModulusLength = Modulus.size();
			Header head;
			head.type = RSAKEY;
			head.length = ModulusLength;
			m_ServerSocketMap[ClientNumber]->Send((char*)&head, sizeof(Header));
			m_ServerSocketMap[ClientNumber]->Send(Modulus.c_str(), ModulusLength);
		}
	}
}

void CServerDlg::OnClose(UINT ClientNumber)
{
	if (m_ServerSocketMap[ClientNumber] != NULL)
	{
		delete m_ServerSocketMap[ClientNumber];
		m_ServerSocketMap.erase(ClientNumber);
		//MessageBox("client close the connect", "Receive");
		//m_ServerSocket = NULL;
	}
}

void CServerDlg::OnAccept()
{
	MySocketS *m_ServerSocket = new MySocketS;
	m_ServerSocket->GetDlg(this);
	m_ListenSocket->Accept(*m_ServerSocket);
	m_ServerSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
	m_ServerSocket->ClientNumber= ClientNumber++;
	m_ServerSocketMap.insert({ m_ServerSocket->ClientNumber ,m_ServerSocket });

	//CString message;
	//message.Format("accept an connect %d", m_ServerSocket->ClientNumber);
	//MessageBox(message, "Receive");
}

void CServerDlg::SocketReset()
{
	auto map_it = m_ServerSocketMap.cbegin();
	while (map_it != m_ServerSocketMap.cend())
	{
		delete map_it->second;
		m_ServerSocketMap.erase(map_it->first);
	}
}


void CServerDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	SocketReset();
	if (m_ListenSocket != NULL)
	{
		delete m_ListenSocket;
		m_ListenSocket = NULL;
	}
	CDialogEx::OnCancel();
}

bool CServerDlg::ReceiveFile(UINT ClientNumber,Header FileHeader)
{
	CFile file;
	if (file.Open(FileHeader.name, CFile::modeCreate | CFile::modeWrite))
	{
		LPBYTE data = new BYTE[ReadSize];
		ULONGLONG ByteReceived = 0;
		int count = 0;
		while (ByteReceived < FileHeader.length)
		{
			count = m_ServerSocketMap[ClientNumber]->Receive(data, ReadSize);
			if (count == SOCKET_ERROR)
				continue;
			file.Write(data, count);//写入文件
			file.Flush();
			ByteReceived += count;
		}
		if (data)
		{
			delete[]  data;
			data = NULL;
		}
		file.Close();
	}
	return true;
}

void CServerDlg::OnBnClickedSendCommond()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	byte commond = strtol(CommondString.GetBuffer(),NULL,2);
	Header ComHeader;
	ComHeader.type = COMMOND;
	ComHeader.length = sizeof(byte);
	for (auto i : m_ServerSocketMap)
	{
		i.second->Send((char*)&ComHeader, sizeof(Header));
		i.second->Send(&commond, sizeof(byte));
	}
}
