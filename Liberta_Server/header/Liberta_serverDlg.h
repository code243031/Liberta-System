// Liberta_serverDlg.h: 헤더 파일
//
#pragma once
#pragma warning(disable : 4996)

#include <thread>
#include <opencv2/opencv.hpp>
#include <locale.h>
#include <WinSock2.h>
#include <string.h>

#define BUFSIZE 44000
#define UM_RECVIMG WM_USER + 1

using namespace cv;
using namespace std;

// CLibertaserverDlg 대화 상자
class CLibertaserverDlg : public CDialogEx
{
// 생성입니다.
public:
	CLibertaserverDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CLibertaserverDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBERTA_SERVER_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	afx_msg void OnDestroy();

// 구현입니다.
protected:
	HICON m_hIcon;
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	// chat socket
	WSADATA wsdata;
	SOCKET m_socketServer;
	SOCKET m_sockClient;
	sockaddr accept_addr;
	bool connect;

	CString str_chat;

	// video socket
	WSADATA wsdata_v;
	SOCKET m_socketServer_v;
	SOCKET m_sockClient_v;
	sockaddr accept_addr_v;
	bool connect_v;

public:
	vector<CString> sendBuf;

	afx_msg void OnEnChangeChat();
	afx_msg void OnEnChangeType();
	afx_msg void OnBnClickedOk();

	afx_msg void OnStnClickedDoc();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	// session create
	bool initSession();
	bool initVideoSession();

	CStatic m_video;
	CStatic m_video_pac;
	
	CEdit m_Chat;

	VideoCapture* capture;
	// doc
	Mat mat_frame;
	CImage cimage_mfc;
	
	//pac
	Mat mat_recv;
	CImage cimage_recv;

protected:
	
public:
	afx_msg void OnBnClickedCancel();
protected:
	afx_msg LRESULT OnUmRecvimg(WPARAM wParam, LPARAM lParam);

};
