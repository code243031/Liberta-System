#include "pch.h"
#include "CConnectSocket.h"
#include "Liberta_clientDlg.h"

void CConnectSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR:Disconnected from server!"));
	::PostQuitMessage(0);
}

void CConnectSocket::OnReceive(int nErrorCode)
{
    TCHAR szBuffer[1024];
    ::ZeroMemory(szBuffer, sizeof(szBuffer));

    if (Receive(szBuffer, sizeof(szBuffer)) > 0) {
        CLibertaclientDlg* pMain = (CLibertaclientDlg*)AfxGetMainWnd();
        pMain->m_chat.SetWindowTextW(szBuffer);          // 리스트에 문자열을 추가한다.
    }
    CSocket::OnReceive(nErrorCode);
}
