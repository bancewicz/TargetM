
// TargetMDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"

#include "xShadeButton.h"

#include "serial.h"
#include "SerialLight.h"

#include "MyEdit.h"
#include "TargetV.h"

#include "graph.h"
#include "IniFileManager.h"
#include "FlashScreenDlg.h"

class CTargetMDlgAutoProxy;

// диалоговое окно CTargetMDlg
class CTargetMDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTargetMDlg);
	friend class CTargetMDlgAutoProxy;

// Создание
public:
	CTargetMDlg(CWnd* pParent = NULL);	// стандартный конструктор
	virtual ~CTargetMDlg();

// Данные диалогового окна
	enum { IDD = IDD_TARGETM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	CTargetMDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTargetRequest();

	CxShadeButton m_COMOPEN_Btn;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//CSerial m_Com;

	CSerialLight m_Com;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CComboBox m_COM_Combo;
	afx_msg void OnBnClickedButtonComOpen();
	void ComPortScan(void);
	CxShadeButton m_OK_Btn;
	CxShadeButton m_TARGREQUEST_Btn;
	CxShadeButton m_RESET_Btn;
	CComboBox m_ADDR_Combo;
	CEdit m_OPERATOR_Edit;
	CMyEdit m_STATE_Edit;
	void SetState(void);
	afx_msg void OnBnClickedButtonTest1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTest2();

	CTargetM m_Target;
	afx_msg void OnBnClickedButtonTest3();
	afx_msg void OnBnClickedButtonTest4();
	afx_msg void OnBnClickedButtonTest5();
	CEdit m_TICKS_Edit;
	CEdit m_AREA_Edit;
	CEdit m_X_Edit;
	CEdit m_Y_Edit;
	CEdit m_INFO_Edit;
	CEdit m_GAP_Edit;
	CComboBox m_GAP_Combo;
	CxShadeButton m_SEEK_Btn;
	CxShadeButton m_ON_Btn;
	CxShadeButton m_OFF_Btn;
	CEdit m_CONSOLEC_Edit;
	CEdit m_CONSOLER_Edit;
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSeek();
	void SetTargetPar(void);
	afx_msg void OnBnClickedButtonOn();
	afx_msg void OnBnClickedButtonOff();
	BOOL m_AUTOINC_Bool;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL SendTargetCmd(TargetM_cmd cmd);

	CGraph* m_pGraph;
	
	void InitGraph(void);
	CSliderCtrl m_RANG_Slider;
	afx_msg void OnNMThemeChangedSliderRang(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderRang(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSliderRang(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMCustomdrawSliderRang(NMHDR *pNMHDR, LRESULT *pResult);
	CxShadeButton m_FIRE_Btn;
	afx_msg void OnBnClickedButtonFire();
	CButton m_AUTO_Check;
	afx_msg void OnEnChangeEditY();
	afx_msg void OnBnClickedCheckAutoinc();
	afx_msg void OnCbnEditchangeComboGap();
	CEdit m_METRIC_Edit;
	afx_msg void OnNMCustomdrawSliderRang(NMHDR *pNMHDR, LRESULT *pResult);
	CMyEdit m_DISPLAY_Edit;
	CxShadeButton m_PROTOCOL_Btn;
	CComboBox m_USER_Combo;
	afx_msg void OnCbnSelchangeComboGap();

	CIniFileManager m_Ini;
	void SaveIniPar(void);
	afx_msg void OnDestroy();
	void LoadIniPar(void);
	afx_msg void OnCbnKillfocusComboUser();
	afx_msg void OnBnClickedButtonProtocol();
	void CreateReport(void);
	void ViewReport(CString* report);
	CStatic m_BANNER_static;
	CStatic m_BANNER_FRAME_Static;
	CStatic m_COMPORT_Static;
	CStatic m_ADDR_Static;


	afx_msg void OnEnSetfocusEditState();
	void PutConsoleC(CString str);
	void PutConsoleR(CString str);
	CString m_CONSOLER_Str;
	CComboBox m_TESTNUM_Combo;
	CEdit m_TESTPAR_Edit;
	afx_msg void OnCbnEditchangeComboTestnum();
	afx_msg void OnCbnSelchangeComboTestnum();
	CxShadeButton m_TESTPAR_Btn;
	afx_msg void OnBnClickedButtonTestpar();
};
