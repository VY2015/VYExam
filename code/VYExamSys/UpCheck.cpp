#include "stdafx.h"
#include "UpCheck.h"

CUpCheckWnd::CUpCheckWnd(void)
	: m_bIdChg(false)
{

}

//CUpCheckWnd::CUpCheckWnd( CLoginWnd *pParentWnd )
//{
//	m_pParentWnd = pParentWnd;
//}

LPCTSTR CUpCheckWnd::GetWindowClassName() const
{ 
	return _T("UIChildLLogin"); 
}

UINT CUpCheckWnd::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG; 
}

void CUpCheckWnd::OnFinalMessage(HWND /*hWnd*/)
{ 
	m_pm.RemovePreMessageFilter(this);
	delete this; 
};

//void IDCard(DWORD dlg)
//{
//	CUpCheckWnd *p = (CUpCheckWnd*)dlg;
//
//	//g_oVYDevice.SaveImage(CVYDevice::IDCRAD,"D:\\id.jpg");
//	CString idNo = g_oVYDevice.GetIdCardData(8);
//
//	CEditUI *pEditUI1 = static_cast<CEditUI*>(p->m_pm.FindControl(_T("id1")));
//	CEditUI *pEditUI2 = static_cast<CEditUI*>(p->m_pm.FindControl(_T("id2")));
//
//	CString text1 = pEditUI1->GetText();
//	CString text2 = pEditUI2->GetText();
//
//	if(text1 == idNo || text2 == idNo)
//		return;
//
//	if(!p->m_bIdChg)
//	{
//		pEditUI1->SetText(idNo);
//		pEditUI1->SetFont(0);
//	}
//	else
//	{
//		pEditUI2->SetText(idNo);
//		pEditUI2->SetFont(0);
//	}
//
//	p->m_bIdChg = !p->m_bIdChg;
//}

void CUpCheckWnd::Init()
{
// 	TFontInfo* pFontInfo = m_pm.GetDefaultFontInfo();
// 	if( pFontInfo->iSize < 18 ) {
// 		TFontInfo* pFontInfo = m_pm.GetFontInfo(0);
// 		if( pFontInfo )m_pm.SetDefaultFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, 
// 			pFontInfo->bUnderline, pFontInfo->bItalic);
// 	}
// 	else {
// 		TFontInfo* pFontInfo = m_pm.GetFontInfo(1);
// 		if( pFontInfo )m_pm.SetDefaultFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, 
// 			pFontInfo->bUnderline, pFontInfo->bItalic);
// 	}
// 	m_pm.GetRoot()->NeedUpdate();

	//CComboUI *pComboUI1 = static_cast<CComboUI*>(m_pm.FindControl(_T("field")));
	//CComboUI *pComboUI2 = static_cast<CComboUI*>(m_pm.FindControl(_T("proj")));
	//
	//pComboUI1->SetItemFont(0);
	//pComboUI2->SetItemFont(0);
	//g_oVYDevice.SetIDCardCallBack(IDCard, (DWORD)this);
	//g_oVYDevice.Open(CVYDevice::IDCRAD);

	////zjd
	//GetIniFile();
}

void CUpCheckWnd::OnPrepare()
{
//	CComboUI *pComboUI1 = static_cast<CComboUI*>(m_pm.FindControl(_T("field")));

// 	CListLabelElementUI *ui= new CListLabelElementUI;
// 	ui->SetName(L"aaa");
// 	ui->SetText(L"aaaaaa");
// 	pComboUI1->Add(ui);
}

eSocketConnectStatus  CUpCheckWnd::Connectd(CStringA Buffer)
{
	WSADATA  Ws;
	SOCKET CientSocket;
	struct sockaddr_in ServerAddr;
	int Ret = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;

	char SendBuffer[MAX_PATH] = "";
	sprintf_s(SendBuffer, Buffer, sizeof(Buffer));

	//Init Windows Socket
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		WSACleanup();
		return SOCKET_START_ERR;
	}

	//Create Socket
	CientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (CientSocket == INVALID_SOCKET)
	{
		closesocket(CientSocket);
		WSACleanup();
		return SOCKET_INIT_ERR;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(m_chServerIP);
	ServerAddr.sin_port = htons(m_nPort);
	memset(ServerAddr.sin_zero, 0x00, 8);

	//超时时间
	int iTimeOut = 20 * 1000;
	setsockopt(CientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeOut, sizeof(iTimeOut));
	setsockopt(CientSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeOut, sizeof(iTimeOut));

	Ret = connect(CientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if (Ret == SOCKET_ERROR)
	{
		closesocket(CientSocket);
		WSACleanup();
		return SOCKET_CONNECT_ERR;
	}

	Ret = send(CientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
	if (Ret == SOCKET_ERROR)
	{
		closesocket(CientSocket);
		WSACleanup();
		return SOCKET_SEND_ERR;
	}

	char RecvBuffer[MAX_PATH] = { 0 };

	Ret = recv(CientSocket, RecvBuffer, MAX_PATH, 0);
	if (Ret == 0 || Ret == SOCKET_ERROR)
	{
		closesocket(CientSocket);
		WSACleanup();
		return SOCKET_RECV_ERR;
	}

	std::string strRecvBuf(RecvBuffer);
	std::string::size_type nBeginPos = strRecvBuf.find("[begin]");
	std::string::size_type nEndPos = strRecvBuf.find("[end]");
	strRecvBuf = strRecvBuf.assign(strRecvBuf, nBeginPos + 7, nEndPos - nBeginPos - 7);
	if (nBeginPos == std::string::npos)
		strRecvBuf = std::string("-1");

	eSocketConnectStatus eConnectStatus = SOCKET_RECV_DATA_ERR;

	eConnectStatus = (eSocketConnectStatus)atoi(strRecvBuf.c_str());

	closesocket(CientSocket);
	WSACleanup();

	return eConnectStatus;
}
void CUpCheckWnd::ConnectResultProc(eSocketConnectStatus eConnectStatus)
{
	switch (eConnectStatus)
	{
	case SOCKET_RECV_SUCCESS:
		MessageBoxA(NULL, "登录成功！", "提示", MB_OK);
		break;

	case SOCKET_START_ERR:
	case SOCKET_INIT_ERR:
	case SOCKET_CONNECT_ERR:
	case SOCKET_SEND_ERR:
	case SOCKET_RECV_ERR:
		MessageBoxA(NULL, "发送失败，请重新尝试！", "提示", MB_OK);
		break;

	case SOCKET_RECV_DATA_ERR:
		MessageBoxA(NULL, "返回结果失败，请检查输入参数！", "提示", MB_OK);
		break;

	case SOCKET_RECV_DATA_EMPTY:
		MessageBoxA(NULL, "返回结果失败，必填参数为空！", "提示", MB_OK);
		break;

	case SOCKET_RECV_DATA_ABNORMAL:
	default:
		MessageBoxA(NULL, "系统异常！", "提示", MB_OK);
		break;
	}
}

bool CUpCheckWnd::GetIniFile()
{
	//GetPrivateProfileStringA("ExamInfo", "ServerIP", "", m_chServerIP, 32, ".\\ExamProfile.ini");
	m_nPort = GetPrivateProfileIntA("ExamInfo", "Port", 80, ".\\ExamProfile.ini");
	//GetPrivateProfileStringA("ExamInfo", "ExamRoom", "", m_stExamInfo._chExamRoom, 32, ".\\ExamProfile.ini");
	//GetPrivateProfileStringA("ExamInfo", "ExamType", "", m_stExamInfo._chExamType, 32, ".\\ExamProfile.ini");
	//char buf[255];
	//GetPrivateProfileStringA("FTP", "LocalPath", "", buf, sizeof(buf), ".\\ExamProfile.ini");
	//m_stFtpConfig._stLocalPath._strLocalPath = std::string(buf);
	//GetPrivateProfileStringA("FTP", "LocalBakPath", "", buf, sizeof(buf), ".\\ExamProfile.ini");
	//m_stFtpConfig._stLocalPath._strLocalBakPath = std::string(buf);
	return true;
}

void CUpCheckWnd::onLoginBtnClicked()
{
	CString strBuf;
	CComboUI *pComboUI1 = static_cast<CComboUI*>(m_pm.FindControl(_T("field")));
	CComboUI *pComboUI2 = static_cast<CComboUI*>(m_pm.FindControl(_T("proj")));
	CEditUI  *pTextUI1  = static_cast<CEditUI*>(m_pm.FindControl(_T("id1")));
	CEditUI  *pTextUI2  = static_cast<CEditUI*>(m_pm.FindControl(_T("id2")));
	strBuf = pComboUI1->GetText();	
	memset(g_login.examRoom,'\0',sizeof(g_login.examRoom));
	strcpy_s(g_login.examRoom,sizeof(g_login.examRoom),CStringA(strBuf));
	//strcpy_s(m_examRoom.Exam_id, sizeof(m_examRoom.Exam_id), CStringA(strBuf));
	strBuf = pComboUI2->GetText();
	memset(g_login.examItem,'\0',sizeof(g_login.examItem));
	strcpy_s(g_login.examItem,sizeof(g_login.examItem),CStringA(strBuf));
	//strcpy_s(m_examRoom, sizeof(m_examRoom.Exam_id), CStringA(strBuf));

	/*
	strcpy_s(m_stExamInfo._chExamType, sizeof(m_stExamInfo._chExamType), CStringA(strBuf));
	strBuf = pTextUI1->GetText();
	strcpy_s(m_stExamInfo._chExaminer, sizeof(m_stExamInfo._chExaminer), CStringA(strBuf));
	strBuf = pTextUI2->GetText();
	strcpy_s(m_stExamInfo._chdeputy, sizeof(m_stExamInfo._chdeputy), CStringA(strBuf));
	if (0 == strcmp(m_stExamInfo._chExaminer, "") || 0 == strcmp(m_stExamInfo._chdeputy, "")
		|| 0 == strcmp(m_stExamInfo._chExaminer, "请将主考身份证放置在识别仪上")
		|| 0 == strcmp(m_stExamInfo._chdeputy, "请将副考身份证放置在识别仪上"))
	{
		MessageBoxA(NULL, "请输入证件号码！", "提示", MB_OK);
		return;
	}
	//初始化考试时间,应该取服务器时间
	CTime t;
	t = CTime::GetCurrentTime();
	CString date(t.Format("%Y%m%d"));
	CStringA strDate(date);
	strcpy_s(m_stExamInfo._chExamTime, sizeof(m_stExamInfo._chExamTime), strDate);

	CString buf = CString("GET /examFront/clientLogin?ExamCode=") + CString(m_stExamInfo._chExamRoom) + CString("&ItemCode=") + CString(m_stExamInfo._chExamType)
		+ CString("&ExaminerId1=") + CString(m_stExamInfo._chExaminer) + CString("&ExaminerId2=") + CString(m_stExamInfo._chdeputy) + CString(" HTTP/1.1\r\nHost:")
		+ CString(m_chServerIP) + CString("\r\nConnection: Close\r\n\r\n");
	//MessageBoxA(NULL, m_chServerIP, "提示", MB_OK);
	*/
	CString buf;
	eSocketConnectStatus eConnectStatus = Connectd(CStringA(buf));
	ConnectResultProc(eConnectStatus);
	if (SOCKET_RECV_SUCCESS != eConnectStatus)
	{
		if (IDYES != MessageBoxA(NULL, "无法执行认证登录！请确认是否使用本地管理模式登录？", "提示", MB_YESNO))
		{
			return;
		}
	}
	else
	{

	}

}
void CUpCheckWnd::Notify(TNotifyUI& msg)
{
	TRACE(L"%s\n\n",msg.sType);

	if( msg.sType == _T("windowinit") )
		OnPrepare();
	else if( msg.sType == _T("click") )
	{
		if( msg.pSender->GetName() == _T("menubtn") )
		{ 
			AfxMessageBox(L"You Clicked Menu!");
			return;
		}
		if( msg.pSender->GetName() == _T("minbtn") ) 
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
		if( msg.pSender->GetName() == _T("closebtn") )
		{ 
			PostMessage(WM_CLOSE);
			return;
		}
		if( msg.pSender->GetName() == _T("loginbtn") ) 
		{ 
			//zjd
			onLoginBtnClicked();
			Close();
			//m_pParentWnd->PostMessage(WM_CLOSE);
			//CLoginWnd::Close();
			return;
		}
	}
	else if (msg.sType == _T("link"))
	{
		if( msg.pSender->GetName() == _T("register") )
		{ 
			//@feng打开注册子窗口
			/*CChildRegistWnd* pChildRegistFrame = new CChildRegistWnd(m_pParentWnd);
			if(pChildRegistFrame == NULL) return;
			pChildRegistFrame->Create(m_pParentWnd->GetHWND(), NULL, UI_WNDSTYLE_CHILD, 0, 100, 150, 0, 0, NULL);
			pChildRegistFrame->ShowWindow(true, false);
			Close();*/
			//UINT nResponse = pLoginFrame->ShowModal();
			//POINT pt = {/*msg.ptMouse.x*/0, /*msg.ptMouse.y*/0};
			//CDuiRegistDialog *pRegist = new CDuiRegistDialog(_T("regist.xml"));
			//pRegist->Init(*this, pt);
			//pRegist->ShowWindow(TRUE);
			//return;
		}
	}
	else if( msg.sType == _T("itemselect") )
	{
		if( msg.pSender->GetName() == _T("field") ) {
			CDuiString text = msg.pSender->GetText();
		}
		else if( msg.pSender->GetName() == _T("proj") ) {
			CDuiString text = msg.pSender->GetText();
		}
	}
}

LRESULT CUpCheckWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("UpCheck.xml"), 0,NULL , &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();

	return 0;
}

LRESULT CUpCheckWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUpCheckWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CUpCheckWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) )
		bHandled = FALSE;

	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CUpCheckWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUpCheckWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUpCheckWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; 
	pt.x = GET_X_LPARAM(lParam); 
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	CContainerUI *pLoginContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("loginbox")));
	CContainerUI *pSystemContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("systembtn")));
	if (pLoginContainer)
	{
		CRect rt1 = pLoginContainer->GetPos();
		CRect rt2 = pSystemContainer->GetPos();
		if(!rt1.PtInRect(pt)&&!rt2.PtInRect(pt))
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CUpCheckWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;

	return 0;
}

LRESULT CUpCheckWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CUpCheckWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE )
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed )
	{
	}

	return lRes;
}

LRESULT CUpCheckWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{ 
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:		   lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:	       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )
		return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUpCheckWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN )
	{
		if( wParam == VK_RETURN )
		{
			return true;
		}
		else if( wParam == VK_ESCAPE )
		{
			PostQuitMessage(0);
			return true;
		}
	}
	return false;
}