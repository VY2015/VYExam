#pragma once

#include "..\\UI\MainFrame.h"
// IDDialogNotPassed �Ի���
class CMainFrame;

class IDDialogNotPassed : public CDialog
{
	DECLARE_DYNAMIC(IDDialogNotPassed)

public:
	IDDialogNotPassed(CString leftPath,CString rightPath,CMainFrame* pFrame,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~IDDialogNotPassed();

// �Ի�������
	enum { IDD = IDD_IDDIALOGNOTPASSED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	char m_picLeftPath[250];	//���ͼƬ
	char m_picRightPath[250];	//�Ҳ�ͼƬ
	CMainFrame* m_pFrame;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRefuse();
};
