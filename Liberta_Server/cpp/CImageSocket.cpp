#include "pch.h"
#include "CImageSocket.h"
#include "CClientSocket.h"

CImageSocket::CImageSocket()
{
}

CImageSocket::~CImageSocket()
{
}

void CImageSocket::OnAccept(int nErrorCode)
{
    CClientSocket* pClient = new CClientSocket;
    CString str;

    if (Accept(*pClient)) { // Ŭ���̾�Ʈ ���� ��û�� ���� ����-Ŭ���̾�Ʈ�� ��������ش�
        pClient->SetListenSocket(this);
        m_ptrImageSocketList.AddTail(pClient); // ����Ʈ�� Ŭ���̾�Ʈ ���� ����
    }
    else {
        delete pClient;
        AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
    }

    CAsyncSocket::OnAccept(nErrorCode);
	CSocket::OnAccept(nErrorCode);
}

void CImageSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CSocket::OnReceive(nErrorCode);
}

void CImageSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CSocket::OnClose(nErrorCode);
}


void CImageSocket::OnSend(int nErrorCode)
{
    

    CSocket::OnSend(nErrorCode);
}
