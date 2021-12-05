// CMainDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Liberta_server.h"
#include "afxdialogex.h"
#include "CMainDlg.h"

// CMainDlg 대화 상자
IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILE, profile);
	DDX_Control(pDX, IDC_PAC_NAME, name);
	DDX_Control(pDX, IDC_PAC_AGE, age);
	DDX_Control(pDX, IDC_PAC_ADDRESS, address);
	DDX_Control(pDX, IDC_PAC_PHONE, phone);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST, &CMainDlg::OnLbnSelchangeList)
	ON_WM_PAINT()
END_MESSAGE_MAP()

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
	setlocale(LC_ALL, "Korean");

	// profile initiated
	CString readfilename = _T("C:\\Users\\lg\\Documents\\projects\\Liberta_server\\Liberta_server\\res\\Patient\\Data\\data.txt");   //읽을 파일의 경로를 설정
	int lineCount = 0; //해당 파일의 라인 수를 저장할 변수 선언

	FILE* fp = NULL;   //파일 객체 fp 선언
	CStringA csAFullAddr = CStringA(readfilename);
	const char* cFullAddr = csAFullAddr;

	_setmode(_fileno(fp), _O_U8TEXT);
	fopen_s(&fp, cFullAddr, "rt+,ccs=UTF-8");  //파일을 열어 준다

	if (fp == NULL)     //해당 파일의 경로 또는 이름이 잘못 되어 파일 객체의 값이 NULL일 경우
	{
		AfxMessageBox(_T("ERROR : Can't file open"));
		return FALSE;
	}

	char szContent[2048] = { 0, };
	memset(szContent, NULL, 2048);
	BOOL bContinue = FALSE;

	while (fgets(szContent, 2048, fp))   //파일을 한 줄씩 읽는다.
	{
		CString strContent;
		strContent.Format(_T("%s"), szContent);
		memset(szContent, NULL, 2048);
		lineCount++;   //파일 라인 증가

		AfxMessageBox(strContent); //해당 라인 수 출력
	}

	fclose(fp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CMainDlg 메시지 처리기
void CMainDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CMainDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CMainDlg::OnLbnSelchangeList()
{


}

