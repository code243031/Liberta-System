#pragma once
#include "afxdialogex.h"

#include <string.h>

#include <opencv2/opencv.hpp>
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include <vector>

using namespace cv;
using namespace std;
// CMainDlg 대화 상자

class Users {
public:
	Users(const char* nm, const char* o2, const char* temp);
	~Users();

private:
	CString name;
	CString hpo2;
	CString tempature;

public:
	CString getName();
	CString getHpo2();
	CString getTemp();
};

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CString doc_nm;
	CString doc_code;

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnPaint();
	afx_msg void OnLvnItemchangedData(NMHDR* pNMHDR, LRESULT* pResult);

	CStatic profile;
	CStatic name;
	CStatic age;
	CStatic address;
	CStatic phone;

	CListBox pac_list;

	vector<Users> users;
	CListCtrl pac_data;
};
