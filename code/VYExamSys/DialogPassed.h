#pragma once

#include "..\\UI\MainFrame.h"
// DialogPassed �Ի���
class CMainFrame;

class DialogPassed : public CDialog
{
	DECLARE_DYNAMIC(DialogPassed)

public:
	DialogPassed(CString leftPath,CString rightPath,CMainFrame* pFrame,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogPassed();

// �Ի�������
	enum { IDD = IDD_DIALOGPASSED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRefuse2();
	virtual BOOL OnInitDialog();
	char m_picLeftPath[250];	//���ͼƬ
	char m_picRightPath[250];	//�Ҳ�ͼƬ
	CMainFrame* m_pFrame;
	afx_msg void OnPaint();
};
