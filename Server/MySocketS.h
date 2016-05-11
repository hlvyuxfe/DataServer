#pragma once
#include "Afxsock.h"
#include "ServerDlg.h"

// MySocketS ÃüÁîÄ¿±ê

class CServerDlg;

class MySocketS : public CAsyncSocket
{
public:
	MySocketS();
	virtual ~MySocketS();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	CServerDlg *m_dlg;
	void GetDlg(CServerDlg *dlg);
};


