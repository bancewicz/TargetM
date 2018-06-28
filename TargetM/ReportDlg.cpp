// ReportDlg.cpp: ���� ����������
//

#include "stdafx.h"
#include "TargetM.h"
#include "ReportDlg.h"
#include "afxdialogex.h"

//#include "TargetM.h"

// ���������� ���� CReportDlg

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


// ����������� ��������� CReportDlg


void CReportDlg::OnBnClickedButtonPrint()
{
	// TODO: �������� ���� ��� ����������� �����������


	
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
	//					PrintDC.DrawText("test text",-1,RECT|DT_VCENTER) ;   //�������� � �����������  
						//PrintDC.DrawText(smystring,-1,DT_VCENTER) ;   //�������� � �����������  

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

	// TODO:  �������� �������������� �������������



	this->m_REPORT_ListCtrl.InsertColumn(0,L"��������"	  ,LVCFMT_LEFT,80);
	this->m_REPORT_ListCtrl.InsertColumn(1,L"������� �������"	  ,LVCFMT_LEFT,134);
	this->m_REPORT_ListCtrl.InsertColumn(2,L"�������"	  ,LVCFMT_LEFT,70);
	this->m_REPORT_ListCtrl.InsertColumn(3,L"X (��)"	  ,LVCFMT_LEFT,120);
	this->m_REPORT_ListCtrl.InsertColumn(4,L"Y (��)"	  ,LVCFMT_LEFT,120);
	this->m_REPORT_ListCtrl.InsertColumn(5,L"�������� � ����(�������.)"	  ,LVCFMT_LEFT,230);

	this->m_REPORT_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	this->m_TESTRES_ListCtrl.InsertColumn(0,L"��������"	  ,LVCFMT_LEFT,230);
	this->m_TESTRES_ListCtrl.InsertColumn(1,L"��������"	  ,LVCFMT_LEFT,90);
	this->m_TESTRES_ListCtrl.InsertColumn(2,L"��������"	  ,LVCFMT_LEFT,90);
	this->m_TESTRES_ListCtrl.InsertColumn(3,L"���������"  ,LVCFMT_LEFT,120);
	this->m_TESTRES_ListCtrl.InsertColumn(4,L"����������" ,LVCFMT_LEFT,220);

	this->m_TESTRES_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |LVS_EX_BORDERSELECT);

	SetTimer(1,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
}


void CReportDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �������� ���� ��� ����������� ��������� ��� ����� ������������


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
	// TODO: �������� ���� ��� ����������� �����������
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
	m_TESTRES_ListBox.AddString(L"���������� ���������");
	m_TESTRES_ListBox.AddString(this->m_Target->m_Test.m_Mark);

	CListCtrl* list=&this->m_TESTRES_ListCtrl;

	list->DeleteAllItems();

	int item=0;

//����� �������
	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"����� �������");
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
//	if (m_Target->m_Test.m_RejectPointIdx>0) wsprintf(ss,L"���� ������������ ��������");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

//---------------------------------------------------------------------------------------------------

//���

	if (m_Target->m_Test.m_NTpointR)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"��� X");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",((double)m_Target->m_Test.m_STP.x)/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

//���
	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"��� Y");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",((double)m_Target->m_Test.m_STP.y)/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);


//���������� ��� �� ��

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"�� X");
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
	wsprintf(ss,L"�� Y");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iSubItem = 1;
	s.Format(L"%.1f",((double)m_Target->m_Test.m_TPoint.y)/10.);
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);


	//���������� ���
		lv.iItem = item++;

		lv.iSubItem = 0;
		wsprintf(ss,L"���������� ���");
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
		s.Format(L"�����"); if (m_Target->m_Test.m_Res.stpdevN==0) s.Format(L"����");
		wsprintf(ss,L"%s",s);
		lv.pszText =(LPWSTR)ss;
		lv.mask = LVIF_TEXT;
		list->SetItem(&lv);
	}

//������� � ��������������
	
	if (m_Target->m_Test.m_NGinRect)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"� �������������� %dx%d",this->m_Target->m_Test.m_Rect.h,this->m_Target->m_Test.m_Rect.w);
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
	s=L"�����"; if (m_Target->m_Test.m_Res.inrectN==0) s="����";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	lv.iSubItem = 4;
	s=" ";
	if (this->m_Target->m_Test.m_RejectPointIdx>=0)  s="���� ������������ ��������";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);


	}

//������� � �����
	
	if (m_Target->m_Test.m_NGinCircle)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"� ����� d=%d",this->m_Target->m_Test.m_NCircleD);
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
	s=L"�����"; if (m_Target->m_Test.m_Res.incircleN==0) s="����";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	lv.iSubItem = 4;
	s=" ";
	if (this->m_Target->m_Test.m_RejectPointIdx>=0)  s="���� ������������ ��������";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);
	}


//��������� ������ ����������� �� �������� �����������
	if (m_Target->m_Test.m_NBandW)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"������� ������ ����");
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
	s=L"�����"; if (m_Target->m_Test.m_Res.bandwN==0) s="����";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}


//��������� ������ ����������� �� ���������
	if (m_Target->m_Test.m_NBandH)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"������������ ������ ����");
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
	s=L"�����"; if (m_Target->m_Test.m_Res.bandhN==0) s="����";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}

//��������� ������ �������������
	if (m_Target->m_Test.m_NBandE)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"������������ ������ ����");
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
	s=L"�����"; if (m_Target->m_Test.m_Res.bandeN==0) s="����";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}

//��������� ������ �������������
	if (m_Target->m_Test.m_N4836)
	{

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"��������� � ������� 48�36");
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
	s=L"��"; if (this->m_Target->m_Test.m_4836==0) s=L"���";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->SetItem(&lv);

	lv.iSubItem = 3;
	s=L"�����"; if (m_Target->m_Test.m_Res.m3846N==0) s="����";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);

	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L" ");
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);

	lv.iItem = item++;

	lv.iSubItem = 0;
	wsprintf(ss,L"����:",this->m_Target->m_Test.m_NCircleD);
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
	s=L"�����"; if (m_Target->m_Test.m_Res.resN==0) s="����";
	wsprintf(ss,L"%s",s);
	lv.pszText =(LPWSTR)ss;
	lv.mask = LVIF_TEXT;
	list->InsertItem(&lv);
	list->SetItem(&lv);


	return 0;

}
