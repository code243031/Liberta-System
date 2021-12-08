// Liberta_clientDlg.h: 헤더 파일
//
#pragma once
#pragma warning(disable : 4996)

#include <opencv2/opencv.hpp>
#include <thread>

#define BUFSIZE 44000

using namespace cv;
using namespace std;

// CLibertaclientDlg 대화 상자
class CLibertaclientDlg : public CDialogEx
{
public:
	CString ip;
	CString port;
	CString name;

	// 생성입니다.
public:
	CLibertaclientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBERTA_CLIENT_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	CStatic m_video;
	CEdit type;
	CEdit m_chat;

	CString str_chat;

	// PAC(클라이언트 측 화면) 용 캠 이미지
	VideoCapture* capture;
	Mat mat_frame;
	CImage cimage_mfc;


	// DOC(서버 측 화면) 용 캠 이미지 - 전송받은 장면을 활용 예정
	// Mat mat_recv;
	// CImage cimage_recv;

	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// session create
	bool initSession();
	bool initVideoSession();

protected:
	// chat socket
	WSADATA wsdata;
	SOCKET m_socketClient;
	sockaddr accept_addr;
	bool connect;

	// video socket
	WSADATA wsdata_v;
	SOCKET m_socketClient_v;
	sockaddr accept_addr_v;
	bool connect_v;
public:
	afx_msg void OnBnClickedCancel();
};
