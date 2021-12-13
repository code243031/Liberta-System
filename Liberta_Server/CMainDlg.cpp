// CMainDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Liberta_server.h"
#include "afxdialogex.h"
#include "CMainDlg.h"
#include "Liberta_serverDlg.h"
#include "databaseLoading.h"

// users 클래스
Users::Users(const char* paccode, const char* nm, const char* age, const char* address, const char* callnum)
{
	name = nm;
	this->paccode = paccode;
	this->age = age;
	this->address = address;
	this->callnum = callnum;
}

Users::~Users()
{

}

CString Users::getPaccode()
{
	return paccode;
}
CString Users::getName()
{
	return name;
}

CString Users::getAge()
{
	return age;
}

CString Users::getAddress()
{
	return address;
}

CString Users::getCallNum()
{
	return callnum;
}

// CMainDlg 대화 상자
IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN, pParent)
{

}

CMainDlg::~CMainDlg()
{
	users.clear();
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILE, profile);
	DDX_Control(pDX, IDC_PAC_NAME, name);
	DDX_Control(pDX, IDC_PAC_AGE, age);
	DDX_Control(pDX, IDC_PAC_ADDRESS, address);
	DDX_Control(pDX, IDC_PAC_PHONE, phone);
	DDX_Control(pDX, IDC_LIST, pac_list);
	DDX_Control(pDX, IDC_DATA, pac_data);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST, &CMainDlg::OnLbnSelchangeList)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DATA, &CMainDlg::OnLvnItemchangedData)
	ON_COMMAND(ID_EXIT, &CMainDlg::OnExit)
END_MESSAGE_MAP()

vector<string> splitStr(const string& str) {
	vector<string> res;
	istringstream stream(str);
	for (string s; stream >> s;) {
		res.push_back(s);
	}

	return res;
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	CRect rt;
	pac_data.GetWindowRect(&rt);
	pac_data.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	pac_data.InsertColumn(0, _T("날짜"), LVCFMT_LEFT, rt.Width() * 0.1);
	pac_data.InsertColumn(1, _T("산소포화도"), LVCFMT_LEFT, rt.Width() * 0.2);
	pac_data.InsertColumn(2, _T("체온"), LVCFMT_LEFT, rt.Width() * 0.1);
	pac_data.InsertColumn(3, _T("분류"), LVCFMT_LEFT, rt.Width() * 0.1);
	pac_data.InsertColumn(4, _T("등급"), LVCFMT_LEFT, rt.Width() * 0.1);

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

	CString sql = _T("");
	sql.Format(_T("SELECT * FROM pac_info"));

	mysql_query(con, "set Names euckr");
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
	MYSQL_ROW row = mysql_fetch_row(result);
	CString res;

	if (row == nullptr) {
		AfxMessageBox(_T("아직 아무도 진료하지 않았음!"));
		mysql_close(con);
		return FALSE;
	}

	while (row != NULL)
	{
		TRACE("%s %s %s %s\n", row[0], row[1], row[2], row[3]); // 디버그용

		// user클래스 생성 성공 시 다이얼로그 연결
		// add list
		res = row[1];
		pac_list.AddString(res);

		// 클래스를 만들고 vector에 추가
		Users user(row[0], row[1], row[2], row[3], row[4]);
		users.push_back(user);

		row = mysql_fetch_row(result);
	}

	mysql_close(con);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CMainDlg 메시지 처리기
void CMainDlg::OnBnClickedOk()
{
	CLibertaserverDlg dlg;
	dlg.DoModal();
}

void CMainDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CMainDlg::OnLbnSelchangeList()
{
	CString tar;
	pac_list.GetText(pac_list.GetCurSel(), tar);

	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].getName().Compare(tar) == 0) {
			name.SetWindowTextW(tar);
			age.SetWindowTextW(users[i].getAge());
			address.SetWindowTextW(users[i].getAddress());
			phone.SetWindowTextW(users[i].getCallNum());

			// pac_data.DeleteAllItems();
			// load items..
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

			CString sql = _T("");
			sql.Format(_T("SELECT * FROM med_record where DOCID='%s' AND PACCODE='%s'"), (LPCTSTR)doc_id, (LPCTSTR)users[i].getPaccode());

			mysql_query(con, "set Names euckr");
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
			MYSQL_ROW row = mysql_fetch_row(result);

			if (row == nullptr) {
				AfxMessageBox(_T("아직 아무도 진료하지 않았음!"));
				mysql_close(con);
				return;
			}

			vector<CString> date;
			vector<CString> oxygen;
			vector<CString> temp;
			vector<CString> value;
			vector<CString> rank;

			while (row != NULL)
			{
				TRACE("%s %s %s %s\n", row[0], row[1], row[2], row[3]); // 디버그용

				// vector에 결과 대입
				date.push_back((LPCTSTR)row[2]);
				oxygen.push_back((LPCTSTR)row[3]);
				temp.push_back((LPCTSTR)row[4]);
				value.push_back(_T("미검"));
				rank.push_back(_T("미검"));

				row = mysql_fetch_row(result);
			}

			for (unsigned long i = 0; i < *result->lengths; i++) {
				pac_data.InsertItem(i, doc_nm, i % 5);

				// 추가하기
				pac_data.SetItemText(i, 0, date[i]);
				pac_data.SetItemText(i, 1, oxygen[i]);
				pac_data.SetItemText(i, 2, temp[i]);
				pac_data.SetItemText(i, 3, value[i]);
				pac_data.SetItemText(i, 4, rank[i]);
			}

			mysql_close(con);
			break;
		}
	}
}

void CMainDlg::OnLvnItemchangedData(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	// pNMLV.
}


BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// Menu Commands
void CMainDlg::OnExit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	OnBnClickedCancel();
}
