// ReportDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "TargetM.h"
#include "ReportDlg.h"
#include "afxdialogex.h"

//#include "TargetM.h"

// диалоговое окно CReportDlg

IMPLEMENT_DYNAMIC(CReportDlg, CDialogEx)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReportDlg::IDD, pParent)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_REPORT, m_REPORT_List);
	DDX_Control(pDX, IDC_LISTC_REPORT, m_REPORT_ListCtrl);

	DDX_Control(pDX, IDC_LIST_TESTRES, m_TESTRES_ListBox);
	DDX_Control(pDX, IDC_LISTC_TESTRES, m_TESTRES_ListCtrl);


}


BEGIN_MESSAGE_MAP(CReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CReportDlg::OnBnClickedButtonPrint)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CReportDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// обработчики сообщений CReportDlg


void CReportDlg::OnBnClickedButtonPrint()
{
	// TODO: добавьте свой код обработчика уведомлений


	
		CString mystring(L"TEST PRINTING!");
		CString& smystring=mystring;

		//mystring=("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

	    CPrintDialog cp(FALSE);                         

		//LPRECT rect;
		
		CRect* rect=new CRect;

		rect->top=50;
		rect->left=350;
		rect->right=10000;
		rect->bottom=10000;

		mystring = "";

		int idx=0;

		for (idx=0; idx<100; idx++)
		{
			mystring+=m_Report[idx];
			mystring+="\r\n";
		}

		//while (mystring!="")
		{
			
			

			if (cp.DoModal()==IDOK)                         
			{
				CDC PrintDC;                                
				PrintDC.Attach(cp.GetPrinterDC());              
				DOCINFO infStru;                            
				::ZeroMemory (&infStru, sizeof (DOCINFO));          
				CString title=L"step 45 ";
				infStru.cbSize = sizeof (DOCINFO);                  
				infStru.lpszDocName=title;
				infStru.lpszOutput=NULL;
				if (PrintDC.StartDoc(&infStru )==-1)AfxMessageBox(L"Error StartDoc") ;  
 
				CFont font,poldfont;
				font.Detach();

				//font.CreatePointFont(10*10,L"Courier New",&PrintDC);
				//font.CreatePointFont(9*9,L"Courier New",&PrintDC);

				font.CreatePointFont(9*9,L"Courier new",&PrintDC);


				//font.CreatePointFont(9*9,L"Arial",&PrintDC);


				//poldfont=
					PrintDC.SelectObject(&font);

        
				PrintDC.MoveTo(9,10);
	//					PrintDC.DrawText("test text",-1,RECT|DT_VCENTER) ;   //проблема с параметрами  
						//PrintDC.DrawText(smystring,-1,DT_VCENTER) ;   //проблема с параметрами  

						//PrintDC.DrawText(smystring,1000,rect,DT_VCENTER);

						PrintDC.DrawText(smystring,rect,DT_VCENTER);



				if (PrintDC.EndPage()<0) AfxMessageBox(L"Error EndPage");       
				if (PrintDC.EndDoc()==0) AfxMessageBox(L"Error EndDoc");    

				//SelectObject(&poldfont);

				PrintDC.Detach();                           
			}
		}
}


void CReportDlg::ViewReport(void)
{
	int idx=0;


	//while(idx<100)
	//{
	//	if (m_Report[idx]=="") break;
	//	this->m_REPORT_List.AddString(m_Report[idx]);
	//	idx++;

	//this->m_REPORT_List.AddString(m_Report[0]);
	this->m_REPORT_List.AddString(L" ");
	this->m_REPORT_List.AddString(m_Report[1]);
	//this->m_REPORT_List.AddString(m_Report[2]);
	this->m_REPORT_List.AddString(m_Report[3]);
	//this->m_REPORT_List.AddString(m_Report[4]);

	//}

}


BOOL CReportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию



	this->m_REPORT_ListCtrl.InsertColumn(0,L"Пробоина"	  ,LVCFMT_LEFT,80);
	this->m_REPORT_ListCtrl.InsertColumn(1,L"Отметка времени"	  ,LVCFMT_LEFT,134);
	this->m_REPORT_ListCtrl.InsertColumn(2,L"Область"	  ,LVCFMT_LEFT,70);
	this->m_REPORT_ListCtrl.InsertColumn(3,L"X (мм)"	  ,LVCFMT_LEFT,120);
	this->m_REPORT_ListCtrl.InsertColumn(4,L"Y (мм)"	  ,LVCFMT_LEFT,120);
	this->m_REPORT_ListCtrl.InsertColumn(5,L"Попадает в круг(прямоуг.)"	  ,LVCFMT_LEFT,230);

	this->m_REPORT_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	this->m_TESTRES_ListCtrl.InsertColumn(0,L"Параметр"	  ,LVCFMT_LEFT,230);
	this->m_TESTRES_ListCtrl.InsertColumn(1,L"Значение"	  ,LVCFMT_LEFT,90);
	this->m_TESTRES_ListCtrl.InsertColumn(2,L"Норматив"	  ,LVCFMT_LEFT,90);
	this->m_TESTRES_ListCtrl.InsertColumn(3,L"Результат"  ,LVCFMT_LEFT,120);
	this->m_TESTRES_ListCtrl.InsertColumn(4,L"Примечание" ,LVCFMT_LEFT,220);

	this->m_TESTRES_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |LVS_EX_BORDERSELECT);

	SetTimer(1,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CReportDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного


	if (nIDEvent==1)
	{
		KillTimer(1);
		ViewReport();
		CreateReport();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CReportDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
}


int CReportDlg::CreateReport(void)
{

	this->m_REPORT_ListCtrl.DeleteAllItems();
	LVITEM lv;
	TCHAR ss[64];
	CString s;

	for (int i=0; i<this->m_Target->m_Test.m_NGaps; i++)
	{
		lv.iItem=i;
	    lv.iSubItem = 0;
		wsprintf(ss,L"%d",i+1);
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		m_REPORT_ListCtrl.InsertItem(&lv);

		
	    lv.iSubItem = 1;
			wsprintf(ss,L"%02d:%02d:%02d:%01d",
			this->m_Target->m_PULL.record[i].res.tick_hour,	
			this->m_Target->m_PULL.record[i].res.tick_min,	
			this->m_Target->m_PULL.record[i].res.tick_sec,	
			this->m_Target->m_PULL.record[i].res.tick_dec
			     );

		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		m_REPORT_ListCtrl.SetItem(&lv);


	    lv.iSubItem = 2;
			wsprintf(ss,L"%02d",
			this->m_Target->m_PULL.record[i].res.area	
			     );

		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		m_REPORT_ListCtrl.SetItem(&lv);


	    lv.iSubItem = 3;
		s.Format(L"%.1f",(double)this->m_Target->m_PULL.record[i].res.X/10.);	
		wsprintf(ss,L"%s",s);
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		m_REPORT_ListCtrl.SetItem(&lv);


	    lv.iSubItem = 4;
		s.Format(L"%.1f",(double)this->m_Target->m_PULL.record[i].res.Y/10.);	
		wsprintf(ss,L"%s",s);
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		m_REPORT_ListCtrl.SetItem(&lv);



	    lv.iSubItem = 5;
			wsprintf(ss,L"%d",
				this->m_Target->m_PULL.record[i].res.inCyrcle|
				this->m_Target->m_PULL.record[i].res.inRect
			     );

		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		m_REPORT_ListCtrl.SetItem(&lv);
		


		m_REPORT_ListCtrl.SetItem(&lv);
	}



	m_TESTRES_ListBox.AddString(L" ");
	m_TESTRES_ListBox.AddString(L"Результаты испытания");
	m_TESTRES_ListBox.AddString(this->m_Target->m_Test.m_Mark);

	CListCtrl* list=&this->m_TESTRES_ListCtrl;

	list->DeleteAllItems();

	int item=0;

//Число пробоин
	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"Число пробоин");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	wsprintf(ss,L"%d",m_Target->m_Test.m_NGaps);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	wsprintf(ss,L"%d",m_Target->m_Test.m_GAll);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 3;
	wsprintf(ss,L" ");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);
	
	lv.iSubItem = 4;
	wsprintf(ss,L" ");
//	if (m_Target->m_Test.m_RejectPointIdx>0) wsprintf(ss,L"Одна оторвавшаяся пробоина");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

//---------------------------------------------------------------------------------------------------

//СТП

	if (m_Target->m_Test.m_NTpointR)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"СТП X");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",((double)m_Target->m_Test.m_STP.x)/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

//СТП
	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"СТП Y");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",((double)m_Target->m_Test.m_STP.y)/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);


//Отклонение СТП от КТ

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"КТ X");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",((double)m_Target->m_Test.m_TPoint.x)/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"КТ Y");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",((double)m_Target->m_Test.m_TPoint.y)/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);


	//отклонение СТП
		lv.iItem = item++;

		lv.iSubItem = 0;
		wsprintf(ss,L"Отклонение СТП");
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		list->InsertItem(&lv);

		lv.iSubItem = 1;
		s.Format(L"%.1f",((double)m_Target->m_Test.m_TpointR)/10.);
		wsprintf(ss,L"%s",s);
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		list->SetItem(&lv);

		lv.iSubItem = 2;
		s.Format(L"%.1f",((double)m_Target->m_Test.m_NTpointR)*10.);
		wsprintf(ss,L"%s",s);
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		list->SetItem(&lv);

		lv.iSubItem = 3;
		s.Format(L"НОРМА"); if (m_Target->m_Test.m_Res.stpdevN==0) s.Format(L"брак");
		wsprintf(ss,L"%s",s);
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		list->SetItem(&lv);
	}

//Пробоин в прямоугольнике
	
	if (m_Target->m_Test.m_NGinRect)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"В прямоугольнике %dx%d",this->m_Target->m_Test.m_Rect.h,this->m_Target->m_Test.m_Rect.w);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%d",m_Target->m_Test.m_GinRect);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	s.Format(L"%d",m_Target->m_Test.m_NGinRect);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);


	lv.iSubItem = 3;
	s=L"НОРМА"; if (m_Target->m_Test.m_Res.inrectN==0) s="брак";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	lv.iSubItem = 4;
	s=" ";
	if (this->m_Target->m_Test.m_RejectPointIdx>=0)  s="Одна оторвавшаяся пробоина";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);


	}

//Пробоин в круге
	
	if (m_Target->m_Test.m_NGinCircle)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"В круге d=%d",this->m_Target->m_Test.m_NCircleD);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%d",m_Target->m_Test.m_GinCircle);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	s.Format(L"%d",m_Target->m_Test.m_NGinCircle);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);


	lv.iSubItem = 3;
	s=L"НОРМА"; if (m_Target->m_Test.m_Res.incircleN==0) s="брак";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	lv.iSubItem = 4;
	s=" ";
	if (this->m_Target->m_Test.m_RejectPointIdx>=0)  s="Одна оторвавшаяся пробоина";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);
	}


//Срединная полоса рассеивания по боковому направлению
	if (m_Target->m_Test.m_NBandW)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"Боковая полоса Сбср");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",(double)this->m_Target->m_Test.m_BandW/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	s.Format(L"%.1f",(double)this->m_Target->m_Test.m_NBandW*10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 3;
	s=L"НОРМА"; if (m_Target->m_Test.m_Res.bandwN==0) s="брак";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}


//Срединная полоса рассеивания по вертикали
	if (m_Target->m_Test.m_NBandH)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"Вертикальная полоса Свср");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",(double)this->m_Target->m_Test.m_BandH/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	s.Format(L"%.1f",(double)this->m_Target->m_Test.m_NBandH*10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 3;
	s=L"НОРМА"; if (m_Target->m_Test.m_Res.bandhN==0) s="брак";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}

//Срединная полоса эквивалентная
	if (m_Target->m_Test.m_NBandE)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"Эквиваленная полоса Свср");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",(double)this->m_Target->m_Test.m_BandE/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	s.Format(L"%.1f",(double)this->m_Target->m_Test.m_NBandE*10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 3;
	s=L"НОРМА"; if (m_Target->m_Test.m_Res.bandeN==0) s="брак";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}

//Срединная полоса эквивалентная
	if (m_Target->m_Test.m_N4836)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"Вхождение в габарит 48х36");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"48x36");
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	s=L"Да"; if (this->m_Target->m_Test.m_4836==0) s=L"Нет";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 3;
	s=L"НОРМА"; if (m_Target->m_Test.m_Res.m3846N==0) s="брак";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Итог

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L" ");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"итог:",this->m_Target->m_Test.m_NCircleD);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L" ");
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 2;
	s.Format(L" ");
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);


	lv.iSubItem = 3;
	s=L"НОРМА"; if (m_Target->m_Test.m_Res.resN==0) s="брак";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);


	return 0;

}
