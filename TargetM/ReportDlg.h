
#include "xShadeButton.h"
#include "PrintListCtrl.h"
#include "TargetV.h"

#pragma once


// диалоговое окно CReportDlg

class CReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReportDlg)

public:
	CReportDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CReportDlg();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

public:

	CString* m_Report;

	afx_msg void OnBnClickedButtonPrint();

	CListBox m_REPORT_List;
	void ViewReport(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();

	CPrintListCtrl m_Printer;

	CListCtrl m_REPORT_ListCtrl;
	int CreateReport(void);

	CTargetM* m_Target;

	CListCtrl m_TESTRES_ListCtrl;
	CListBox m_TESTRES_ListBox;


};
