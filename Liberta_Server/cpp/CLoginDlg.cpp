// CLoginDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Liberta_server.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include "Liberta_serverDlg.h"
#include "databaseLoading.h"
#include "CMainDlg.h"

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString id, pw, code;

	GetDlgItemText(IDC_ID, id);
	GetDlgItemText(IDC_PW, pw);
	GetDlgItemText(IDC_CODE, code);

	if (id.GetLength() == 0 || pw.GetLength() == 0 || code.GetLength() == 0) {
		AfxMessageBox(_T("아이디와 비밀번호, 접속코드를 입력해주세요!"));
		return;
	}
	else if (code.GetLength() == 0) {
		AfxMessageBox(_T("접속코드를 입력해주세요!"));
		return;
	}
	else {
		// authentiation
		MYSQL* con = mysql_init(NULL);

		if (con == NULL)
		{
			fprintf(stderr, "mysql_init() failed \n");
			exit(1);
		}

		if (mysql_real_connect(con, "localhost", "connectuser", "1234",
			"molou", 0, NULL, 0) == NULL)

		{
			finish_with_error(con);
		}


		CString sql = L"";
		sql.Append(_T("SELECT * FROM doc_info where "));
		sql.Append(_T("ID='"));
		sql.Append(id);
		sql.Append(_T("' AND PW='"));
		sql.Append(pw);
		sql.Append(_T("'"));

		if (mysql_query(con, (CStringA)sql))
		{
			finish_with_error(con);
		}

		// mysql_store_result함수로 result set을 가져오고
		// result set을 MYSQL_RES 구조체에 저장한다
		MYSQL_RES* result = mysql_store_result(con);
		if (result == NULL)
		{
			finish_with_error(con);
		}

		// mysql_num_fields 함수로 테이블의 Column 수를 알아낸다
		MYSQL_ROW row;
		CString res;
		int num_fields = mysql_num_fields(result);
		/*
		while (row = mysql_fetch_row(result))
		{
			res.Format(_T("%s"), row[0]);
			printf("\n");
		}

		AfxMessageBox(res);
		*/
		if (num_fields != 5) {
			AfxMessageBox(_T("아이디/비밀번호가 존재하지 않습니다!"));
			return;
		}
	}

	// 로그인 성공 시 다이얼로그 연결
	this->ShowWindow(SW_HIDE);

	CMainDlg dlg;
	dlg.DoModal();

	this->ShowWindow(SW_SHOW);
}

void CLoginDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void finish_with_error(MYSQL* con)
{
	fprintf(stderr, "%s \n", mysql_error(con));
	mysql_close(con);
	exit(1);
}

bool compare(tuple<double, double, string> Pa, tuple<double, double, string> Pb)
{
	// 체온이 같을 경우
	if (get<0>(Pa) == get<0>(Pb))
	{
		// 산소포화도가 높은 경우가 더 먼저 출력
		return get<1>(Pa) > get<1>(Pb);
	}
	else
	{
		return get<1>(Pa) < get<1>(Pb);
	}
}

bool sortdesc(const tuple<double, double, string>& P1,
	const tuple<double, double, string>& P2)
{
	return (get<0>(P1) > get<0>(P2));
}