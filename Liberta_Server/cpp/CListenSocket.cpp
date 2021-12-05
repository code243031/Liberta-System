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

    if (Accept(*pClient)) { // 클라이언트 접속 요청이 오면 서버-클라이언트를 연결시켜준다
        pClient->SetListenSocket(this);
        m_ptrClientSocketList.AddTail(pClient); // 리스트에 클라이언트 소켓 저장
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
            // 클라이언트 연결중지후 종료
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
            // Send함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
            // 이 함수는 전송한 데이터의 길이를 반환한다.
            int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
            if (checkLenOfData != lstrlen(pszMessage) * 2) {
                AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다!"));
            }
        }
    }
}
