// Liberta_clientDlg.h: 헤더 파일
//
#pragma once
#pragma warning(disable : 4996)

#include <opencv2/opencv.hpp>
#include <thread>

#define BUFSIZE 44000
#define UM_RECVIMG WM_USER + 1
#define UM_SENDIMG WM_USER + 2

using namespace cv;
using namespace std;

// CLibertaclientDlg 대화 상자
class CLibertaclientDlg : public CDialogEx
{
// 생성입니다.
public:
	CLibertaclientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBERTA_CLIENT_DIALOG };
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
	afx_msg LRESULT OnUmRecvimg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// chat socket
	WSADATA wsdata;
	SOCKET m_socketClient;
	// sockaddr accept_addr;
	bool connect;

	// video socket(just send)
	WSADATA wsdata_v;
	SOCKET m_socketClient_v;
	// sockaddr accept_addr_v;
	bool connect_v;

	// video socket
	WSADATA wsdata_s;
	SOCKET m_socketClient_s;
	// sockaddr accept_addr_s;
	bool connect_s;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// session create
	bool initSession();
	void initVideoSession();

	CStatic m_video;
	CStatic m_video_pac;
	CEdit type;
	CEdit m_chat;

	CString str_chat;// 입력한 채팅 받아오기용

	// PAC(클라이언트 측 화면) 용 캠 이미지
	VideoCapture* capture;

	// pac
	Mat mat_frame;
	CImage cimage_mfc;

	//doc
	Mat mat_recv;
	CImage cimage_recv;

public:
	CString ip;
	CString port;
	CString name;

protected:
	afx_msg LRESULT OnUmSendimg(WPARAM wParam, LPARAM lParam);
};
