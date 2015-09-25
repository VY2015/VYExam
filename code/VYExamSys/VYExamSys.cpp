
// VYExamSys.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "VYExamSys.h"


//登陆界面
#include "DownLoadProgressBar.h"
#include "UpLoadProgressBar.h"
#include "LoginWnd.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVYExamSysApp

BEGIN_MESSAGE_MAP(CVYExamSysApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVYExamSysApp 构造

CVYExamSysApp::CVYExamSysApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CVYExamSysApp 对象

CVYExamSysApp theApp;


// CVYExamSysApp 初始化

BOOL CVYExamSysApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	HRESULT hr=CoInitialize(NULL);
	if (FAILED(hr))
	{
		return FALSE;
	}

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//eloamCom
	g_oVYDevice.Setup();

	//UI
	CPaintManagerUI::SetInstance(CWinApp::m_hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("Skins"));

	InstanceDB a(_T("..\\DB\\VYExamDB"));

	//登陆界面
	CLoginWnd* pLoginFrame = new CLoginWnd;
	if(pLoginFrame == NULL) return 0;
	pLoginFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginFrame->CenterWindow();
	UINT nResponse = pLoginFrame->ShowModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码		
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	//@feng下载进度条
	CDownLoadProgressBar* pLoginFrame1 = new CDownLoadProgressBar;
	if(pLoginFrame1 == NULL) return 0;
	pLoginFrame1->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginFrame1->CenterWindow();
	nResponse = pLoginFrame1->ShowModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: 在此放置处理何时用
	//	//  “确定”来关闭对话框的代码		
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: 在此放置处理何时用
	//	//  “取消”来关闭对话框的代码
	//}

	
	//主框架窗口
	CMainFrame* pFrame = new CMainFrame();
	//pFrame->setLoginWnd(pLoginFrame);	//二代身份验证回调需要考试时间
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("考试核验系统"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 0, 0);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();


	// 删除上面创建的 shell 管理器。


		
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	
	CoUninitialize();

	return FALSE;
}



