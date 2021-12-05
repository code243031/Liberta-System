#include "pch.h"
#include "CConnectSocket.h"
#include "Liberta_clientDlg.h"

void CConnectSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
        pMain->m_chat.SetWindowTextW(szBuffer);          // ����Ʈ�� ���ڿ��� �߰��Ѵ�.
    }
    CSocket::OnReceive(nErrorCode);
}
