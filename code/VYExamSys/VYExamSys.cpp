
// VYExamSys.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "VYExamSys.h"


//��½����
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


// CVYExamSysApp ����

CVYExamSysApp::CVYExamSysApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CVYExamSysApp ����

CVYExamSysApp theApp;


// CVYExamSysApp ��ʼ��

BOOL CVYExamSysApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	HRESULT hr=CoInitialize(NULL);
	if (FAILED(hr))
	{
		return FALSE;
	}

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//eloamCom
	g_oVYDevice.Setup();

	//UI
	CPaintManagerUI::SetInstance(CWinApp::m_hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("Skins"));

	InstanceDB a(_T("..\\DB\\VYExamDB"));

	//��½����
	CLoginWnd* pLoginFrame = new CLoginWnd;
	if(pLoginFrame == NULL) return 0;
	pLoginFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginFrame->CenterWindow();
	UINT nResponse = pLoginFrame->ShowModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���		
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	//@feng���ؽ�����
	CDownLoadProgressBar* pLoginFrame1 = new CDownLoadProgressBar;
	if(pLoginFrame1 == NULL) return 0;
	pLoginFrame1->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginFrame1->CenterWindow();
	nResponse = pLoginFrame1->ShowModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: �ڴ˷��ô����ʱ��
	//	//  ��ȷ�������رնԻ���Ĵ���		
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: �ڴ˷��ô����ʱ��
	//	//  ��ȡ�������رնԻ���Ĵ���
	//}

	
	//����ܴ���
	CMainFrame* pFrame = new CMainFrame();
	//pFrame->setLoginWnd(pLoginFrame);	//���������֤�ص���Ҫ����ʱ��
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("���Ժ���ϵͳ"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 0, 0);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();


	// ɾ�����洴���� shell ��������


		
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	
	CoUninitialize();

	return FALSE;
}



