// Liberta_serverDlg.h: 헤더 파일
//
#pragma once

#include <opencv2/opencv.hpp>
#include <locale.h>

#include "CListenSocket.h"

using namespace cv;
using namespace std;

// CLibertaserverDlg 대화 상자
class CLibertaserverDlg : public CDialogEx
{
// 생성입니다.
public:
	CLibertaserverDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CListenSocket m_ListenSocket;
	CListenSocket send;
	CListenSocket recv;

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

public:
	afx_msg void OnEnChangeChat();
	afx_msg void OnEnChangeType();
	afx_msg void OnBnClickedOk();

	afx_msg void OnStnClickedDoc();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	CStatic m_video;
	CStatic m_video_pac;
	
	CEdit m_Chat;

	VideoCapture* capture;
	Mat mat_frame;
	CImage cimage_mfc;
};
