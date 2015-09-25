#include "stdafx.h"
//#include "LoginWnd.h"
#include "DuiRegistDialog.h"
//
//
//CDuiRegistDialog::CDuiRegistDialog(LPCTSTR pszXMLPath)
//	: m_bIdChg(false),m_strXMLPath(pszXMLPath)
//{}
//void CDuiRegistDialog::OnFinalMessage(HWND /*hWnd*/)
//{ 
//	m_pm.RemovePreMessageFilter(this);
//	delete this; 
//};
//UINT CDuiRegistDialog::GetClassStyle() const
//{
//	return UI_CLASSSTYLE_DIALOG; 
//}
//
//LPCTSTR CDuiRegistDialog::GetWindowClassName() const
//{ 
//	return _T("UILoginWnd"); 
//}
//
//void CDuiRegistDialog::Init(HWND hWndParent, POINT ptPos)
//{
//	Create(hWndParent, _T("MenuWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
//	::ClientToScreen(hWndParent, &ptPos);
//	::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
//}
//
//void CDuiRegistDialog::Notify(TNotifyUI& msg)
//{
//	TRACE(L"%s\n\n",msg.sType);
//
//	/*if( msg.sType == _T("windowinit") )
//	{
//	}*/
//    if( msg.sType == _T("click") )
//	{
//		if( msg.pSender->GetName() == _T("menubtn") )
//		{ 
//			AfxMessageBox(L"You Clicked Menu!");
//			return;
//		}
//		if( msg.pSender->GetName() == _T("minbtn") ) 
//		{
//			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
//			return;
//		}
//		if( msg.pSender->GetName() == _T("closebtn") )
//		{ 
//			PostMessage(WM_CLOSE);
//			return;
//		}
//		if( msg.pSender->GetName() == _T("loginbtn11") ) 
//		{ 
//			//zjd
//			AfxMessageBox(_T("dsdf"));
//			return;
//		}
//	}
//	else if (msg.sType == _T("link"))
//	{
//		if( msg.pSender->GetName() == _T("register") )
//		{ 
//			//@feng注册窗口
//			POINT pt = {/*msg.ptMouse.x*/0, /*msg.ptMouse.y*/0};
//			CDuiRegistDialog *pRegist = new CDuiRegistDialog(_T("regist.xml"));
//			pRegist->Init(*this, pt);
//			pRegist->ShowWindow(TRUE);
//			return;
//		}
//	}
//	else if( msg.sType == _T("itemselect") )
//	{
//		if( msg.pSender->GetName() == _T("field") ) {
//			CDuiString text = msg.pSender->GetText();
//		}
//		else if( msg.pSender->GetName() == _T("proj") ) {
//			CDuiString text = msg.pSender->GetText();
//		}
//	}
//}
//
//LRESULT CDuiRegistDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	bHandled = FALSE;
//	return 0;
//}
//
//LRESULT CDuiRegistDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	//::PostQuitMessage(0L);
//
//	bHandled = FALSE;
//	return 0;
//}
//
//LRESULT CDuiRegistDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	if( ::IsIconic(*this) )
//		bHandled = FALSE;
//
//	return (wParam == 0) ? TRUE : FALSE;
//}
//
//LRESULT CDuiRegistDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	return 0;
//}
//
//LRESULT CDuiRegistDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	return 0;
//}
//
//LRESULT CDuiRegistDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	POINT pt; 
//	pt.x = GET_X_LPARAM(lParam); 
//	pt.y = GET_Y_LPARAM(lParam);
//	::ScreenToClient(*this, &pt);
//
//	RECT rcClient;
//	::GetClientRect(*this, &rcClient);
//
//	CContainerUI *pLoginContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("loginbox")));
//	CContainerUI *pSystemContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("systembtn")));
//	if (pLoginContainer)
//	{
//		CRect rt1 = pLoginContainer->GetPos();
//		CRect rt2 = pSystemContainer->GetPos();
//		if(!rt1.PtInRect(pt)&&!rt2.PtInRect(pt))
//			return HTCAPTION;
//	}
//
//	return HTCLIENT;
//}
//
//LRESULT CDuiRegistDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	SIZE szRoundCorner = m_pm.GetRoundCorner();
//	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
//		CDuiRect rcWnd;
//		::GetWindowRect(*this, &rcWnd);
//		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
//		rcWnd.right++; rcWnd.bottom++;
//		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
//		::SetWindowRgn(*this, hRgn, TRUE);
//		::DeleteObject(hRgn);
//	}
//
//	bHandled = FALSE;
//
//	return 0;
//}
//
//
//LRESULT CDuiRegistDialog::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	MONITORINFO oMonitor = {};
//	oMonitor.cbSize = sizeof(oMonitor);
//	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
//	CDuiRect rcWork = oMonitor.rcWork;
//	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
//
//	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
//	lpMMI->ptMaxPosition.x	= rcWork.left;
//	lpMMI->ptMaxPosition.y	= rcWork.top;
//	lpMMI->ptMaxSize.x		= rcWork.right;
//	lpMMI->ptMaxSize.y		= rcWork.bottom;
//
//	bHandled = FALSE;
//	return 0;
//}
//
//LRESULT CDuiRegistDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
//	if( wParam == SC_CLOSE )
//	{
//		::PostQuitMessage(0L);
//		bHandled = TRUE;
//		return 0;
//	}
//	BOOL bZoomed = ::IsZoomed(*this);
//	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
//	if( ::IsZoomed(*this) != bZoomed )
//	{
//	}
//
//	return lRes;
//}
//
//LRESULT CDuiRegistDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT lRes = 0;
//	BOOL bHandled = TRUE;
//	switch( uMsg ) 
//	{ 
//
//	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
//	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
//	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
//	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
//	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
//	case WM_CLOSE:		   lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
//	case WM_DESTROY:	       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
//	default:
//		bHandled = FALSE;
//	}
//	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )
//		return lRes;
//	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
//}
//
//LRESULT CDuiRegistDialog::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
//{
//	if( uMsg == WM_KEYDOWN )
//	{
//		if( wParam == VK_RETURN )
//		{
//			return true;
//		}
//		else if( wParam == VK_ESCAPE )
//		{
//			PostQuitMessage(0);
//			return true;
//		}
//	}
//	return false;
//}