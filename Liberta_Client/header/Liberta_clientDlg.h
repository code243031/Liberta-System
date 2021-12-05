// Liberta_clientDlg.h: 헤더 파일
//

#pragma once

#include <opencv2/opencv.hpp>

#include "CConnectSocket.h"

using namespace cv;
using namespace std;

// CLibertaclientDlg 대화 상자
class CLibertaclientDlg : public CDialogEx
{
	// 생성입니다.
public:
	CLibertaclientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CConnectSocket m_Socket;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBERTA_CLIENT_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	CStatic m_video;
	CEdit type;

	VideoCapture* capture;
	Mat mat_frame;
	CImage cimage_mfc;

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

	CEdit m_Chat;
};
