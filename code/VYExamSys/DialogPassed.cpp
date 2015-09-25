// DialogPassed.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VYExamSys.h"
#include "DialogPassed.h"
#include "afxdialogex.h"


// DialogPassed �Ի���

IMPLEMENT_DYNAMIC(DialogPassed, CDialog)

DialogPassed::DialogPassed(CString leftPath,CString rightPath,CMainFrame* pFrame,CWnd* pParent /*=NULL*/)
	: CDialog(DialogPassed::IDD, pParent)
{
	strcpy_s(m_picLeftPath,sizeof(m_picLeftPath),CStringA(leftPath));
	strcpy_s(m_picRightPath,sizeof(m_picRightPath),CStringA(rightPath));
	m_pFrame = pFrame;
}

DialogPassed::~DialogPassed()
{
}

void DialogPassed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogPassed, CDialog)
	ON_BN_CLICKED(IDOK, &DialogPassed::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DialogPassed::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_REFUSE2, &DialogPassed::OnBnClickedRefuse2)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DialogPassed ��Ϣ�������


void DialogPassed::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


void DialogPassed::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


void DialogPassed::OnBnClickedRefuse2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pFrame->doClearUIandBuffer();
	CDialog::OnCancel();

}

BOOL DialogPassed::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void DialogPassed::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CImage img;
	CRect rect;
	GetDlgItem(IDC_STATICleft)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CDC* pDC = GetDlgItem(IDC_STATICleft)->GetDC();
	if (S_OK != img.Load(CString(m_picLeftPath)))
		return;
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, img.GetWidth(), img.GetHeight());
	GetDlgItem(IDC_STATICleft)->ReleaseDC(pDC);
	CImage img2;
	GetDlgItem(IDC_STATICright)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CDC *pDC2 = GetDlgItem(IDC_STATICright)->GetDC();
	if (S_OK != img2.Load(CString(m_picRightPath)))
		return;
	pDC2->SetStretchBltMode(STRETCH_HALFTONE);
	img2.Draw(pDC2->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, img2.GetWidth(), img2.GetHeight());
	GetDlgItem(IDC_STATICright)->ReleaseDC(pDC2);
}
