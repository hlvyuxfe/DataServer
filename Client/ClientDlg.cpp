
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include <random>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <bitset>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using std::ofstream;
using std::bitset;
/*
Global 
*/

#define ReadSize 1024*4//�ļ�һ�ζ�ȡ����

bool Connected = false;//����״̬���������ð�ť״̬

bool sending = false;//����״̬������Ƿ��ڷ�����

MyRSAPublic public_key;
MyAES aes;

unsigned short bitswitch;

std::default_random_engine generator(time(NULL));
std::uniform_int_distribution<size_t> axis_dist(0,65535);
std::uniform_real_distribution<double> distribution(0.0, 1.0);

//end of global

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClientDlg �Ի���


CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, DomainString(_T("1k49g77592.iask.in"))
	, Port(31903)
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
	DDX_Text(pDX, IDC_DATA0, datagather[0]);
	DDX_Text(pDX, IDC_DATA1, datagather[1]);
	DDX_Text(pDX, IDC_DATA2, datagather[2]);
	DDX_Text(pDX, IDC_DATA3, datagather[3]);
	DDX_Text(pDX, IDC_DATA4, datagather[4]);
	DDX_Text(pDX, IDC_DATA5, datagather[5]);
	DDX_Text(pDX, IDC_DATA6, datagather[6]);
	DDX_Text(pDX, IDC_DATA7, datagather[7]);
	DDX_Text(pDX, IDC_DATA8, datagather[8]);

}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedExit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ClientSocket = NULL;
	aes.GenerateKey();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClientDlg::OnBnClickedConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		}
		else
		{
			AfxMessageBox("Get Address Failed!");
			return;
		}
	}
	else
	{
		OnClose();
	}
}


void CClientDlg::OnReceive()
{
	char HeaderBuff[sizeof(Header)];
	ZeroMemory(HeaderBuff, sizeof(HeaderBuff));
	m_ClientSocket->Receive(HeaderBuff, sizeof(Header));
	m_ClientSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
	Header* header = (Header*)HeaderBuff;
	char type = header->type;
	UINT length = header->length;
	if (type == RSAKEY)
	{
		char *Rsakeybuff = new char[length+1];
		ZeroMemory(Rsakeybuff, length+1);
		while (SOCKET_ERROR == (m_ClientSocket->Receive(Rsakeybuff, length)));
		m_ClientSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
		string Modulus(Rsakeybuff);
		public_key.SetPublic(Modulus,"17");
		delete[]Rsakeybuff;
		string cliperkey=public_key.encrypt(reinterpret_cast<char const*>(aes.key));
		Header Aeshead;
		Aeshead.type = AESKEY;
		Aeshead.length = cliperkey.size();
		m_ClientSocket->Send((char*)&Aeshead,sizeof(Header));
		m_ClientSocket->Send(cliperkey.c_str(), cliperkey.size());

	}
	else if (type == COMMOND)
	{
		byte commond=0;
		m_ClientSocket->Receive(&commond, length);
		if (commond & 0x80)
		{//1*******����
			if (commond & 0x40)
			{//11******����
				Connected = true;
				CWnd* MyWnd = GetDlgItem(IDC_BUTTON1);
				if (Connected == true)
				{
					MyWnd->SetWindowTextA("Close");
					DomainControl.EnableWindow(false);
					PortControl.EnableWindow(false);
					//OnTimer(1);
					SetTimer(1, 1000, NULL);
				}
			}
			else
			{//10******
				if (commond & 0x20)
				{//101*****ֹͣ
					KillTimer(1);
				}
				else
				{//100*****�Ͽ�
					OnClose();
				}
			}
		}
		else
		{//0******������

		}
	}
}

void CClientDlg::OnClose()
{
	while (sending);
	SocketReset();
	Connected = false;
	CWnd* MyWnd = GetDlgItem(IDC_BUTTON1);
	if (Connected == false)
	{
		MyWnd->SetWindowTextA("Connect");
		KillTimer(1);
	}
}


void CClientDlg::OnConnect(int nErrorCode)
{
	if (nErrorCode)
	{
		AfxMessageBox("connect error!");
		return;
	}
	Header ConnectHeader;
	ConnectHeader.type = CONNECT;
	m_ClientSocket->Send((char*)&ConnectHeader, sizeof(Header));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	while (sending);
	SocketReset();
	CDialogEx::OnCancel();
}

void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	sending = true;
	CDialogEx::OnTimer(nIDEvent);
	CTime curtime;
	curtime = CTime::GetCurrentTime();
	bitswitch = axis_dist(generator);
	for (auto &i : datagather)
	{
		i = distribution(generator);
	}
	bitset<16> bit(bitswitch);
	CWnd* MyWnd = GetDlgItem(IDC_STATIC_BITSWITCH);
	MyWnd->SetWindowText(bit.to_string().c_str());
	UpdateData(false);
	CString FileName = curtime.Format(_T("%Y%m%d%H%M%S.dat"));
	ofstream file(curtime.Format(_T("%Y%m%d%H%M%S.dat")),std::ios::binary);
	for (auto i : datagather)
	{
		file << i;
	}
	file << '|';
	file << bitswitch;
	file.close();
	ZipHelper z;
	z.AddFile(FileName.GetBuffer());
	z.ToZip(FileName + ".zip");
	DeleteFile(FileName);//Դ�ļ�
	FileName += ".zip";
	string Afteraes = FileName.GetBuffer();
	Afteraes += ".aes";
	aes.EncryptFile(FileName.GetBuffer(), Afteraes);
	DeleteFile(FileName);//ѹ����
	SendFile(Afteraes);
	DeleteFile(Afteraes.c_str());//���ܺ�
	sending = false;

}


bool CClientDlg::SendFile(string FilePath)
{
	CFile file;
	if (!file.Open(FilePath.c_str(), CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("�ļ�������"));
		return false;
	}
	Header FileHeader;
	FileHeader.type = DATA;
	FileHeader.length =file.GetLength();
	memcpy(FileHeader.name, FilePath.c_str(), 27);
	m_ClientSocket->Send((char*)&FileHeader, sizeof(Header));
	LPBYTE data = new BYTE[ReadSize]; // ���ڴ�Ŷ�����ļ����ݿ�
	UINT ByteSended = 0, count;
	while (ByteSended < FileHeader.length)
	{
		count=file.Read(data, ReadSize);
		while (SOCKET_ERROR == m_ClientSocket->Send(data, count))
		{
		}
		ByteSended += count;
	}
	if (data)
	{
		delete[] data;
		data = NULL;
	}
	file.Close();
	return true;
}