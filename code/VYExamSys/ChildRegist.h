#pragma once

#include "..\\GlobalObject.h"

class CChildRegistWnd :
	public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:
	CChildRegistWnd(void);
	CChildRegistWnd(CLoginWnd* pParentWnd);
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);

	void Init();
	void OnPrepare();

	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void onLoginBtnClicked();					//点击登录的一系列操作
	bool GetIniFile();							//从配置文件中ExamProfile.ini读取serverIp和port,调试在工程目录下，运行在exe目录下
	eSocketConnectStatus  Connectd(CStringA Buffer);				//socket短链接web前段
	void ConnectResultProc(eSocketConnectStatus eConnectStatus);	//socket错误

public:
	CPaintManagerUI m_pm;
	//zjd
	EXAM_ROOM             m_examRoom;//考场信息
	char			m_chServerIP[32];
	int						 m_nPort;

public:
	CLoginWnd* m_pRParentWnd;
	bool m_bIdChg;
};

