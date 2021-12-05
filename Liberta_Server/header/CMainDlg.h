#pragma once
#include "afxdialogex.h"

#include <opencv2/opencv.hpp>
#include <fstream>
#include <fcntl.h>
#include <io.h>

using namespace cv;
using namespace std;
// CMainDlg 대화 상자

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMainDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnPaint();

	CStatic profile;
	CStatic name;
	CStatic age;
	CStatic address;
	CStatic phone;

};
