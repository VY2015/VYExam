#pragma once
#include "resource.h"
#include "LoginWnd.h"
#include <vector>
#include "..\\GlobalObject.h"
#include "UIDataDefine.h"
#include <sstream>
#include <direct.h>
#include "DialogPassed.h"
#include "IDDialogNotPassed.h"
#include "..\\EncryptFile\\encryptyFile.h"
#include "vyexamfiledef.h"

#include "DlgChoPerson.h"
//#include "algo.h"
class DialogPassed;
class IDDialogNotPassed;

class CMainFrame :
	public CWindowWnd, public INotifyUI, public IMessageFilterUI, public IListCallbackUI
{
public:
	CMainFrame(void);
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
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	BOOL InitVideo();
	LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static DWORD WINAPI Search(LPVOID lpParameter);
	BOOL InitList();
	LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
	
	void doClearBuffer();			//清空磁盘图片，路径及花名册等缓存
	void clearRoster();
	void clearEntrance();
	inline void setLoginWnd(CLoginWnd * login){ m_pLoginWnd = login;}
	void showIDCardUI();			//二代身份证界面的显示
	void capCurrentPicCompare();	//拍照并比较
	void doIDPassed();				//二代证比较通过
	void doClearUIandBuffer();		//二代证清空缓存和界面

	void initID2Check();					//切换非二代证验证界面即初始化
	void showID2UI(EXAM_ROSTER eRoster);	//非二代证显示花名册和第几次入场(点击选择考生)
	void doClearID2UIandBuffer();			//非二代证清空界面和缓存

//	int					m_ListNum;	//获取列表行号 
	Report_List_Info	m_List_Info;
	std::vector<CString>	m_AddFileList;	//文件添加列表
	CDlgChoPerson	m_ChoPerson;	//姓名获取
	CString			m_GetDetailInfo;//获取详细情况 
	int				m_disp_Num;		//记录类型
	CString			m_RegistrNum;	//获取的报名号
	CString			m_Detail;		//详细情况
	CMenu			pPopMenu;		//菜单
	vector<int>    m_Own_Value;		//事件号
	vector<string> m_Discription;	//事件描述
	CListUI*		pListReport;	//list列表
	CListTextElementUI* pListElementRp;


	CString BootOpenDialog();
	bool	InitMenu();
	void	Recurrence(string ver,vector<EventNode>& nodes,CMenu *menue);
	void	GetVersion();
	bool	ShowReport();

public:
	CPaintManagerUI m_pm;
	IEloamViewPtr m_pView1;
	IEloamViewPtr m_pView2;


	//zjd
	EXAM_ROSTER  m_roster;
	EXAM_ENTRANCE m_entrance;
	CLoginWnd *  m_pLoginWnd;
	//Algo m_algo;
	//VYFeature m_vyfeature1,m_vyfeature2;//图片对比算法
	int m_nTimes;						  //考生当前进场次数 == 花名册次数+1
	CEncryptyFile m_cEncryptyFile;		  //图片加密
	std::string m_key;					  //图片加密密钥
	std::string m_examid;				  //考场号  == g_examid
	std::string m_regID;				  //报名号	== m_roster.fix_regid
	char        m_LicencePath[250];		  //证件照png路径
	char		m_LicenceHeaderPath[250]; //大头照
	char		m_curPicPath[250];		  //当前照
	char		m_hisPicPath[250];		  //历史照
	char		m_lastPicPath[250];		  //上一张
	int			m_iRetry;				  //二代证比对时重新核验次数entrancei++.png
};

