#pragma once
#include <afxsock.h>
class CImageSocket :
    public CSocket
{
public:
    CImageSocket();
    ~CImageSocket();

    CPtrList m_ptrImageSocketList;
    virtual void OnAccept(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
    virtual void OnClose(int nErrorCode);
    virtual void OnSend(int nErrorCode);
};

