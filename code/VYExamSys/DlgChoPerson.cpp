
#include "stdafx.h"
#include "VYExamSys.h"
#include "DlgChoPerson.h"
#include "afxdialogex.h"

#include <ctime>
#include <Windows.h>
#include "vyexamfiledef.h"


IMPLEMENT_DYNAMIC(CDlgChoPerson, CDialog)

CDlgChoPerson::CDlgChoPerson(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChoPerson::IDD, pParent)
{

}

CDlgChoPerson::~CDlgChoPerson()
{
}

void CDlgChoPerson::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CDlgChoPerson, CDialog)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgChoPerson::OnNMClickList2)
	ON_BN_CLICKED(IDOK, &CDlgChoPerson::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CDlgChoPerson::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listCtrl.DeleteAllItems();

	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_HEADERDRAGDROP |LVS_EX_CHECKBOXES |LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listCtrl.InsertColumn(0,L"", LVCFMT_CENTER,95,-1);

	LVCOLUMN col;
	col.fmt = LVCFMT_CENTER;
	col.mask= LVCF_TEXT | LVCF_FMT;
	col.pszText = L"NO";
	col.cchTextMax= 256;
	m_listCtrl.SetColumn(0, &col);


	m_listCtrl.InsertColumn(1, L"4",LVCFMT_CENTER,85,-1);
	m_listCtrl.InsertColumn(2, L"45",LVCFMT_CENTER,85,-1);
	m_listCtrl.InsertColumn(3, L"34",LVCFMT_CENTER,85,-1);
	m_listCtrl.InsertColumn(4, L"23",LVCFMT_CENTER,85,-1);
	m_listCtrl.InsertColumn(5, L"21",LVCFMT_CENTER,85,-1);


	
	m_listCtrl.SetBkColor(RGB(255,255,255));     //
	
	InsertListCtrl();

	CRect rcDlgs;
	GetWindowRect(rcDlgs);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(cx/2 - rcDlgs.Width()+33,cy/2 - rcDlgs.Height()/2+53,rcDlgs.Width(),rcDlgs.Height(),TRUE);

	return TRUE; 
}


void CDlgChoPerson::InsertListCtrl()
{
	CTime t = CTime::GetCurrentTime();
	CString date(t.Format("%Y/%m/%d\r\n"));
	CString dateTime = date;


 	int nIndex = m_listCtrl.GetItemCount();
// 	CString strIndex;
// 	strIndex.Format(L"%d", nIndex + 1);

	char buf[32];
	for(int i = 0 ; i < 10 ; i++)
	{
		itoa(1,buf,32);
		CStringA te(buf);
		CString strIndex(te.GetBuffer());
		te.ReleaseBuffer();

		m_listCtrl.InsertItem(LVIF_TEXT|LVIF_STATE,nIndex, strIndex, 0,LVIS_SELECTED,0,0);

		m_listCtrl.SetItemText(nIndex,1,L"start");
		m_listCtrl.SetItemText(nIndex,2,L"stop");
		m_listCtrl.SetItemText(nIndex,3,L"1212312");
		m_listCtrl.SetItemText(nIndex,4,L"rest");
		m_listCtrl.SetItemText(nIndex,5,strIndex);
	}
	
}


void CDlgChoPerson::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	
}

//选择全部
 BOOL CDlgChoPerson::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == 'A')
        {
            if (::GetKeyState(VK_CONTROL)<0)
            {
                for (int i = 0; i<m_listCtrl.GetItemCount(); i++)
                {
                    m_listCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					 m_listCtrl.SetCheck(i);
                }
            }
        }
		if(pMsg->wParam == 'Z')
		{
			if (::GetKeyState(VK_CONTROL)<0)
			{
				for (int i = 0; i<m_listCtrl.GetItemCount(); i++)
				{
					//m_listCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					m_listCtrl.SetCheck(i,FALSE);
				}
			}
		}
    }
    return FALSE;
 }

void CDlgChoPerson::OnBnClickedOk()
 {
	 // TODO: 在此添加控件通知处理程序代码

	 for(int i =0 ; i < 20 ;i++)
	 {
		if(m_listCtrl.GetCheck(i))  //获取复选框状态
		{

			m_RegId = m_RegId + "," +m_listCtrl.GetItemText(i, 3);     


			CStringA reg(m_RegId.GetBuffer());
			strcpy(St_list_Info.RegisterId,reg);

			m_name = m_name + "," +m_listCtrl.GetItemText(i, 1);
			CStringA nameA(m_name.GetBuffer());
			strcpy(St_list_Info.Name,nameA);


			m_Attend = m_Attend + "," +m_listCtrl.GetItemText(i, 2);
			CStringA AttendA(m_Attend.GetBuffer());
			strcpy(St_list_Info.Attend,AttendA);

			m_Seat  = m_Seat + "," +m_listCtrl.GetItemText(i, 4);
			CStringA SeatA(m_Seat.GetBuffer());
			strcpy(St_list_Info.Seat,SeatA);


			m_Times = m_Times + "," +m_listCtrl.GetItemText(i, 5);
			CStringA TimesA(m_Times.GetBuffer());
			strcpy(St_list_Info.Times,TimesA);

		}

		//	continue;
	 }

	 CDialog::OnOK();
 }