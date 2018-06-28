// TestParDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "TargetM.h"
#include "TestParDlg.h"
#include "afxdialogex.h"
//#include "TargetM.h"

// диалоговое окно CTestParDlg

IMPLEMENT_DYNAMIC(CTestParDlg, CDialogEx)

CTestParDlg::CTestParDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestParDlg::IDD, pParent)
	, m_4836_Bool(FALSE)
{

}

CTestParDlg::~CTestParDlg()
{
}

void CTestParDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GAPS, m_GAPS_Combo);
	DDX_Control(pDX, IDC_EDIT_INRECT, m_INRECT_Edit);
	DDX_Control(pDX, IDC_EDIT_INCIRCLE, m_INCIRCLE_Edit);
	DDX_Control(pDX, IDC_EDIT_DOC, m_DOC_Edit);
	DDX_Control(pDX, IDC_EDIT_MARK, m_MARK_Edit);
	DDX_Control(pDX, IDC_COMBO_CIRCLE, m_CIRCLE_Combo);
	DDX_Control(pDX, IDC_COMBO_RECTH, m_RECTH_Combo);
	DDX_Control(pDX, IDC_COMBO_RECTW, m_RECTW_Combo);
	DDX_Control(pDX, IDC_COMBO_TPX, m_TPX_Combo);
	DDX_Control(pDX, IDC_COMBO_TPY, m_TPY_Combo);
	DDX_Control(pDX, IDC_COMBO_TPR, m_TPR_Combo);
	DDX_Control(pDX, IDC_COMBO_BANDH, m_BANDH_Combo);
	DDX_Control(pDX, IDC_COMBO_BANDW, m_BANDW_Combo);
	DDX_Control(pDX, IDC_COMBO_BANDE, m_BANDE_Combo);
	DDX_Check(pDX, IDC_CHECK_4836, m_4836_Bool);
}


BEGIN_MESSAGE_MAP(CTestParDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTestParDlg::OnBnClickedOk)
	ON_CBN_EDITCHANGE(IDC_COMBO_CIRCLE, &CTestParDlg::OnCbnEditchangeComboCircle)
	ON_CBN_EDITUPDATE(IDC_COMBO_CIRCLE, &CTestParDlg::OnCbnEditupdateComboCircle)
	ON_CBN_DROPDOWN(IDC_COMBO_CIRCLE, &CTestParDlg::OnCbnDropdownComboCircle)
	ON_CBN_SELCHANGE(IDC_COMBO_CIRCLE, &CTestParDlg::OnCbnSelchangeComboCircle)
	ON_WM_TIMER()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTestParDlg::OnDeltaposSpin2)
	ON_CBN_SELCHANGE(IDC_COMBO_RECTH, &CTestParDlg::OnCbnSelchangeComboRecth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTestParDlg::OnDeltaposSpin1)
END_MESSAGE_MAP()


// обработчики сообщений CTestParDlg


void CTestParDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений

	if (MessageBox(L"Сохранить изменения параметров испытания?",L"ВНИМАНИЕ!",1)!=IDOK) return;

	UpdateData(TRUE);

	SetIntPar(&this->m_GAPS_Combo,&m_Test->m_NGaps);

	SetIntPar(&this->m_CIRCLE_Combo,(int*)&m_Test->m_NCircleD);


	this->SetStrPar(&this->m_DOC_Edit, &this->m_Test->m_Doc);
	this->SetStrPar(&this->m_MARK_Edit,&this->m_Test->m_Mark);

	SetIntPar(&this->m_RECTH_Combo,(int*)&this->m_Test->m_Rect.h);
	SetIntPar(&this->m_RECTW_Combo,(int*)&this->m_Test->m_Rect.w);

	SetIntPar(&this->m_TPX_Combo,(int*)&this->m_Test->m_TPoint.x);
	SetIntPar(&this->m_TPY_Combo,(int*)&this->m_Test->m_TPoint.y);
	SetIntPar(&this->m_TPR_Combo,(int*)&this->m_Test->m_NTpointR);

	SetIntPar(&this->m_BANDH_Combo,(int*)&this->m_Test->m_NBandH);
	SetIntPar(&this->m_BANDW_Combo,(int*)&this->m_Test->m_NBandW);
	SetIntPar(&this->m_BANDE_Combo,(int*)&this->m_Test->m_NBandE);

	SetIntPar(&this->m_INRECT_Edit,(int*)&this->m_Test->m_NGinRect);
	SetIntPar(&this->m_INCIRCLE_Edit,(int*)&this->m_Test->m_NGinCircle);


	m_Test->m_N4836=(int)this->m_4836_Bool;

	m_Test->SaveIniPar();

	m_Test->DisplayTestPar();

	CDialogEx::OnOK();
}


void CTestParDlg::SetIntPar(CWnd* wnd,int* par)
{
	CString s;
	CString& ss=s;
	wnd->GetWindowTextW(ss);
	*par=StrToInt(s);
}


BOOL CTestParDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию

	CString title;
	title.Format(L"Настройка параметров испытания [%d]",m_Test->m_TNum);
	SetWindowText(title);

	this->m_GAPS_Combo.AddString(L"0");
	this->m_GAPS_Combo.AddString(L"4");
	this->m_GAPS_Combo.AddString(L"10");
	this->m_GAPS_Combo.AddString(L"20");
	this->m_GAPS_Combo.AddString(L"60");

	this->m_CIRCLE_Combo.AddString(L"0");
	this->m_CIRCLE_Combo.AddString(L"10");
	this->m_CIRCLE_Combo.AddString(L"15");
	this->m_CIRCLE_Combo.AddString(L"18");
	this->m_CIRCLE_Combo.AddString(L"20");

	this->m_RECTH_Combo.AddString(L"0");
	this->m_RECTH_Combo.AddString(L"12");
	this->m_RECTH_Combo.AddString(L"16");

	this->m_RECTW_Combo.AddString(L"0");
	this->m_RECTW_Combo.AddString(L"10");
	this->m_RECTW_Combo.AddString(L"14");

	this->m_TPX_Combo.AddString(L"0");

	this->m_TPY_Combo.AddString(L"0");
	this->m_TPY_Combo.AddString(L"13");
	this->m_TPY_Combo.AddString(L"14");
	this->m_TPY_Combo.AddString(L"16");
	this->m_TPY_Combo.AddString(L"26");


	this->m_TPR_Combo.AddString(L"0");
	this->m_TPR_Combo.AddString(L"5");
	this->m_TPR_Combo.AddString(L"7");
	this->m_TPR_Combo.AddString(L"10");

	this->m_BANDH_Combo.AddString(L"0");
	this->m_BANDH_Combo.AddString(L"12");
	this->m_BANDH_Combo.AddString(L"15");
	this->m_BANDH_Combo.AddString(L"20");

	this->m_BANDW_Combo.AddString(L"0");
	this->m_BANDW_Combo.AddString(L"12");
	this->m_BANDW_Combo.AddString(L"15");
	this->m_BANDW_Combo.AddString(L"18");
	this->m_BANDW_Combo.AddString(L"20");

	this->m_BANDE_Combo.AddString(L"0");
	this->m_BANDE_Combo.AddString(L"9");
	this->m_BANDE_Combo.AddString(L"10");
	this->m_BANDE_Combo.AddString(L"18");
	this->m_BANDE_Combo.AddString(L"20");


	ViewIntPar(&this->m_GAPS_Combo,this->m_Test->m_NGaps);
	ViewIntPar(&this->m_CIRCLE_Combo,this->m_Test->m_NCircleD);

	ViewIntPar(&this->m_RECTH_Combo,this->m_Test->m_Rect.h);
	ViewIntPar(&this->m_RECTW_Combo,this->m_Test->m_Rect.w);

	ViewIntPar(&this->m_TPX_Combo,this->m_Test->m_TPoint.x);
	ViewIntPar(&this->m_TPY_Combo,this->m_Test->m_TPoint.y);
	ViewIntPar(&this->m_TPR_Combo,this->m_Test->m_NTpointR);

	ViewIntPar(&this->m_BANDH_Combo,this->m_Test->m_NBandH);
	ViewIntPar(&this->m_BANDW_Combo,this->m_Test->m_NBandW);
	ViewIntPar(&this->m_BANDE_Combo,this->m_Test->m_NBandE);

	m_4836_Bool=(int)this->m_Test->m_N4836;

	this->ViewStrPar(&this->m_DOC_Edit,this->m_Test->m_Doc);
	this->ViewStrPar(&this->m_MARK_Edit,this->m_Test->m_Mark);

	this->ViewIntPar(&this->m_INRECT_Edit,this->m_Test->m_NGinRect);
	this->ViewIntPar(&this->m_INCIRCLE_Edit,this->m_Test->m_NGinCircle);


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CTestParDlg::ViewIntPar(CWnd* wnd, int par)
{
	CString s;
	s.Format(L"%d",par);
	wnd->SetWindowTextW(s);
}


void CTestParDlg::SetStrPar(CWnd* wnd,CString* par)
{
	CString s;
	CString& ss=s;
	wnd->GetWindowTextW(ss);
	*par=(s);
}


void CTestParDlg::ViewStrPar(CWnd* wnd, CString par)
{
	wnd->SetWindowTextW(par);
}


void CTestParDlg::OnCbnEditchangeComboCircle()
{
	// TODO: добавьте свой код обработчика уведомлений


	return;
}


void CTestParDlg::OnCbnEditupdateComboCircle()
{
	// TODO: добавьте свой код обработчика уведомлений

	return;
}


void CTestParDlg::OnCbnDropdownComboCircle()
{
	// TODO: добавьте свой код обработчика уведомлений

	return;
}


void CTestParDlg::OnCbnSelchangeComboCircle()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetTimer(30,10,NULL);
	return;
}


void CTestParDlg::UpdateInCyrcle(void)
{
	UpdateData(TRUE);
	CString s;
	CString& ss=s;
	this->m_GAPS_Combo.GetWindowTextW(ss);
	int ipar=StrToInt(s);
	ipar*=80;
	ipar/=100;

	this->m_CIRCLE_Combo.GetWindowTextW(ss);
	if (StrToInt(s)==0) ipar=0;

	s.Format(L"%d",ipar);
	this->m_INCIRCLE_Edit.SetWindowTextW(s);
}

void CTestParDlg::UpdateInRect(void)
{
	UpdateData(TRUE);
	CString s;
	CString& ss=s;
	this->m_GAPS_Combo.GetWindowTextW(ss);
	int ipar=StrToInt(s);
	ipar*=80;
	ipar/=100;

	this->m_RECTH_Combo.GetWindowTextW(ss);
	if (StrToInt(s)==0) ipar=0;

	s.Format(L"%d",ipar);
	this->m_INRECT_Edit.SetWindowTextW(s);
}


void CTestParDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	if (nIDEvent==30)
	{
		KillTimer(30);
		UpdateInCyrcle();
	}

	if (nIDEvent==31)
	{
		KillTimer(31);
		UpdateInRect();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTestParDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	CString s;
	CString& ss=s;
	this->m_INCIRCLE_Edit.GetWindowTextW(ss);
	int ipar=StrToInt(s);
	this->m_GAPS_Combo.GetWindowTextW(ss);
	int ilpar=StrToInt(s);

	if (pNMUpDown->iDelta<0) ipar++;
	if (pNMUpDown->iDelta>0) ipar--;

	if (ipar<0) ipar=0;
	if (ipar>ilpar) ipar=ilpar;

	s.Format(L"%d",ipar);
	m_INCIRCLE_Edit.SetWindowTextW(s);

	*pResult = 0;
}


void CTestParDlg::OnCbnSelchangeComboRecth()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetTimer(31,10,NULL);
}


void CTestParDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	CString s;
	CString& ss=s;
	this->m_INRECT_Edit.GetWindowTextW(ss);
	int ipar=StrToInt(s);
	this->m_GAPS_Combo.GetWindowTextW(ss);
	int ilpar=StrToInt(s);

	if (pNMUpDown->iDelta<0) ipar++;
	if (pNMUpDown->iDelta>0) ipar--;

	if (ipar<0) ipar=0;
	if (ipar>ilpar) ipar=ilpar;

	s.Format(L"%d",ipar);
	m_INRECT_Edit.SetWindowTextW(s);

	*pResult = 0;
}
