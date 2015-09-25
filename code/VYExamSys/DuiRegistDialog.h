#pragma once
// �˴���Ҫ����duilib��ͷ�ļ�#include <UIlib.h>��һ�Σ���ϸ�������ǰ��̳�

class CDuiRegistDialog : public WindowImplBase
{
protected:
	virtual ~CDuiRegistDialog(){};        // ˽�л����������������˶���ֻ��ͨ��new�����ɣ�������ֱ�Ӷ���������ͱ�֤��delete this�������
	CDuiString  m_strXMLPath;
public:
	explicit CDuiRegistDialog(LPCTSTR pszXMLPath): m_strXMLPath(pszXMLPath){}
	virtual LPCTSTR    GetWindowClassName()const{ return _T("CDuiMenu "); }
	virtual CDuiString GetSkinFolder()          { return _T("");            }
	virtual CDuiString GetSkinFile()            { return m_strXMLPath;      }
	virtual void       OnFinalMessage(HWND hWnd){ delete this;              }

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		Close();
		bHandled = FALSE;
		return 0;
	}

	void Init(HWND hWndParent, POINT ptPos)
	{
		Create(hWndParent, _T("MenuWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		::ClientToScreen(hWndParent, &ptPos);
		::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	virtual void Notify(TNotifyUI& msg)
	{
		TRACE(L"%s\n\n",msg.sType);
			
	    if( msg.sType == _T("click") )
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
			if(msg.pSender->GetName() == _T("loginbtn11") )
			{
				AfxMessageBox(_T("SADF"));
				return;
			}
		}

	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		LRESULT lRes = 0;
		BOOL    bHandled = TRUE;

		switch( uMsg )
		{
		case WM_KILLFOCUS:    
			lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
			break; 
		default:
			bHandled = FALSE;
		}

		if(bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) 
		{
			return lRes;
		}
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
};



