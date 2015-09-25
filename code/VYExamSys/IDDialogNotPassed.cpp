// IDDialogNotPassed.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VYExamSys.h"
#include "IDDialogNotPassed.h"
#include "afxdialogex.h"


// IDDialogNotPassed �Ի���

IMPLEMENT_DYNAMIC(IDDialogNotPassed, CDialog)

IDDialogNotPassed::IDDialogNotPassed(CString leftPath,CString rightPath,CMainFrame* pFrame,CWnd* pParent /*=NULL*/)
	: CDialog(IDDialogNotPassed::IDD, pParent)
{
	strcpy_s(m_picLeftPath,sizeof(m_picLeftPath),CStringA(leftPath));
	strcpy_s(m_picRightPath,sizeof(m_picRightPath),CStringA(rightPath));
	m_pFrame = pFrame;
}

IDDialogNotPassed::~IDDialogNotPassed()
{
}

void IDDialogNotPassed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(IDDialogNotPassed, CDialog)
	ON_BN_CLICKED(IDOK, &IDDialogNotPassed::OnBnClickedOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_REFUSE, &IDDialogNotPassed::OnBnClickedRefuse)
END_MESSAGE_MAP()


// IDDialogNotPassed ��Ϣ�������


void IDDialogNotPassed::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


void IDDialogNotPassed::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CImage img;
	CRect rect;
	GetDlgItem(IDC_STATICleft2)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CDC* pDC = GetDlgItem(IDC_STATICleft2)->GetDC();
	if (S_OK != img.Load(CString(m_picLeftPath)))
		return;
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, img.GetWidth(), img.GetHeight());
	GetDlgItem(IDC_STATICleft2)->ReleaseDC(pDC);
	CImage img2;
	GetDlgItem(IDC_STATICright2)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CDC *pDC2 = GetDlgItem(IDC_STATICright2)->GetDC();
	if (S_OK != img2.Load(CString(m_picRightPath)))
		return;
	pDC2->SetStretchBltMode(STRETCH_HALFTONE);
	img2.Draw(pDC2->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, img2.GetWidth(), img2.GetHeight());
	GetDlgItem(IDC_STATICright2)->ReleaseDC(pDC2);
}


void IDDialogNotPassed::OnBnClickedRefuse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pFrame->doClearUIandBuffer();
	CDialog::OnCancel();
}
