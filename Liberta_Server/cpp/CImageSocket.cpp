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

    if (Accept(*pClient)) { // 클라이언트 접속 요청이 오면 서버-클라이언트를 연결시켜준다
        pClient->SetListenSocket(this);
        m_ptrImageSocketList.AddTail(pClient); // 리스트에 클라이언트 소켓 저장
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnReceive(nErrorCode);
}

void CImageSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnClose(nErrorCode);
}


void CImageSocket::OnSend(int nErrorCode)
{
    

    CSocket::OnSend(nErrorCode);
}
