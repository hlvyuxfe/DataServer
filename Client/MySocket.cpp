// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "MySocket.h"


// MySocket

MySocket::MySocket()
{
}

MySocket::~MySocket()
{
}

void MySocket::OnClose(int nErrorCode)
{
	m_dlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

void MySocket::OnReceive(int nErrorCode)
{
	m_dlg->OnReceive();
	AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnConnect(int nErrorCode)
{
	m_dlg->OnConnect(nErrorCode);
	CAsyncSocket::OnConnect(nErrorCode);
}

void MySocket::GetDlg(CClientDlg * dlg)
{
	m_dlg = dlg;
}


// MySocket 成员函数
