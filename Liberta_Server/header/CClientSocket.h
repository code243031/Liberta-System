#pragma once
#include <afxsock.h>
class CClientSocket : public CSocket
{
public:
    CAsyncSocket* m_pListenSocket;
    void SetListenSocket(CAsyncSocket* pSocket);
    virtual void OnClose(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
};

