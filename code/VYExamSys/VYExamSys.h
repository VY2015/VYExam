
// VYExamSys.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CVYExamSysApp:
// �йش����ʵ�֣������ VYExamSys.cpp
//

class CVYExamSysApp : public CWinApp
{
public:
	CVYExamSysApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVYExamSysApp theApp;