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
	/*if (m_dlg->m_ServerSocket == NULL)
	{
		m_dlg->OnAccept();
	}*/
	CAsyncSocket::OnAccept(nErrorCode);
}

void MySocketS::OnClose(int nErrorCode)
{
	m_dlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

void MySocketS::OnReceive(int nErrorCode)
{
	m_dlg->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocketS::GetDlg(CServerDlg * dlg)
{
	m_dlg = dlg;
}


// MySocketS 成员函数
