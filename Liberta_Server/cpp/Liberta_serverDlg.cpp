// Liberta_serverDlg.cpp: 구현 파일
//
#pragma warning(disable : 4996)

#include "pch.h"
#include "framework.h"
#include "Liberta_server.h"
#include "Liberta_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLibertaserverDlg 대화 상자
CLibertaserverDlg::CLibertaserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIBERTA_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	connect = false;

}

CLibertaserverDlg::~CLibertaserverDlg()
{

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

bool CLibertaserverDlg::initSession() {
	// 윈도우 소켓 라이브러리 초기화
	int iRes = ::WSAStartup(MAKEWORD(0x02, 0x02), &wsdata);
	if (ERROR_SUCCESS != iRes)
		return false;

	// 소켓 만들기
	m_socketServer = ::socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_socketServer)
		return false;

	int nTimeOutValue = 1000;
	
	// IP와 포트를 생성한 소켓에 결합
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(8200);

	iRes = ::bind(m_socketServer, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (ERROR_SUCCESS != iRes)
		return false;

	// Listen
	iRes = ::listen(m_socketServer, SOMAXCONN);
	if (ERROR_SUCCESS != iRes)
		return false;

	// 클라이언트 Accept
	iRes = ::WSAAsyncSelect(m_socketServer, m_hWnd, 10000, FD_ACCEPT);
	if (ERROR_SUCCESS != iRes)
		return false;

	return true;
}

bool CLibertaserverDlg::initVideoSession() {
	// 윈도우 소켓 라이브러리 초기화
	int iRes = ::WSAStartup(MAKEWORD(0x02, 0x02), &wsdata_v);
	if (ERROR_SUCCESS != iRes)
		return false;

	// 소켓 만들기
	m_socketServer_v = ::socket(PF_INET, SOCK_STREAM, 0);
	if (m_socketServer_v ==INVALID_SOCKET)
		return false;

	int nTimeOutValue = 1000;

	// IP와 포트를 생성한 소켓에 결합
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(8201);

	iRes = ::bind(m_socketServer_v, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (iRes != ERROR_SUCCESS)
		return false;

	// Listen
	iRes = ::listen(m_socketServer_v, SOMAXCONN);
	if (iRes != ERROR_SUCCESS)
		return false;

	// 클라이언트 Accept
	iRes = ::WSAAsyncSelect(m_socketServer_v, m_hWnd, 10000, FD_ACCEPT);
	if (iRes != ERROR_SUCCESS)
		return false;

	AfxMessageBox(_T("됬어!"));
	return true;
}

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
	connect = initSession();
	if (connect != true) {
		AfxMessageBox(_T("네트워크 연결 실패"));
		return FALSE;
	}

	connect_v = initVideoSession();
	if (connect_v != true) {
		AfxMessageBox(_T("네트워크 연결 실패"));
		return FALSE;
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
	CDialogEx::OnSysCommand(nID, lParam);
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
	
	::ReleaseDC(m_video.m_hWnd, dc);
	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

	// 이미지 파일 전송 받은거를 화면에 입력
	if (!mat_recv.empty()) {
		int bpp = 8 * mat_recv.elemSize();
		assert((bpp == 8 || bpp == 24 || bpp == 32));

		int padding = 0;
		if (bpp < 32) {
			padding = 4 - (mat_recv.cols % 4);
		}

		if (padding == 4) {
			padding = 0;
		}

		int border = 0;

		if (bpp < 32) {
			border = 4 - (mat_recv.cols % 4);
		}

		Mat mat_temp;
		if (border > 0 || mat_recv.isContinuous() == false) {
			cv::copyMakeBorder(mat_recv, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
		}
		else {
			mat_temp = mat_recv;
		}

		RECT r;
		m_video_pac.GetClientRect(&r);
		cv::Size winSize(r.right, r.bottom);

		cimage_recv.Create(winSize.width, winSize.height, 24);

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
			SetDIBitsToDevice(cimage_recv.GetDC(),
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

			StretchDIBits(cimage_recv.GetDC(),
				destx, desty, destw, desth,
				imgx, imgy, imgWidth, imgHeight,
				mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		HDC dc = ::GetDC(m_video_pac.m_hWnd);
		cimage_recv.BitBlt(dc, 0, 0);

		::ReleaseDC(m_video_pac.m_hWnd, dc);
		cimage_recv.ReleaseDC();
		cimage_recv.Destroy();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CLibertaserverDlg::OnBnClickedOk()
{
	CString str, str_chat, res;

	GetDlgItemText(IDC_TYPE, str);
	GetDlgItemText(IDC_CHAT, str_chat);

	res.Append(str_chat);
	res.Append(_T("[나] : "));
	res.Append(str);
	res.Append(_T("\r\n"));

	CStringA conv = (CStringA)str;
	const char* cBuff = conv.GetBuffer();

	int iSend = ::send(m_sockClient, cBuff, sizeof(cBuff), 0);
	if (iSend == SOCKET_ERROR)
	{
		// Send error
	}

	SetDlgItemText(IDC_TYPE, _T(""));
	SetDlgItemText(IDC_CHAT, res);
}


void CLibertaserverDlg::OnDestroy()
{
	POSITION pos;

	if (capture->isOpened()) {
		capture->release();
	}

	// 윈도우 소켓 사용 종료
	::closesocket(m_sockClient);
	::closesocket(m_socketServer);
	WSACleanup();

	CDialog::OnDestroy();
}

LRESULT CLibertaserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (10000 == message) {
		SOCKET hSocket = (SOCKET)wParam;

		switch (lParam)
		{
		case FD_ACCEPT:	// 클라이언트 접속
		{
			int iLen = sizeof(accept_addr);
			SOCKET hSocket = ::accept(m_socketServer, &accept_addr, &iLen);
			if (INVALID_SOCKET != hSocket) {
				// 클라이언트 소켓의 이벤트도 받을 수 있게 접속 시 등록한다.
				int iRes = ::WSAAsyncSelect(hSocket, m_hWnd, 10000, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
				m_sockClient = hSocket;
			}

			GetDlgItemText(IDC_CHAT, str_chat);

			str_chat.Append(_T("[SYSTEM] : 상대방이 접속했습니다."));
			str_chat.Append(_T("\r\n"));

			SetDlgItemText(IDC_CHAT, str_chat);

			break;
		}
		case FD_READ: // 데이터 수신
		{
			int addrlen = sizeof(accept_addr);
			int addrlen_v = sizeof(accept_addr_v);

			if (getsockname(hSocket, &accept_addr, &addrlen) == 0 && accept_addr.sa_family == AF_INET && addrlen == sizeof(accept_addr)) {
				char cBuff[BUFSIZE];
				memset(&cBuff, 0, sizeof(cBuff));
				int iLen = ::recv(hSocket, cBuff, sizeof(cBuff), 0);

				if (0 < iLen) { // 정상 수신되지 않을 시

				}

				GetDlgItemText(IDC_CHAT, str_chat);

				str_chat.Append(_T("[상대방] : "));
				str_chat.Append((CString)cBuff);
				str_chat.Append(_T("\r\n"));

				SetDlgItemText(IDC_CHAT, str_chat);
			}
			else if (getsockname(hSocket, &accept_addr_v, &addrlen_v) == 0 && accept_addr_v.sa_family == AF_INET && addrlen_v == sizeof(accept_addr_v)) {
				FILE* fp = NULL;
				char buf[BUFSIZE];
				char buf2[BUFSIZE];

				// 이미지 파일 수신
				ZeroMemory(buf, NULL);
				fopen_s(&fp, "recv.jpg", "wb");

				// 버퍼 채우기
				int iLen = ::recv(hSocket, buf, sizeof(buf), 0);
				fwrite(buf, BUFSIZE, 1, fp);

				// 버퍼를 두번 받으면 딜레이가 사라진다??
				iLen = ::recv(hSocket, buf2, sizeof(buf2), 0);
				fwrite(buf2, BUFSIZE, 1, fp);
				fclose(fp);

				// 이미지 저장
				mat_recv = imread("recv.jpg");

				GetDlgItemText(IDC_CHAT, str_chat);

				str_chat.Append(_T("[SYSTEM] : 상대방이 이미지를 전송함."));
				str_chat.Append(_T("\r\n"));

				SetDlgItemText(IDC_CHAT, str_chat);
			}

			break;
		}
		// 소켓 종료
		case FD_CLOSE:
		{
			int iRes = ::closesocket(hSocket);
			OnDestroy();

			AfxMessageBox(_T("상대방이 연결을 종료했습니다!"));

			break;
		}
		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

BOOL CLibertaserverDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			OnBnClickedOk();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
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

void CLibertaserverDlg::OnStnClickedDoc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
