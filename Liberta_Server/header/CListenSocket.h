#pragma once
#include <afxsock.h>
class CListenSocket : public CAsyncSocket
{
public:
    CListenSocket();
    ~CListenSocket();

    CPtrList m_ptrClientSocketList;
    virtual void OnAccept(int nErrorCode);
    void CloseClientSocket(CSocket* pClient);
    void SendAllMessage(TCHAR* pszMessage);
};

