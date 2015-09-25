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
	
	void doClearBuffer();			//��մ���ͼƬ��·����������Ȼ���
	void clearRoster();
	void clearEntrance();
	inline void setLoginWnd(CLoginWnd * login){ m_pLoginWnd = login;}
	void showIDCardUI();			//�������֤�������ʾ
	void capCurrentPicCompare();	//���ղ��Ƚ�
	void doIDPassed();				//����֤�Ƚ�ͨ��
	void doClearUIandBuffer();		//����֤��ջ���ͽ���

	void initID2Check();					//�л��Ƕ���֤��֤���漴��ʼ��
	void showID2UI(EXAM_ROSTER eRoster);	//�Ƕ���֤��ʾ������͵ڼ����볡(���ѡ����)
	void doClearID2UIandBuffer();			//�Ƕ���֤��ս���ͻ���

//	int					m_ListNum;	//��ȡ�б��к� 
	Report_List_Info	m_List_Info;
	std::vector<CString>	m_AddFileList;	//�ļ�����б�
	CDlgChoPerson	m_ChoPerson;	//������ȡ
	CString			m_GetDetailInfo;//��ȡ��ϸ��� 
	int				m_disp_Num;		//��¼����
	CString			m_RegistrNum;	//��ȡ�ı�����
	CString			m_Detail;		//��ϸ���
	CMenu			pPopMenu;		//�˵�
	vector<int>    m_Own_Value;		//�¼���
	vector<string> m_Discription;	//�¼�����
	CListUI*		pListReport;	//list�б�
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
	//VYFeature m_vyfeature1,m_vyfeature2;//ͼƬ�Ա��㷨
	int m_nTimes;						  //������ǰ�������� == ���������+1
	CEncryptyFile m_cEncryptyFile;		  //ͼƬ����
	std::string m_key;					  //ͼƬ������Կ
	std::string m_examid;				  //������  == g_examid
	std::string m_regID;				  //������	== m_roster.fix_regid
	char        m_LicencePath[250];		  //֤����png·��
	char		m_LicenceHeaderPath[250]; //��ͷ��
	char		m_curPicPath[250];		  //��ǰ��
	char		m_hisPicPath[250];		  //��ʷ��
	char		m_lastPicPath[250];		  //��һ��
	int			m_iRetry;				  //����֤�ȶ�ʱ���º������entrancei++.png
};

