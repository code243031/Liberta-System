#include "pch.h"
#include "CClientSocket.h"
#include "CListenSocket.h"
#include "Liberta_serverDlg.h"

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}

void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*) m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReceive(int nErrorCode)
{
    CString str = _T(""), str_chat = _T(""), res = _T(""), strIPAddress = _T("");
    UINT uPortNumber = 0;
    TCHAR strBuffer[1024];
    ::ZeroMemory(strBuffer, sizeof(strBuffer)); // :: 붙이고 안붙이고 차이 알아보기

    GetPeerName(strIPAddress, uPortNumber);
    if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
        CLibertaserverDlg* pMain = (CLibertaserverDlg*)AfxGetMainWnd();

        str.Format(_T("[%s:%d]: %s\r\n"), strIPAddress, uPortNumber, strBuffer);

        pMain->m_Chat.GetWindowTextW(str_chat);

        res.Append(str_chat);
        res.Append(str);

        pMain->m_Chat.SetWindowTextW(res);
    }

    CSocket::OnReceive(nErrorCode);
}
