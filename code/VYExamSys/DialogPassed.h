#pragma once

#include "..\\UI\MainFrame.h"
// DialogPassed 对话框
class CMainFrame;

class DialogPassed : public CDialog
{
	DECLARE_DYNAMIC(DialogPassed)

public:
	DialogPassed(CString leftPath,CString rightPath,CMainFrame* pFrame,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogPassed();

// 对话框数据
	enum { IDD = IDD_DIALOGPASSED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRefuse2();
	virtual BOOL OnInitDialog();
	char m_picLeftPath[250];	//左侧图片
	char m_picRightPath[250];	//右侧图片
	CMainFrame* m_pFrame;
	afx_msg void OnPaint();
};
