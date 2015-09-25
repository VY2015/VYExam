#include "stdafx.h"
#include "LoginWnd.h"
#include "DownLoadProgressBar.h"


CDownLoadProgressBar::CDownLoadProgressBar(void)
	: m_bIdChg(false)
{

}

LPCTSTR CDownLoadProgressBar::GetWindowClassName() const
{ 
	return _T("UILoginWnd"); 
}

UINT CDownLoadProgressBar::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG; 
}

void CDownLoadProgressBar::OnFinalMessage(HWND /*hWnd*/)
{ 
	m_pm.RemovePreMessageFilter(this);
	delete this; 
};

//void IDCard(DWORD dlg)
//{
//	CDownLoadProgressBar *p = (CDownLoadProgressBar*)dlg;
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

void CDownLoadProgressBar::Init()
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

	CComboUI *pComboUI1 = static_cast<CComboUI*>(m_pm.FindControl(_T("field")));
	CComboUI *pComboUI2 = static_cast<CComboUI*>(m_pm.FindControl(_T("proj")));
	
	pComboUI1->SetItemFont(0);
	pComboUI2->SetItemFont(0);
//	g_oVYDevice.SetIDCardCallBack(IDCard, (DWORD)this);
	g_oVYDevice.Open(CVYDevice::IDCRAD);

	//zjd
	GetIniFile();
}

void CDownLoadProgressBar::OnPrepare()
{
	//@feng进度条设置
	CProgressUI *pProcessUI1 = static_cast<CProgressUI*>(m_pm.FindControl(_T("DownLoadProgress")));
	pProcessUI1->SetValue(rand()%100);
}

eSocketConnectStatus  CDownLoadProgressBar::Connectd(CStringA Buffer)
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
void CDownLoadProgressBar::ConnectResultProc(eSocketConnectStatus eConnectStatus)
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

bool CDownLoadProgressBar::GetIniFile()
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

void CDownLoadProgressBar::onLoginBtnClicked()
{
	/*CTextUI  *pTextUI1 = static_cast<CTextUI*>(m_pm.FindControl(_T("test1")));
	CTextUI  *pTextUI2 = static_cast<CTextUI*>(m_pm.FindControl(_T("test2")));
	CTextUI  *pTextUI3 = static_cast<CTextUI*>(m_pm.FindControl(_T("test3")));
	CTextUI  *pTextUI4 = static_cast<CTextUI*>(m_pm.FindControl(_T("test4")));
	CTextUI  *pTextUI5 = static_cast<CTextUI*>(m_pm.FindControl(_T("test5")));*/
}
void CDownLoadProgressBar::Notify(TNotifyUI& msg)
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
			return;
		}
		if( msg.pSender->GetName() == _T("TEST") ) 
		{ 
			OnPrepare();
		}
	}
	else if (msg.sType == _T("link"))
	{
		if( msg.pSender->GetName() == _T("register") )
		{ 
			//@feng注册窗口
			//POINT pt = {/*msg.ptMouse.x*/0, /*msg.ptMouse.y*/0};
			//CDuiRegistDialog *pRegist = new CDuiRegistDialog(_T("regist.xml"));
			//pRegist->Init(*this, pt);
			//pRegist->ShowWindow(TRUE);
			return;
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

LRESULT CDownLoadProgressBar::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("DownLoadProgressBar.xml"), 0,NULL , &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();

	return 0;
}

LRESULT CDownLoadProgressBar::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDownLoadProgressBar::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CDownLoadProgressBar::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) )
		bHandled = FALSE;

	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CDownLoadProgressBar::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CDownLoadProgressBar::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CDownLoadProgressBar::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDownLoadProgressBar::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDownLoadProgressBar::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDownLoadProgressBar::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CDownLoadProgressBar::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CDownLoadProgressBar::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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