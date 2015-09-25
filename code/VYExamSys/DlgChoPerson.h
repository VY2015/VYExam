#pragma once
#include "afxcmn.h"

#include "vyexamfiledef.h"
// CDlgChoPerson 对话框

class CDlgChoPerson : public CDialog
{
	DECLARE_DYNAMIC(CDlgChoPerson)

public:
	CDlgChoPerson(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChoPerson();

// 对话框数据
	enum { IDD = IDD_DLGCHOPERSON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl		m_listCtrl;
	int				m_nSelectColumn;
	int				m_nSelectLine;
	bool			m_bHit;
	int				m_itemSel;

	CString			m_RegId;
	CString			m_name;
	CString			m_Attend;
	CString			m_Seat;
	CString			m_Times;



	Report_List_Info	St_list_Info;


	void			InsertListCtrl();
	virtual BOOL	OnInitDialog();
	void			GetItemInfo(NMHDR *pNMHDR, LRESULT *pResult);
	void			OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL			PreTranslateMessage(MSG* pMsg);
	void			OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult) ;
	afx_msg void	OnSize(UINT nType, int cx, int cy);


	afx_msg void OnBnClickedOk();
};
