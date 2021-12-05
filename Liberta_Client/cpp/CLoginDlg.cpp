// CLoginDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Liberta_client.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include "Liberta_clientDlg.h"


// CLoginDlg 대화 상자

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDlg 메시지 처리기
void CLoginDlg::OnBnClickedOk()
{
	CString code, port, name;

	GetDlgItemText(IDC_CODE, code);
	GetDlgItemText(IDC_PORT, port);
	GetDlgItemText(IDC_CODE2, name);

	if (code.GetLength() == 0 || port.GetLength() == 0 || name.GetLength() == 0) {
		AfxMessageBox(_T("아이디와 비밀번호, 접속코드를 입력해주세요!"));
		return;
	}
	else if (name.GetLength() == 0) {
		AfxMessageBox(_T("접속코드를 입력해주세요!"));
		return;
	}

	// 로그인 성공 시 다이얼로그 연결
	this->ShowWindow(SW_HIDE);

	CLibertaclientDlg dlg;
	dlg.ip = code;
	dlg.port = port;
	dlg.name = name;

	dlg.DoModal();

	this->ShowWindow(SW_SHOW);
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
