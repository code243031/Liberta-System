// CMainDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Liberta_server.h"
#include "afxdialogex.h"
#include "CMainDlg.h"
#include "Liberta_serverDlg.h"

// users 클래스
Users::Users(const char* nm, const char* o2, const char* temp)
{
	name = nm;
	hpo2 = o2;
	tempature = temp;
}

Users::~Users()
{
}

CString Users::getName()
{
	return name;
}

CString Users::getHpo2()
{
	return hpo2;
}

CString Users::getTemp()
{
	return tempature;
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

	// profile initiated
	ifstream fin;
	fin.open(_T("C:\\Users\\lg\\Documents\\projects\\Liberta_server\\Liberta_server\\res\\Patient\\Data\\data.txt"));

	string c;
	vector<string> conv;
	CString res;
	int cnt = 0;
	while (fin.peek() != EOF)
	{
		getline(fin, c);
		if (fin.fail())
		{
			break;
		}

		conv = splitStr(c);

		// add list
		res = conv[2].c_str();
		pac_list.AddString(res);

		// make class of users
		Users user(conv[2].c_str(), conv[0].c_str(), conv[1].c_str());
		users.push_back(user);
	}

	fin.close();

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
		if (users[i].getName().Compare(tar) != 0) {
			name.SetWindowTextW(tar);
			pac_data.DeleteAllItems();

			// load items..

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
