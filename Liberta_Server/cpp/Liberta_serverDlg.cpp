// Liberta_serverDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Liberta_server.h"
#include "Liberta_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CClientSocket.h"

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CLibertaserverDlg 대화 상자
CLibertaserverDlg::CLibertaserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIBERTA_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLibertaserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT, m_Chat);
	DDX_Control(pDX, IDC_DOC, m_video);
	DDX_Control(pDX, IDC_PAC, m_video_pac);
}

BEGIN_MESSAGE_MAP(CLibertaserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_CHAT, &CLibertaserverDlg::OnEnChangeChat)
	ON_EN_CHANGE(IDC_TYPE, &CLibertaserverDlg::OnEnChangeType)
	ON_BN_CLICKED(IDOK, &CLibertaserverDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_DOC, &CLibertaserverDlg::OnStnClickedDoc)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLibertaserverDlg 메시지 처리기
BOOL CLibertaserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if (m_ListenSocket.Create(8200, SOCK_STREAM)) { // 소켓생성
		if (!m_ListenSocket.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}

	if (send.Create(8201, SOCK_STREAM)) { // 소켓생성
		if (!send.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}

	if (recv.Create(8202, SOCK_STREAM)) { // 소켓생성
		if (!recv.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}

	capture = new VideoCapture(0);
	if (!capture->isOpened()) {
		MessageBox(_T("웹캠을 열 수 없습니다.\n"));
		capture->release();
	}
	else {
		capture->set(CAP_PROP_FRAME_WIDTH, 320);
		capture->set(CAP_PROP_FRAME_HEIGHT, 240);
	}

	SetTimer(1000, 30, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLibertaserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLibertaserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLibertaserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLibertaserverDlg::OnDestroy()
{
	POSITION pos;

	if (capture->isOpened()) {
		capture->release();
	}

	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	// 생성되어있는 클라이언트 소켓이 없을때까지 체크하여 소켓닫기
	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			pClient->ShutDown(); // 연결된 상대방 소켓에 연결이 종료됨을 알린다. 
			pClient->Close(); // 소켓을 닫는다

			delete pClient;
		}
	}

	m_ListenSocket.ShutDown();
	m_ListenSocket.Close();

	CDialog::OnDestroy();
}

void CLibertaserverDlg::OnEnChangeChat()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLibertaserverDlg::OnEnChangeType()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CLibertaserverDlg::OnBnClickedOk()
{
	CString str, str_chat, res;
	TCHAR* send = (TCHAR*)(LPCTSTR)str;

	GetDlgItemText(IDC_TYPE, str);
	GetDlgItemText(IDC_CHAT, str_chat);

	CListenSocket* pServerSocket = &m_ListenSocket;
	pServerSocket->SendAllMessage(send); // 다른 클라이언트들에게 메시지 전달

	res.Append(str_chat);
	res.Append(_T("[나] : "));
	res.Append(str);
	res.Append(_T("\r\n"));
	
	SetDlgItemText(IDC_TYPE, _T(""));
	SetDlgItemText(IDC_CHAT, res);
}


void CLibertaserverDlg::OnStnClickedDoc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLibertaserverDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// MAT_FRAME : 이미지
	capture->read(mat_frame);

	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	if (bpp < 32) {
		padding = 4 - (mat_frame.cols % 4);
	}

	if (padding == 4) {
		padding = 0;
	}

	int border = 0;

	if (bpp < 32) {
		border = 4 - (mat_frame.cols % 4);
	}

	Mat mat_temp;
	if (border > 0 || mat_frame.isContinuous() == false) {
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else {
		mat_temp = mat_frame;
	}

	RECT r;
	m_video.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);

	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;

	if (bpp == 8) {
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++) {
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
		}
	}

	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height) {
		SetDIBitsToDevice(cimage_mfc.GetDC(),
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else {
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	HDC dc = ::GetDC(m_video.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);

	if (send != INVALID_SOCKET) {
		send.Send(&mat_temp, BUFSIZ);
	}
	
	::ReleaseDC(m_video.m_hWnd, dc);
	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

	CDialogEx::OnTimer(nIDEvent);
}
