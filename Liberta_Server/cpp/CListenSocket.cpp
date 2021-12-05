#include "pch.h"
#include "CListenSocket.h"
#include "CClientSocket.h"
#include "Liberta_serverDlg.h"


CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}

void CListenSocket::OnAccept(int nErrorCode)
{
    CClientSocket* pClient = new CClientSocket;
    CString str;

    if (Accept(*pClient)) { // Ŭ���̾�Ʈ ���� ��û�� ���� ����-Ŭ���̾�Ʈ�� ��������ش�
        pClient->SetListenSocket(this);
        m_ptrClientSocketList.AddTail(pClient); // ����Ʈ�� Ŭ���̾�Ʈ ���� ����
    }
    else {
        delete pClient;
        AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
    }

    CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pClient)
{
    POSITION pos;
    pos = m_ptrClientSocketList.Find(pClient);

    if (pos != NULL) {
        if (pClient != NULL) {
            // Ŭ���̾�Ʈ ���������� ����
            pClient->ShutDown();
            pClient->Close();
        }

        CLibertaserverDlg* pMain = (CLibertaserverDlg*) AfxGetMainWnd();
        CString str1, str2;
        UINT indx = 0, posNum;

        m_ptrClientSocketList.RemoveAt(pos);
        delete pClient;
    }
}

void CListenSocket::SendAllMessage(TCHAR* pszMessage)
{
    POSITION pos;
    CString arr;

    pos = m_ptrClientSocketList.GetHeadPosition();
    CClientSocket* pClient = NULL;

    while (pos != NULL) {
        pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);

        if (pClient != NULL) {
            // Send�Լ��� �ι�° ���ڴ� �޸��� ũ���ε� �����ڵ带 ����ϰ� �����Ƿ� *2�� �� ũ�Ⱑ �ȴ�.
            // �� �Լ��� ������ �������� ���̸� ��ȯ�Ѵ�.
            int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
            if (checkLenOfData != lstrlen(pszMessage) * 2) {
                AfxMessageBox(_T("�Ϻ� �����Ͱ� ���������� ���۵��� ���߽��ϴ�!"));
            }
        }
    }
}
