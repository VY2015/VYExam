#pragma once

#include "..\\UI\MainFrame.h"
// IDDialogNotPassed 对话框
class CMainFrame;

class IDDialogNotPassed : public CDialog
{
	DECLARE_DYNAMIC(IDDialogNotPassed)

public:
	IDDialogNotPassed(CString leftPath,CString rightPath,CMainFrame* pFrame,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~IDDialogNotPassed();

// 对话框数据
	enum { IDD = IDD_IDDIALOGNOTPASSED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	char m_picLeftPath[250];	//左侧图片
	char m_picRightPath[250];	//右侧图片
	CMainFrame* m_pFrame;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRefuse();
};
