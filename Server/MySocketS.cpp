// MySocketS.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "MySocketS.h"


// MySocketS

MySocketS::MySocketS()
{
}

MySocketS::~MySocketS()
{
}

void MySocketS::OnAccept(int nErrorCode)
{
		m_dlg->OnAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}

void MySocketS::OnClose(int nErrorCode)
{
	m_dlg->OnClose(this->ClientNumber);
	CAsyncSocket::OnClose(nErrorCode);
}

void MySocketS::OnReceive(int nErrorCode)
{
	m_dlg->OnReceive(this->ClientNumber);
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocketS::GetDlg(CServerDlg * dlg)
{
	m_dlg = dlg;
}


// MySocketS 成员函数
