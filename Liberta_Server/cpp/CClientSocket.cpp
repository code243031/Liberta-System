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

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReciev(int nErrorCode)
{
    CString strTmp = _T(""), strIPAddress = _T("");
    UINT uPortNumber = 0;
    TCHAR strBuffer[1024];
    ::ZeroMemory(strBuffer, sizeof(strBuffer)); // :: 붙이고 안붙이고 차이 알아보기

    GetPeerName(strIPAddress, uPortNumber);
    if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
        CLibertaserverDlg* pMain = (CLibertaserverDlg*)AfxGetMainWnd();
        strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);
        pMain->m_List.AddString(strTmp);  // 메시지 리스트(메시지창?)에 입력받은 메시지 띄우기
        pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

        CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
        pServerSocket->SendAllMessage(strBuffer); // 다른 클라이언트들에게 메시지 전달
    }

    CSocket::OnReceive(nErrorCode);
}
