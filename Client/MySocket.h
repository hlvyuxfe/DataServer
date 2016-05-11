#pragma once
#include "Afxsock.h"
#include "ClientDlg.h"
// MySocket ÃüÁîÄ¿±ê

class CClientDlg;

class MySocket : public CAsyncSocket
{
public:
	MySocket();
	virtual ~MySocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);

	CClientDlg *m_dlg;
	void GetDlg(CClientDlg *dlg);

};


