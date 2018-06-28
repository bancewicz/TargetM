#pragma once
#include "afxwin.h"
#include "TargetV.h"

//class CTargetM;

// диалоговое окно CTestParDlg

class CTestParDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestParDlg)

public:
	CTestParDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CTestParDlg();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_GAPS_Combo;
	CEdit m_INRECT_Edit;
	CEdit m_INCIRCLE_Edit;

	CTestM* m_Test;

	void SetIntPar(CWnd* wnd,int* par);
	virtual BOOL OnInitDialog();
	void ViewIntPar(CWnd* wnd, int par);
	void SetStrPar(CWnd* wnd,CString* par);
	void ViewStrPar(CWnd* wnd, CString par);
	CEdit m_DOC_Edit;
	CEdit m_MARK_Edit;
	CComboBox m_CIRCLE_Combo;
	CComboBox m_RECTH_Combo;
	CComboBox m_RECTW_Combo;
	CComboBox m_TPX_Combo;
	CComboBox m_TPY_Combo;
	CComboBox m_TPR_Combo;
	CComboBox m_BANDH_Combo;
	CComboBox m_BANDW_Combo;
	CComboBox m_BANDE_Combo;
	afx_msg void OnCbnEditchangeComboCircle();
	afx_msg void OnCbnEditupdateComboCircle();
	afx_msg void OnCbnDropdownComboCircle();
	afx_msg void OnCbnSelchangeComboCircle();
	void UpdateInCyrcle(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboRecth();
	void UpdateInRect(void);

	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_4836_Bool;
};
