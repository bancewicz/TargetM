
// TargetMDlg.cpp : файл реализации
//int m_TARGSCALEFACTOR1/0

#include "stdafx.h"
#include "TargetM.h"
#include "TargetMDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "TestParDlg.h"

#include "ReportDlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define EMULATEMODE
#define WAIT_CMD_RESPOND_TIMEOUT 5000

#define SETGRAY(X)		{X.SetShade(SHS_DIAGSHADE,8,50,5,RGB(75,255,255));X.RedrawWindow();} 
#define SETYELLOW(X)	{X.SetShade(3,8,40,90,RGB(255,255,5));X.RedrawWindow();} 
#define SETGREEN(X)		{X.SetShade(3,8,40,90,RGB(5,255,5));X.RedrawWindow();} 


//Список доступных СОМ-портов
BYTE clist[512];

int GLOBALSEEK = 0;


//Режим работы
//0 - ручной
//1 - автомат
int MODE=1;

//Режим чтения
//0 - по таймеру
//1 - по таймауту
int READMODE=0;


//Флаг поиска мишени
BOOL RESPONDFLAG =	FALSE;

//Флаг поиска мишени
BOOL TARGETSEEK	=	FALSE;

//Флаг включения мишени
BOOL TARGON		=	FALSE;

//Флаг пвторного открытия СОМ-порта
BOOL PORTREOPENFLAG=FALSE;


CTargetM* TARGET=NULL;

int NUMREPORT=1;

CString REPORT[100];

CString OPERATOR("ТЕСТ");

CString OPERATORS[]={

	L"",L"",L"",L"",L"",L"",L"",L"",L"",L""

};


int TARGETSCALE[]={

	50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000

};


BOOL READPROCFLAG=FALSE;

CString m_VERSION(" v.1.0.21");

BOOL bgraph=FALSE;

BOOL LISTENFLAG=FALSE;

BYTE RXBUF[1024];
BYTE DEVRXBUF[8448];

DWORD dwBytesRead=0;
DWORD g_dwBytesRead=0;
uintptr_t ListenComThreadPtr=NULL;

HANDLE COMLISTENTHREAD=NULL;

CTargetMDlg* MAINDLG;

LPG_FUNCTIONSTRUCT m_lpfs = NULL;

BOOL gbFIREFLAG=FALSE;

DWORD COMPORTNUM=0;

int PACKTOUT=100;
int READPERD=1000;

int READERRLIMIT=10;

int READERRORCNT=0;

int CMDSTATUS=0;

CString CMDCONSOLE("");
CString RESPONDCONSOLE("");


void RXProc(void)
{

//	SetTimer(1,10,NULL);
//	return;

/////////////////////////////////////////////////////////////////////////////

//Забираем в свой буфер
	memcpy(&DEVRXBUF[g_dwBytesRead],RXBUF,dwBytesRead);
//Двигаем счетчик прочитанных байт
	g_dwBytesRead+=dwBytesRead;
	dwBytesRead=0;

	MAINDLG->SetTimer(2,PACKTOUT,NULL);
}

//void SerialListenThread (void* param) 
DWORD  WINAPI SerialListenThread (void* param) 
{
	
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);		

//	CSerial* serial = (CSerial*)param;


////////////////////////////////ИНИЦИАЛИЗАЦИЯ СИСТЕМНОГОТАЙМЕРА/////////////////////////////////////////////////////////////////////////////////////


	//check current system timer resolution
	ULONG ulMinRes = 0;
    ULONG ulMaxRes = 0;
    ULONG ulCurRes = 0;
	HMODULE NTDLLModuleHandle=NULL;
	NTDLLModuleHandle = LoadLibrary(L"NTDLL.DLL");

	if (NTDLLModuleHandle!=NULL)
	{
		typedef int (WINAPI *pNtQueryTimerResolution)(ULONG* ulMinRes, ULONG* ulMaxRes, ULONG* ulCurRes );
		pNtQueryTimerResolution GetSysRes;
		GetSysRes = (pNtQueryTimerResolution)::GetProcAddress(NTDLLModuleHandle,"NtQueryTimerResolution");
		int code = (*GetSysRes)(&ulMinRes, &ulMaxRes, &ulCurRes );

		//set current system timer resolution as 1ms
		ulCurRes=10000;
		typedef int (WINAPI *pNtSetTimerResolution)(ULONG ulMinRes, BOOL par, ULONG* ulCurRes );
		pNtSetTimerResolution SetSysRes;
		SetSysRes = (pNtSetTimerResolution)::GetProcAddress(NTDLLModuleHandle,"NtSetTimerResolution");
		SetSysRes(5000,TRUE,&ulCurRes);
//		SetSysRes(10000,TRUE,&ulCurRes);

	}
	else
	{
		AfxMessageBox(L"Fine Timer: Load NTDLL.DLL fail!");
	}



////////Создание таймера 1мс////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HANDLE hTimer = NULL;
    LARGE_INTEGER liDueTime;

	//liDueTime.QuadPart = (-10000000LL);  //1c

	liDueTime.QuadPart = (-10000000LL)/10000LL; //10ms

	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);

    if (NULL == hTimer)
    {
        AfxMessageBox(L"Cycle Shot: CreateWaitableTimer failed!");
        return 0u;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CSerialLight* serial = (CSerialLight*)param;

	serial->SetTimeouts(500,500);

	memset(RXBUF,0,1024);
	g_dwBytesRead=0;

	BYTE byte=0;
	serial->Purge();
	int tout=0;

	while (LISTENFLAG)
	{

//Ждем 1 мс


		if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
		{
			AfxMessageBox(L"Cycle Shot: SetWaitableTimer failed!");
			return 0u;
		}

		if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
		{
			AfxMessageBox(L"Cycle Shot: WaitForSingleObject failed!");
			return 0u;
		}	

/*
		if (READPROCFLAG)
		{
			//Sleep(10);
			//READPROCFLAG=FALSE;
			continue;
		}
*/


		if (serial->BytesAvilable())
		{
			while(serial->BytesAvilable())
			{			
				if ((serial->PortNRead(1,&byte))==FALSE) continue;
				RXBUF[g_dwBytesRead]=byte;
				g_dwBytesRead++;

				if (gbFIREFLAG)
				{
					if (g_dwBytesRead==15)	MAINDLG->SetTimer(2,PACKTOUT,NULL);
					else					MAINDLG->SetTimer(2,20,NULL);
				}
				else
				{
					MAINDLG->SetTimer(2,PACKTOUT,NULL);
				}

				//tout=0;
			}
		}
		else
		{
			//if (g_dwBytesRead) tout++;
			//if (tout>=100) MAINDLG->SetTimer(2,1,NULL);
		}

		continue;

		////////////////////////////////////////////////////////////////////////////////


		if (serial->SetTimeouts(5,1)==FALSE)
		{
		//	Sleep(100);
		}
		else
		{

			if ((serial->PortNRead(1,&byte))==FALSE)
			{
				if (g_dwBytesRead>0) tout++;
				if (tout>=10) 
				{
					tout=0;
					MAINDLG->SetTimer(2,1,NULL);
				}
			}
			else
			{
				RXBUF[g_dwBytesRead]=byte;
				g_dwBytesRead++;
				tout=0;
			}

		}

	}

/*
//	lLastError = serial->SetMask(~(CSerial::EEventBreak)|CSerial::EEventRecv|CSerial::EEventRcvEv);	
	
	lLastError = serial->SetMask(CSerial::EEventRecv|CSerial::EEventRcvEv);	

	serial->SetupHandshaking(CSerial::EHandshakeOff);
	serial->Purge();
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);		

	serial->SetEventChar(0);

//	serial->SetupReadTimeouts(10);

	while (LISTENFLAG)
	{

		if (serial->IsOpen()==FALSE)
		{
			Sleep(1000);
				if (serial->Open(L"\\\\.\\COM10",10000,10000)!=ERROR_SUCCESS)
				{
					Sleep(1000);
					continue;
				}

				Sleep(1000);
			}



		Sleep(1);
		serial->WaitEvent();
		Sleep(1);

    	//if (LISTENFLAG==FALSE)	break;

		const CSerial::EEvent eEvent = serial->GetEventType();	

		//if ((eEvent&EV_RXCHAR)||(eEvent&EV_RXFLAG))
		if (eEvent&EV_RXCHAR)
		//if (eEvent&EV_RXFLAG)

		//if (eEvent)
		{
				//serial->Read(&RXBUF[g_dwBytesRead],1024-g_dwBytesRead,&dwBytesRead);

				serial->Read(RXBUF,1024,&dwBytesRead);	

				//g_dwBytesRead+=dwBytesRead;
//				memset(&RXBUF[g_dwBytesRead],0,1024-g_dwBytesRead);
				RXProc();
				serial->Purge();
				//Sleep(1);
		}
		
	}
*/

	return 0u;
}



// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// диалоговое окно CTargetMDlg




IMPLEMENT_DYNAMIC(CTargetMDlg, CDialogEx);

CTargetMDlg::CTargetMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTargetMDlg::IDD, pParent)
	, m_AUTOINC_Bool(FALSE)
	, m_CONSOLER_Str(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CTargetMDlg::~CTargetMDlg()
{
	// Если для данного диалогового окна используется прокси-сервер автоматизации, установите
	//  для его обратного указателя на это диалоговое окно значение NULL, чтобы он узнал об
	//  удалении диалогового окна.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CTargetMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_COM_OPEN, m_COMOPEN_Btn);
	DDX_Control(pDX, IDC_COMBO1, m_COM_Combo);
	DDX_Control(pDX, IDOK, m_OK_Btn);
	DDX_Control(pDX, IDC_BUTTON_TARGET_REQUEST, m_TARGREQUEST_Btn);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_RESET_Btn);
	DDX_Control(pDX, IDC_COMBO_ADDR, m_ADDR_Combo);
	DDX_Control(pDX, IDC_EDIT_OPERATOR, m_OPERATOR_Edit);
	DDX_Control(pDX, IDC_EDIT_STATE, m_STATE_Edit);
	DDX_Control(pDX, IDC_EDIT_TICKS, m_TICKS_Edit);
	DDX_Control(pDX, IDC_EDIT_AREA, m_AREA_Edit);
	DDX_Control(pDX, IDC_EDIT_X, m_X_Edit);
	DDX_Control(pDX, IDC_EDIT_Y, m_Y_Edit);
	DDX_Control(pDX, IDC_EDIT_INFO, m_INFO_Edit);
	DDX_Control(pDX, IDC_EDIT_GAP, m_GAP_Edit);
	DDX_Control(pDX, IDC_COMBO_GAP, m_GAP_Combo);
	DDX_Control(pDX, IDC_BUTTON_SEEK, m_SEEK_Btn);
	DDX_Control(pDX, IDC_BUTTON_ON, m_ON_Btn);
	DDX_Control(pDX, IDC_BUTTON_OFF, m_OFF_Btn);
	DDX_Control(pDX, IDC_EDIT_CONSLOE_CMD, m_CONSOLEC_Edit);
	DDX_Control(pDX, IDC_EDIT_CONSOLE_RESPOND2, m_CONSOLER_Edit);
	DDX_Check(pDX, IDC_CHECK_AUTOINC, m_AUTOINC_Bool);

	DDX_Control(pDX, IDC_SLIDER_RANG, m_RANG_Slider);
	DDX_Control(pDX, IDC_BUTTON_FIRE, m_FIRE_Btn);
	DDX_Control(pDX, IDC_CHECK_AUTOINC, m_AUTO_Check);
	DDX_Control(pDX, IDC_EDIT_DISPLAY, m_DISPLAY_Edit);
	DDX_Control(pDX, IDC_BUTTON_PROTOCOL, m_PROTOCOL_Btn);
	DDX_Control(pDX, IDC_COMBO_USER, m_USER_Combo);
	DDX_Control(pDX, IDC_STATIC_BANNER, m_BANNER_static);
	DDX_Control(pDX, IDC_STATIC_BANNER_FRAME, m_BANNER_FRAME_Static);
	DDX_Control(pDX, IDC_STATIC_COMPORT, m_COMPORT_Static);
	DDX_Control(pDX, IDC_STATIC_ADDR, m_ADDR_Static);

	//DDX_Text(pDX, IDC_EDIT_CONSOLE_RESPOND, m_CONSOLER_Str);
	DDX_Control(pDX, IDC_COMBO_TESTNUM, m_TESTNUM_Combo);
	DDX_Control(pDX, IDC_EDIT_TESTPAR, m_TESTPAR_Edit);
	DDX_Control(pDX, IDC_BUTTON_TESTPAR, m_TESTPAR_Btn);
}

BEGIN_MESSAGE_MAP(CTargetMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TARGET_REQUEST, &CTargetMDlg::OnBnClickedButtonTargetRequest)
	ON_BN_CLICKED(IDC_BUTTON_COM_OPEN, &CTargetMDlg::OnBnClickedButtonComOpen)
	ON_BN_CLICKED(IDC_BUTTON_TEST1, &CTargetMDlg::OnBnClickedButtonTest1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TEST2, &CTargetMDlg::OnBnClickedButtonTest2)
	ON_BN_CLICKED(IDC_BUTTON_TEST3, &CTargetMDlg::OnBnClickedButtonTest3)
	ON_BN_CLICKED(IDC_BUTTON_TEST4, &CTargetMDlg::OnBnClickedButtonTest4)
	ON_BN_CLICKED(IDC_BUTTON_TEST5, &CTargetMDlg::OnBnClickedButtonTest5)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CTargetMDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDCANCEL, &CTargetMDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEEK, &CTargetMDlg::OnBnClickedButtonSeek)
	ON_BN_CLICKED(IDC_BUTTON_ON, &CTargetMDlg::OnBnClickedButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_OFF, &CTargetMDlg::OnBnClickedButtonOff)
	ON_BN_CLICKED(IDOK, &CTargetMDlg::OnBnClickedOk)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDER_RANG, &CTargetMDlg::OnNMThemeChangedSliderRang)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RANG, &CTargetMDlg::OnNMReleasedcaptureSliderRang)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_RANG, &CTargetMDlg::OnTRBNThumbPosChangingSliderRang)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RANG, &CTargetMDlg::OnNMCustomdrawSliderRang)
	ON_BN_CLICKED(IDC_BUTTON_FIRE, &CTargetMDlg::OnBnClickedButtonFire)
	ON_EN_CHANGE(IDC_EDIT_Y, &CTargetMDlg::OnEnChangeEditY)
	ON_BN_CLICKED(IDC_CHECK_AUTOINC, &CTargetMDlg::OnBnClickedCheckAutoinc)
	ON_CBN_EDITCHANGE(IDC_COMBO_GAP, &CTargetMDlg::OnCbnEditchangeComboGap)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RANG, &CTargetMDlg::OnNMCustomdrawSliderRang)
	ON_CBN_SELCHANGE(IDC_COMBO_GAP, &CTargetMDlg::OnCbnSelchangeComboGap)
	ON_WM_DESTROY()
	ON_CBN_KILLFOCUS(IDC_COMBO_USER, &CTargetMDlg::OnCbnKillfocusComboUser)
	ON_BN_CLICKED(IDC_BUTTON_PROTOCOL, &CTargetMDlg::OnBnClickedButtonProtocol)
	ON_EN_SETFOCUS(IDC_EDIT_STATE, &CTargetMDlg::OnEnSetfocusEditState)
	ON_CBN_EDITCHANGE(IDC_COMBO_TESTNUM, &CTargetMDlg::OnCbnEditchangeComboTestnum)
	ON_CBN_SELCHANGE(IDC_COMBO_TESTNUM, &CTargetMDlg::OnCbnSelchangeComboTestnum)
	ON_BN_CLICKED(IDC_BUTTON_TESTPAR, &CTargetMDlg::OnBnClickedButtonTestpar)
END_MESSAGE_MAP()


// обработчики сообщений CTargetMDlg

BOOL CTargetMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	///////////////////////////////////////TRACE////////////////////////////////////////////////////
	m_Ini.Init(L"TargetM");
	this->m_Target.m_Test.m_Ini=&this->m_Ini;
	this->m_Target.m_Test.LoadIniPar(1);	

	this->m_Target.m_Test.m_DisplayPar=&m_TESTPAR_Edit;
	this->m_Target.m_Test.m_DisplayRes=&m_DISPLAY_Edit;
	this->m_Target.m_Test.m_PULL=&this->m_Target.m_PULL;

//Флаг поиска мишениthis->m_Target.m_Test.LoadIniPar(1);	
m_Target.TARGETSEEK=&TARGETSEEK;

//Флаг включения мишени
m_Target.TARGON=&TARGON;


#ifdef EMULATEMODE

	SetWindowText(L"TargetM [Режим эмуляции]");
#else
//	CString title;
//	if (MODE==0)	title.Format(L"TargetM [%s]",m_VERSION);
//	else			title.Format(L"TargetM [%s] [COM-порт=%d АДРЕС=%в]",m_VERSION, COMPORTNUM, m_Target.m_ADDR);
//	SetWindowText(title);
#endif


	//CEdit* timeticks,CEdit* resx,CEdit* resy,CEdit* tarea,CMyEdit* tstatus, CSerial*  serial);

	CFont font;

	CFont *myFont = new CFont();
	myFont->CreateFont( 24, 0, 0, 0, FW_HEAVY, false, false,
        0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH|FF_MODERN, _T("Arial") );


	this->m_STATE_Edit.SetFont(myFont);
	this->m_X_Edit.SetFont(myFont);
	this->m_Y_Edit.SetFont(myFont);

	this->m_FIRE_Btn.SetFont(myFont);

	//this->m_SEEK_Btn.SetFont(myFont);
	//this->m_ON_Btn.SetFont(myFont);
    //this->m_OFF_Btn.SetFont(myFont);

	m_Target.m_Operator=&OPERATOR;

	CFont *myFont1 = new CFont();
	myFont1->CreateFont( 17, 0, 0, 0, FW_HEAVY, false, false,
        0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH|FF_MODERN, _T("Arial") );

	this->m_BANNER_static.SetFont(myFont1);

	m_Target.Trace(&this->m_TICKS_Edit,
					&this->m_X_Edit,
					&this->m_Y_Edit,
					&this->m_AREA_Edit,
					&this->m_STATE_Edit,
					&this->m_Com);

	this->m_DISPLAY_Edit.SetFont(myFont1);


	CFont *myFont2 = new CFont();
	myFont2->CreateFont( 16, 0, 0, 0, FW_HEAVY, false, false,
        0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH|FF_MODERN, _T("Courier") );

	this->m_CONSOLEC_Edit.SetFont(myFont2);
	this->m_CONSOLER_Edit.SetFont(myFont2);

	m_Target.m_MAINWND=this->GetSafeHwnd();

	m_Target.m_INFO_Edit=&m_INFO_Edit;
	m_Target.m_GAP_Edit=&m_GAP_Edit;
	m_Target.m_CONSOLEC_Edit=&m_CONSOLEC_Edit;
	m_Target.m_CONSOLER_Edit=&m_CONSOLER_Edit;
	m_Target.m_DISPLAY_Edit=&m_DISPLAY_Edit;
	m_Target.m_TESTPAR_Edit=&m_TESTPAR_Edit;

	MAINDLG=this;

	SETGRAY(m_TESTPAR_Btn);
	SETGRAY(m_COMOPEN_Btn);
	SETGRAY(m_OK_Btn);
	SETGRAY(m_RESET_Btn);
	SETGRAY(m_TARGREQUEST_Btn);

	SETGRAY(m_SEEK_Btn);
	SETGRAY(m_ON_Btn);
	SETGRAY(m_OFF_Btn);
	SETGRAY(m_FIRE_Btn);
	SETGRAY(m_PROTOCOL_Btn);

	this->m_PROTOCOL_Btn.EnableWindow(FALSE);
	this->m_TARGREQUEST_Btn.EnableWindow(FALSE);
    this->m_SEEK_Btn.EnableWindow(FALSE);
	this->m_ON_Btn.EnableWindow(FALSE);
	this->m_OFF_Btn.EnableWindow(FALSE);
	this->m_RESET_Btn.EnableWindow(FALSE);
	this->m_FIRE_Btn.EnableWindow(FALSE);

	this->m_DISPLAY_Edit.SetColorBG(RGB(0,0,0));
	this->m_DISPLAY_Edit.SetColorText(RGB(0,255,0));

	//m_RANG_Slider.SetRange(1,20);

	m_RANG_Slider.SetRange(1,14);

	//m_RANG_Slider.SetTic(10);
	m_RANG_Slider.SetPos(14);


	m_OPERATOR_Edit.SetWindowText(L"ТЕСТ");
	CString s("");


	for (int i=1; i<=20; i++)
	{
		s.Format(L"%d",i);
		m_TESTNUM_Combo.AddString(s);
	}


	m_TESTNUM_Combo.SetWindowText(L"1");
	m_Target.SetTestNгmber(1);

	for (int i=1; i<=255; i++)
	{
			s.Format(L"%d",i);
			m_ADDR_Combo.AddString(s);
	}

	for (int i=1; i<=60; i++)
	{
			s.Format(L"%d",i);
			m_GAP_Combo.AddString(s);
	}
		
			m_GAP_Combo.SetCurSel(0);
			m_ADDR_Combo.SetCurSel(0);

			m_Target.StatusView(TargetM_state::TSTATE_unknown);

	TARGET=&m_Target;
	m_USER_Combo.SetWindowText(L"Тест");

    SetTimer(20,100,NULL);

	ComPortScan();
	SetTargetPar();
	InitGraph();
	LoadIniPar();

	//this->m_CONSOLEC_Edit.ShowWindow(SW_SHOW);
	//this->m_CONSOLER_Edit.ShowWindow(SW_SHOW);

	SetTimer(30,10,NULL);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTargetMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTargetMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	if (bgraph)
	{
		m_pGraph->PaintGraph();
		m_Target.DrawPull();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTargetMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Серверы автоматизации не должны завершать работу при закрытии интерфейса пользователя,
//  если контроллеры по-прежнему удерживают один из его объектов. Эти
//  обработчики сообщений гарантируют, что если прокси-сервер еще используется,
//  то пользовательский интерфейс скрывается, но диалоговое окно остается (если оно
//  закрыто).

void CTargetMDlg::OnClose()
{
	//if (m_Com.IsOpen()) OnBnClickedButtonComOpen();

	if (CanExit())
		CDialogEx::OnClose();
}

void CTargetMDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CTargetMDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CTargetMDlg::CanExit()
{
	// Если объект прокси-сервера по-прежнему запущен, то контроллер
	//  автоматизации удерживает это приложение. Оставьте
	//  диалоговое окно запущенным, но скройте его пользовательский интерфейс.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CTargetMDlg::OnBnClickedButtonTargetRequest()
{
	// TODO: добавьте свой код обработчика уведомлений

	SetTargetPar();

		CString s;
		CString& ss=s;

		UpdateData(TRUE);

		//m_GAP_Combo.GetWindowTextW(ss);
		//m_Target.m_numGAP=StrToInt(s);

/*
	if (m_AUTOINC_Bool)
	{

		if (m_Target.m_numGAP<60)
		{
			int n=m_Target.m_numGAP+1;
			s.Format(L"%d",n);
			m_GAP_Combo.SetWindowTextW(s);
		}

	}
*/

			s.Format(L"%d",m_Target.m_numGAP);
			m_GAP_Combo.SetWindowTextW(s);


#ifdef EMULATEMODE

	BYTE buf[1024];
	int len=0;
	len=m_Target.RESPOND_emul(buf,TargetM_cmd::TCMD_READ);	
	m_Com.Write(buf,len);

#else

	//m_Target.CMD_create(TargetM_cmd::TCMD_READ);	
	//SetTimer(3,WAIT_CMD_RESPOND_TIMEOUT,NULL);

	SendTargetCmd(TargetM_cmd::TCMD_READ);

	if (m_Target.m_numGAP>=60)
	{
		m_Target.m_numGAP=1;
		m_GAP_Combo.SetWindowTextW(L"1");
		AfxMessageBox(L"Упражнеие закончено!");
	}

#endif
	

}


void CTargetMDlg::OnBnClickedButtonComOpen()
{
	// TODO: добавьте свой код обработчика уведомлений
	CString ss("");
	CString& s=ss;


	//if (m_Com.IsOpen()==FALSE)
	if (m_Com.st.on==FALSE)
	{


		if (COMPORTNUM==0)
		{
			this->m_COM_Combo.GetWindowText(s);


			if (StrToInt(s)==0)
			{
				AfxMessageBox(L"COM-порт не выбран!");
				return;
			}
		}
		else
		{
			s.Format(L"%d",COMPORTNUM);
		}

		CString cname("");
		cname.Format(L"\\\\.\\COM%s",s);

//		if (m_Com.Open(cname,2048,2048)!=ERROR_SUCCESS)
//		if (m_Com.Open(cname,2048,2048,FALSE)!=ERROR_SUCCESS)
		if (m_Com.Open(TRUE,StrToInt(s))==FALSE)
		{
			AfxMessageBox(L"Не удается открыть выбранный СОМ-порт!");
			COMPORTNUM=0;
			return;
		}

		/*
		if (m_Com.Setup(CSerial::EBaud19200)!=ERROR_SUCCESS)
		{
			AfxMessageBox(L"Не удается открыть выбранный СОМ-порт!");
			return;
		}
		*/

		COMPORTNUM=StrToInt(s);

		m_COMOPEN_Btn.SetWindowText(L"Закрыть");
		this->m_COM_Combo.EnableWindow(FALSE);
		SETGREEN(m_COMOPEN_Btn);
		LISTENFLAG=TRUE;

		

		 DWORD   id     = 0u;
		//HANDLE  thread 

		if (READMODE==0) COMLISTENTHREAD = CreateThread(NULL, 256U, SerialListenThread, &m_Com, 0U, &id);


		this->m_TARGREQUEST_Btn.EnableWindow(TRUE);
		this->m_SEEK_Btn.EnableWindow(TRUE);
		this->m_ON_Btn.EnableWindow(TRUE);
		this->m_OFF_Btn.EnableWindow(TRUE);
		this->m_RESET_Btn.EnableWindow(TRUE);
		this->m_FIRE_Btn.EnableWindow(TRUE);
	}
	else
	{
		COMPORTNUM=0;
		LISTENFLAG=FALSE;
		Sleep(100);	

		CloseHandle((HANDLE)COMLISTENTHREAD);

		Sleep(100);

		m_Com.Close();

		m_COMOPEN_Btn.SetWindowText(L"Открыть");
		this->m_COM_Combo.EnableWindow(TRUE);
		SETGRAY(m_COMOPEN_Btn);

		this->m_TARGREQUEST_Btn.EnableWindow(FALSE);
		this->m_SEEK_Btn.EnableWindow(FALSE);
		this->m_ON_Btn.EnableWindow(FALSE);
		this->m_OFF_Btn.EnableWindow(FALSE);
		this->m_RESET_Btn.EnableWindow(FALSE);
		this->m_FIRE_Btn.EnableWindow(FALSE);

	}


}


void CTargetMDlg::ComPortScan(void)
{
//	BYTE clist[512];

	CString s("");
	m_Com.Scan(clist);
	
	this->m_COM_Combo.Clear();

	int c=m_COM_Combo.GetCount();

//	for (int i=0; i<=m_COM_Combo.GetCount(); i++) m_COM_Combo.DeleteString(i);

	for (int i=0; i<255; i++)
	{
		if (clist[i]==0) break;
		s.Format(L"%d",clist[i]);
		m_COM_Combo.AddString(s);
	}

	m_COM_Combo.SetCurSel(0);
	
}


void CTargetMDlg::SetState(void)
{
}


void CTargetMDlg::OnBnClickedButtonTest1()
{
}


//DWORD  PORTSCAN=0;
BYTE   ADRSCAN=0;
int	   portidx=-1;

CString pref=L"";
int ipref=0;


void CTargetMDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	if (nIDEvent==2)
	{

		//if (g_dwBytesRead!=15) return;

		//READPROCFLAG=TRUE;

		KillTimer(2);
		KillTimer(3);

		m_Com.Purge();

		int len=g_dwBytesRead;

		if (len!=15)
			Sleep(1);

		memcpy(DEVRXBUF,RXBUF,len);
		g_dwBytesRead=0;

		CString s(""),ss("");
		for (int i=0; i<len; i++)
		{
			s.Format(L"%02X ",DEVRXBUF[i]);
			ss+=s;
		}

		this->PutConsoleR(ss);

		//m_Target.RESPOND_proc(RXBUF,g_dwBytesRead);

		TargetM_res res;
		res=m_Target.RESPOND_proc(DEVRXBUF,len);


		//READPROCFLAG=FALSE;

		memset(RXBUF,0,1024);
		g_dwBytesRead=0;
		m_Com.Purge();
	}

	if (nIDEvent==3)
	{
		KillTimer(3);
		m_Target.StatusView(TargetM_state::TSTATE_noanswer);
	}


	if (nIDEvent==10)
	{

		if (ipref==0) pref=L"";
		if (ipref==1) pref=L".";
		if (ipref==2) pref=L"..";
		if (ipref==3) pref=L"...";
		
		ipref++; if (ipref>3) ipref=0;
	

		CString s;

		/*
		if (GLOBALSEEK==1)
		{

				//memset (clist,0,512);//!!!

				if (clist[0]==0)
				{
					s.Format(L"В СИСТЕМЕ НЕ ОБНАРУЖЕНЫ СОМ ПОРТЫ!");
					m_DISPLAY_Edit.SetWindowTextW(s);
					KillTimer(10);
					goto end_timer;
				}


			if (ADRSCAN==0)
			{
				ADRSCAN=1;
				portidx++;
				COMPORTNUM=clist[portidx];

				if (COMPORTNUM==0)
				{
					s.Format(L"ПОИСК ЗАКОНЧЕН!");
					m_DISPLAY_Edit.SetWindowTextW(s);
					KillTimer(10);
					goto end_timer;
				}
				else
				{
					m_Com.Close();
					if (m_Com.Open(TRUE,COMPORTNUM)==FALSE)
					{
						s.Format(L"ОТКРЫТИЕ СОМ%d ... НЕДОСТУПЕН!",COMPORTNUM);
						m_DISPLAY_Edit.SetWindowTextW(s);
						ADRSCAN=0;
						SetTimer(10,5000,NULL);
						goto end_timer;
					}
					else
					{
						SetTimer(10,1000,NULL);
						goto end_timer;
					}
				}

			}
			else
			{
				s.Format(L"ПОИСК СОМ=%d ADDR=%d ... ",COMPORTNUM,ADRSCAN);
				m_DISPLAY_Edit.SetWindowTextW(s);

				m_Target.m_ADDR=ADRSCAN-1;

				this->SendTargetCmd(TargetM_cmd::TCMD_SEEK);

				ADRSCAN++;
				SetTimer(10,100,NULL);
				goto end_timer;
			}

			goto end_timer;
		}
	    */ 


		if (MODE==0)
		{
			if (PORTREOPENFLAG==TRUE)
			{
				if (m_Com.Open(TRUE,COMPORTNUM)==TRUE)
				{
					m_DISPLAY_Edit.SetWindowTextW(L"Открытие СОМ-порта ...\r\nУспешно!");
					PORTREOPENFLAG=FALSE;
					SetTimer(10,READPERD,NULL);
				}
				else
				{
					SetTimer(10,5000,NULL);
				}
			}
		}
		
		if (MODE==1)
		{
			if (PORTREOPENFLAG==TRUE)
			{

				this->m_STATE_Edit.SetColorBG(RGB(150,150,150));
				this->m_STATE_Edit.SetWindowText(L"ОТКЛЮЧЕНО");

				s.Format(L"Открытие СОМ-порта %s",pref);

				m_DISPLAY_Edit.SetWindowTextW(s);

				if (m_Com.Open(TRUE,COMPORTNUM)==TRUE)
				{
					m_DISPLAY_Edit.SetWindowTextW(L"Открытие СОМ-порта ...\r\nУспешно!");
					PORTREOPENFLAG=FALSE;
					SetTimer(10,READPERD,NULL);
					goto end_timer;
				}
				else
				{
					SetTimer(10,5000,NULL);
					goto end_timer;
				}
			}
			else
			{
				if (TARGETSEEK==FALSE)
				{
					m_DISPLAY_Edit.SetWindowTextW(L"Поиск мишени ...");
					RESPONDFLAG=FALSE;
					this->SendTargetCmd(TargetM_cmd::TCMD_SEEK);
					SetTimer(1,READPERD,NULL);
					goto end_timer;
				}
				else
					{
						if (TARGON==FALSE)
						{
							m_DISPLAY_Edit.SetWindowTextW(L"Включение мишени ...");
							this->SendTargetCmd(TargetM_cmd::TCMD_ON);
							SetTimer(1,READPERD,NULL);
							goto end_timer;
						}
						else
						{
							m_DISPLAY_Edit.SetWindowTextW(L"Чтение данных ...");
							m_Target.m_Auto=TRUE;
							SetTimer(1,READPERD,NULL);
							//goto end_timer;
						}

					}

			}

		}



		//CString s("");

		s.Format(L"%d",m_Target.m_numGAP);
		m_GAP_Combo.SetWindowTextW(s);
	
		if (CMDSTATUS!=0)
		{
			READERRORCNT++;
			if (READERRORCNT>=READERRLIMIT)
			{
				KillTimer(10);
				AfxMessageBox(L"Достигнут лимит ошибок передачи!");
				goto stop_shot;
			}
		}
		else
		{
			READERRORCNT=0;
		}


		//SendTargetCmd(TargetM_cmd::TCMD_READ);
		
		//m_Target.m_numGAP++;



		//if (((m_Target.m_numGAP>=60)&&(CMDSTATUS==0))||(m_Target.m_STOP))
		//if (((m_Target.m_numGAP>=60)&&(m_Target.m_PULL.idx==59)&&(CMDSTATUS==0))||(m_Target.m_STOP))
		//if (((m_Target.m_numGAP>=m_Target.m_TESTPAR_gapsnum)&&(m_Target.m_PULL.idx==m_Target.m_TESTPAR_gapsnum-1)&&(CMDSTATUS==0))||(m_Target.m_STOP))
		if (((m_Target.m_numGAP>=m_Target.m_Test.m_NGaps)&&(m_Target.m_PULL.idx==m_Target.m_Test.m_NGaps-1)&&(CMDSTATUS==0))||(m_Target.m_STOP))
		{

stop_shot:
			gbFIREFLAG=FALSE;
			SETGRAY(m_FIRE_Btn);

			KillTimer(10);

			m_Target.m_numGAP=1;
			this->m_GAP_Combo.SetWindowTextW(L"1");
			m_AUTOINC_Bool=FALSE;
			UpdateData(FALSE);

			m_GAP_Combo.EnableWindow(TRUE);
			m_AUTO_Check.EnableWindow(TRUE);


			this->m_TARGREQUEST_Btn.EnableWindow(TRUE);
			this->m_SEEK_Btn.EnableWindow(TRUE);
			this->m_ON_Btn.EnableWindow(TRUE);
			this->m_OFF_Btn.EnableWindow(TRUE);
			this->m_RESET_Btn.EnableWindow(TRUE);
			this->m_TESTPAR_Btn.EnableWindow(TRUE);


			this->m_STATE_Edit.SetWindowText(L"ОСТАНОВ");
			this->m_DISPLAY_Edit.SetWindowText(L"ОСТАНОВ");

			AfxMessageBox(L"Стрельба закончена!");

			m_FIRE_Btn.SetWindowTextW(L"ПУСК");
		    this->m_PROTOCOL_Btn.EnableWindow(TRUE);

			this->m_ADDR_Combo.EnableWindow(TRUE);
			this->m_USER_Combo.EnableWindow(TRUE);
			this->m_RANG_Slider.EnableWindow(TRUE);

			CreateReport();

			NUMREPORT++;

		}
		else
		{
			if (SendTargetCmd(TargetM_cmd::TCMD_READ)==FALSE)
			{
				m_Com.Close();
				PORTREOPENFLAG=TRUE;

				//TARGETSEEK	=	FALSE;

				//TARGON		=	FALSE;

				m_DISPLAY_Edit.SetWindowTextW(L"Открытие СОМ-порта ...");
				SetTimer(10,5000,NULL);
			}
		}

	}


	if (nIDEvent==11)
	{
	

	}

	if (nIDEvent==20)
	{
		KillTimer(20);

		m_pGraph->m_DrawGaps=TRUE;

		//!!!SetTimer(1,1000,NULL);//!!!

		if ((MODE==1)&&(GLOBALSEEK==0))
		{
			CRect rect;
			//this->GetClientRect(&rect);
			this->GetWindowRect(&rect);
			this->MoveWindow(rect.left,rect.top,rect.Width()-400,rect.Height());
		}
		else
		{
			this->m_CONSOLER_Edit.ShowWindow(SW_SHOW);
			this->m_CONSOLEC_Edit.ShowWindow(SW_SHOW);
			this->m_RESET_Btn.ShowWindow(SW_SHOW);

		}

	}


	if (nIDEvent==30)
	{
		KillTimer(30);

		UpdateData(TRUE);

		int testnum=1;

		CString s;
		CString& ss=s;

		this->m_TESTNUM_Combo.GetWindowTextW(ss);

		testnum=StrToInt(s);

		this->m_Target.SetTestNгmber(testnum);

		this->m_Target.m_Test.LoadIniPar(testnum);

		CString title;
		if (MODE==0)
		{
			title.Format(L"TargetM [%s]",m_VERSION);
		}
		else
		{
			if (COMPORTNUM>0)   title.Format(L"TargetM [%s] [COM-порт=%d] [АДРЕС=%d]",m_VERSION, COMPORTNUM, m_Target.m_ADDR);
			else				title.Format(L"TargetM [%s] [COM-порт=не выбран] [АДРЕС=%d]",m_VERSION, m_Target.m_ADDR);

		}

		title+=L" [";
		title+=this->m_Target.m_Test.m_Mark;
		title+=L" ]";
		SetWindowText(title);

		m_Target.m_Test.DisplayTestPar();
	}



end_timer:

	CDialogEx::OnTimer(nIDEvent);
}


void CTargetMDlg::OnBnClickedButtonTest2()
{
	// TODO: добавьте свой код обработчика уведомлений
	
}


void CTargetMDlg::OnBnClickedButtonTest3()
{
	// TODO: добавьте свой код обработчика уведомлений

	BYTE frame[] = {

//		0x1F,	0x07,	0x9F,	0xAB,	0xAC,	0xFF,	0xD5

    0x1F,	0x0C,	0xA0,	0xFF,	0x00,	0xFF,	0xFF,	0xFF,	0x00,	0x00,	0x05,	0xE5

	};


	unsigned short crc16 = m_Target.CRC16(frame,10);




	Sleep(1);

}


void CTargetMDlg::OnBnClickedButtonTest4()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_Target.CMD_create(TargetM_cmd::TCMD_SEEK);
	m_Target.CMD_create(TargetM_cmd::TCMD_ON);
	m_Target.CMD_create(TargetM_cmd::TCMD_OFF);
	m_Target.CMD_create(TargetM_cmd::TCMD_READ);
}


int x,y=0;
int n=3;

void CTargetMDlg::OnBnClickedButtonTest5()
{
	// TODO: добавьте свой код обработчика уведомлений
	
	//m_pGraph->DrawGap(m_pGraph->target_LEFT_X()/2,m_pGraph->target_TOP_Y()/2,n); n++;

	//unsigned int total=0;

	//m_DISPLAY_Edit.SetWindowTextW(L"test");

	//SetTimer(11,100,NULL);

	//return;

	m_Target.DrawPull();
	return;

	m_pGraph->DrawGap(0,0,n); n++; Sleep(1000);
	m_pGraph->DrawGap(100,-100,n); n++; Sleep(1000);
	m_pGraph->DrawGap(100,100,n); n++; Sleep(1000);
	m_pGraph->DrawGap(-100,-100,n); n++; Sleep(1000);
	m_pGraph->DrawGap(-100,100,n); n++; Sleep(1000);


/*
	m_pGraph->DrawGap(0,0,n); n++; Sleep(1000);
	m_pGraph->DrawGap(m_pGraph->target_WIDE()/2,0,n); n++; Sleep(1000);
	m_pGraph->DrawGap(-m_pGraph->target_WIDE()/2,0,n); n++; Sleep(1000);
	m_pGraph->DrawGap(0,m_pGraph->target_HEIGHT()/2,n); n++; Sleep(1000);
	m_pGraph->DrawGap(0,-m_pGraph->target_HEIGHT()/2,0); n++; Sleep(1000);

	m_pGraph->DrawGap(m_pGraph->target_WIDE()/2,m_pGraph->target_HEIGHT()/2,n); n++; Sleep(1000);
	m_pGraph->DrawGap(-m_pGraph->target_WIDE()/2,m_pGraph->target_HEIGHT()/2,n); n++; Sleep(1000);
	m_pGraph->DrawGap(m_pGraph->target_WIDE()/2,-m_pGraph->target_HEIGHT()/2,n); n++; Sleep(1000);
	m_pGraph->DrawGap(-m_pGraph->target_WIDE()/2,-m_pGraph->target_HEIGHT()/2,n); n++; Sleep(1000);
*/

	//m_pGraph->DrawGap(0,0,n); n++;

	

}


void CTargetMDlg::OnBnClickedButtonReset()
{
	// TODO: добавьте свой код обработчика уведомлений
	this->PutConsoleC(L"");
	this->PutConsoleR(L"");


	/*
	m_Target.ClearForm();
	m_DISPLAY_Edit.SetWindowTextW(L"");
	m_Target.ClearPULL();
	m_pGraph->PaintGraph();
	*/
}


void CTargetMDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void CTargetMDlg::OnBnClickedButtonSeek()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetTargetPar();

#ifdef EMULATEMODE
	m_Target.m_Info="";
	BYTE buf[1024];
	int len=0;
	len=m_Target.RESPOND_emul(buf,TargetM_cmd::TCMD_SEEK);	
	m_Com.Write(buf,len);
#else
	//m_Target.CMD_create(TargetM_cmd::TCMD_SEEK);
	//SetTimer(3,WAIT_CMD_RESPOND_TIMEOUT,NULL);
	SendTargetCmd(TargetM_cmd::TCMD_SEEK);

#endif


	return;

}


void CTargetMDlg::SetTargetPar(void)
{
	m_Target.m_Info="";

	UpdateData(TRUE);
	CString s;
	CString& ss=s;
	m_ADDR_Combo.GetWindowTextW(s);
	m_Target.m_ADDR=StrToInt(s);

}


void CTargetMDlg::OnBnClickedButtonOn()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetTargetPar();

#ifdef EMULATEMODE
	m_Target.m_Info="";
	BYTE buf[1024];
	int len=0;
	len=m_Target.RESPOND_emul(buf,TargetM_cmd::TCMD_ON);	
	m_Com.Write(buf,len);

#else
	//m_Target.CMD_create(TargetM_cmd::TCMD_ON);
	//SetTimer(3,WAIT_CMD_RESPOND_TIMEOUT,NULL);

	m_Target.m_numGAP=1;
	SendTargetCmd(TargetM_cmd::TCMD_ON);

#endif



	return;

}


void CTargetMDlg::OnBnClickedButtonOff()
{
	// TODO: добавьте свой код обработчика уведомлений

	
	int NumDigits = 2;
	double x = 210.496;
	CString s;
	s.Format(_T("%010.1f"), x);
	AfxMessageBox(s);

	return;//!!!

/*
	SetTargetPar();

#ifdef EMULATEMODE
	m_Target.m_Info="";
	BYTE buf[1024];
	int len=0;
	len=m_Target.RESPOND_emul(buf,TargetM_cmd::TCMD_OFF);	
	m_Com.Write(buf,len);
#else

	//m_Target.CMD_create(TargetM_cmd::TCMD_OFF);
	//SetTimer(3,WAIT_CMD_RESPOND_TIMEOUT,NULL);

	SendTargetCmd(TargetM_cmd::TCMD_OFF);

#endif
	*/

	SetTargetPar();
	SendTargetCmd(TargetM_cmd::TCMD_OFF);


	return;

}


void CTargetMDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений

	//if (m_Com.IsOpen()) OnBnClickedButtonComOpen();
	if (m_Com.st.on) OnBnClickedButtonComOpen();

	CDialogEx::OnOK();
}


BOOL CTargetMDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: добавьте специализированный код или вызов базового класса


	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CTargetMDlg::SendTargetCmd(TargetM_cmd cmd)
{

	m_Com.Purge();

	int res=TRUE;

	CString srequest=L"КОМАНДА?";

	switch (cmd)
	{
		case TargetM_cmd::TCMD_NONE:
			srequest=L"НЕТ КОМАНДЫ?";
		break;

		case TargetM_cmd::TCMD_SEEK:
			srequest=L"ПОИСК?";
		break;

		case TargetM_cmd::TCMD_ON:
			srequest=L"ВКЛЮЧИТЬ?";
		break;

		case TargetM_cmd::TCMD_OFF:
			srequest=L"ВЫКЛЮЧИТЬ?";
		break;

		case TargetM_cmd::TCMD_READ:
			srequest=L"ЧТЕНИЕ?";
		break;

	}

	this->m_STATE_Edit.SetColorBG(RGB(150,150,150));
	this->m_STATE_Edit.SetWindowTextW(srequest);

	if (this->m_Target.CMD_create(cmd)==FALSE)
	{
		CString s;
		s.Format(L"СОМ=%d АДР=%d ошибка записи ... ",COMPORTNUM,m_Target.m_ADDR);
		m_DISPLAY_Edit.SetWindowText(s);
		res=FALSE;
	}
	else
	{

		CString s(""),ss("");

		for (int i=0; i<m_Target.m_txIDX; i++)
		{
			s.Format(L"%02X ",m_Target.m_bufTX[i]);
			ss+=s;
		}

		this->PutConsoleC(ss);

		if (READMODE==0) SetTimer(3,WAIT_CMD_RESPOND_TIMEOUT,NULL);
	}


	if (READMODE==1)
	{
		Sleep(250);

		if (m_Target.AskTargetRequest(cmd)==TRUE)
		{

			//if (m_Target.m_ADDR==1) 
		    RESPONDFLAG=TRUE;

			CString s(""),ss("");
			for (int i=0; i<m_Target.m_rxIDX; i++)
			{
				s.Format(L"%02X ",m_Target.m_bufRX[i]);
				ss+=s;
			}

		this->PutConsoleR(ss);
		}
	}

	return res;
}


void drawpull(void)
{
	TARGET->DrawPull();
}

void CTargetMDlg::InitGraph(void)
{
//	m_pGraph=new CGraph(this,360,80,450,450+13,G_TSMCHEME);

	//m_pGraph=new CGraph(this,360,80,450+13+13+13+13,450+13+13+13+13,G_TSMCHEME);

	m_pGraph=new CGraph(this,370,80,500,500,G_TSMCHEME);

	m_Target.m_pGraph=m_pGraph;
	m_pGraph->drawgaps=drawpull;
	m_pGraph->PaintGraph();
	bgraph=TRUE;

}


void CTargetMDlg::OnNMThemeChangedSliderRang(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Для этого средства требуется Windows XP или более поздняя версия.
	// Символ _WIN32_WINNT должен быть >= 0x0501.
	// TODO: добавьте свой код обработчика уведомлений

	Sleep(1);

	*pResult = 0;
}


void CTargetMDlg::OnNMReleasedcaptureSliderRang(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: добавьте свой код обработчика уведомлений

	*pResult = 0;
}


void CTargetMDlg::OnTRBNThumbPosChangingSliderRang(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Для этого средства требуется Windows Vista или более поздняя версия.
	// Символ _WIN32_WINNT должен быть >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	*pResult = 0;
}


//void CTargetMDlg::OnNMCustomdrawSliderRang(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: добавьте свой код обработчика уведомлений
//
//	int scale=this->m_RANG_Slider.GetPos();
//	m_pGraph->m_TARGETSCALE=TARGETSCALE[scale-1];
//	m_pGraph->PaintGraph();
//
//
//	*pResult = 0;
//}


void CTargetMDlg::OnBnClickedButtonFire()
{
	// TODO: добавьте свой код обработчика уведомлений

	//if (MODE==0)
	//{
	//	SendTargetCmd(TargetM_cmd::TCMD_READ);
	//	return;
	//}


	if (gbFIREFLAG==FALSE)
	{

		this->m_pGraph->mdlX=0;
		this->m_pGraph->mdlY=0;
		//this->m_Target.m_TSETRESULT_Str=L"";

		if (MODE==1)
		{
			TARGETSEEK=FALSE;
			TARGON=FALSE;
			PORTREOPENFLAG=TRUE;
			m_Com.Close();
		}


		OnBnClickedButtonReset();

		READERRORCNT=0;
		CMDSTATUS=0;

		m_Target.ClearPULL();

		gbFIREFLAG=TRUE;
		SETYELLOW(m_FIRE_Btn);
		m_Target.m_numGAP=1;
		this->m_GAP_Combo.SetWindowTextW(L"1");
		m_AUTOINC_Bool=FALSE;
		UpdateData(FALSE);

		m_GAP_Combo.EnableWindow(FALSE);
		m_AUTO_Check.EnableWindow(FALSE);

		this->m_TARGREQUEST_Btn.EnableWindow(FALSE);
		this->m_SEEK_Btn.EnableWindow(FALSE);
		this->m_ON_Btn.EnableWindow(FALSE);
		this->m_OFF_Btn.EnableWindow(FALSE);
		this->m_RESET_Btn.EnableWindow(FALSE);
		this->m_PROTOCOL_Btn.EnableWindow(FALSE);
		this->m_TESTPAR_Btn.EnableWindow(FALSE);


		m_Target.m_Auto=TRUE;
		m_Target.m_STOP=FALSE;

		SetTimer(10,READPERD,NULL);

		m_FIRE_Btn.SetWindowTextW(L"СТОП");

		this->m_ADDR_Combo.EnableWindow(FALSE);
		this->m_USER_Combo.EnableWindow(FALSE);
		this->m_RANG_Slider.EnableWindow(FALSE);

		m_DISPLAY_Edit.SetWindowTextW(L"ОПРОС ДАННЫХ");

		this->m_Target.m_TestOver=FALSE;

		m_Target.m_Test.ClearRes();
	}
	else
	{

		this->m_ADDR_Combo.EnableWindow(TRUE);
		this->m_USER_Combo.EnableWindow(TRUE);
		this->m_RANG_Slider.EnableWindow(TRUE);

		m_Target.m_Auto=FALSE;

		gbFIREFLAG=FALSE;
		SETGRAY(m_FIRE_Btn);

		KillTimer(10);


		m_Target.m_numGAP=1;
		this->m_GAP_Combo.SetWindowTextW(L"1");
		m_AUTOINC_Bool=FALSE;

		UpdateData(FALSE);

		m_GAP_Combo.EnableWindow(TRUE);
		m_AUTO_Check.EnableWindow(TRUE);


		this->m_TARGREQUEST_Btn.EnableWindow(TRUE);
		this->m_SEEK_Btn.EnableWindow(TRUE);
		this->m_ON_Btn.EnableWindow(TRUE);
		this->m_OFF_Btn.EnableWindow(TRUE);
		this->m_RESET_Btn.EnableWindow(TRUE);
		this->m_PROTOCOL_Btn.EnableWindow(TRUE);
		this->m_TESTPAR_Btn.EnableWindow(TRUE);


		this->m_STATE_Edit.SetWindowText(L"ОСТАНОВ");
		this->m_DISPLAY_Edit.SetWindowText(L"ОСТАНОВ");

		m_FIRE_Btn.SetWindowTextW(L"ПУСК");


		CreateReport();

		NUMREPORT++;

	}


}


void CTargetMDlg::OnEnChangeEditY()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}


void CTargetMDlg::OnBnClickedCheckAutoinc()
{
	// TODO: добавьте свой код обработчика уведомлений

	UpdateData(TRUE);

	m_Target.m_Auto=m_AUTOINC_Bool;

}


void CTargetMDlg::OnCbnEditchangeComboGap()
{
	// TODO: добавьте свой код обработчика уведомлений

		CString s;
		CString& ss=s;

		UpdateData(TRUE);

		m_GAP_Combo.GetWindowTextW(ss);
		m_Target.m_numGAP=StrToInt(s);

}


void CTargetMDlg::OnNMCustomdrawSliderRang(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	int scale=this->m_RANG_Slider.GetPos();
	m_pGraph->m_TARGETSCALE=TARGETSCALE[scale-1];
	m_pGraph->PaintGraph();
	*pResult = 0;
}


void CTargetMDlg::OnCbnSelchangeComboGap()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void CTargetMDlg::SaveIniPar(void)
{

	TCHAR tpar[1024];

	CString s;
	CString& ss=s;

	this->m_USER_Combo.GetWindowTextW(ss);
	m_Ini.SaveIniPar(L"COMMON",L"OPERATOR",STR_PARAMETER_FORMAT,&s);


	for (int i=0; i<10; i++)
	{
		//par.Format(L"OPERATOR%d",i);
		swprintf(tpar,L"%OPERATOR%d",i);
		m_Ini.SaveIniPar(L"OPERATORS",tpar,STR_PARAMETER_FORMAT,&OPERATORS[i]);

	}



	//if (MODE==1) return;


	this->m_COM_Combo.GetWindowTextW(ss);

//	if (m_Com.IsOpen()) COMPORTNUM=StrToInt(s);

	if (MODE==0)
	{
		if (m_Com.st.on)    COMPORTNUM=StrToInt(s);
		else				COMPORTNUM=0;
	}

	m_Ini.SaveIniPar(L"COMMON",L"COMPORT",INT_PARAMETER_FORMAT,&COMPORTNUM);



	this->m_ADDR_Combo.GetWindowTextW(ss);
	m_Ini.SaveIniPar(L"COMMON",L"ADDRESS",STR_PARAMETER_FORMAT,&s);

	m_Ini.SaveIniPar(L"PROTOCOL",L"PACKETPAUSE",INT_PARAMETER_FORMAT,&PACKTOUT);
	m_Ini.SaveIniPar(L"PROTOCOL",L"READPERIOD",INT_PARAMETER_FORMAT,&READPERD);
	m_Ini.SaveIniPar(L"PROTOCOL",L"READERRLIMIT",INT_PARAMETER_FORMAT,&READERRLIMIT);

/*
	m_Ini.SaveIniPar(L"PROTOCOL",L"MODE",INT_PARAMETER_FORMAT,&MODE);
	m_Ini.SaveIniPar(L"PROTOCOL",L"READMODE",INT_PARAMETER_FORMAT,&READMODE);
*/

	//CString spar;
	

	//m_Ini.SaveIniPar(L"METRICS",L"SCALEDEVIDER",INT_PARAMETER_FORMAT,&m_pGraph->m_TARGSCALEFACTOR);

	m_Ini.SaveIniPar(L"METRICS",L"EMULATEMODE",INT_PARAMETER_FORMAT,&m_Target.m_EMULATEMODE);

	m_Ini.SaveIniPar(L"METRICS",L"TARGETMETRIC",INT_PARAMETER_FORMAT,&m_pGraph->m_TARGETSCALE);	
	
}


void CTargetMDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	SaveIniPar();

	// TODO: добавьте свой код обработчика сообщений
}


void CTargetMDlg::LoadIniPar(void)
{
	m_Ini.Init(L"TargetM");

	int iport=0;
	CString s;


	if (m_Ini.GetIniPar(L"PROTOCOL",L"MODE",INT_PARAMETER_FORMAT,&iport)==0)
	{
		MODE=iport;
	}

	if (m_Ini.GetIniPar(L"PROTOCOL",L"READMODE",INT_PARAMETER_FORMAT,&iport)==0)
	{ 
		READMODE=iport;
	}

	if (m_Ini.GetIniPar(L"COMMON",L"COMPORT",INT_PARAMETER_FORMAT,&iport)==0)
	{
		COMPORTNUM=iport;

		if (MODE==0)
		{
			if (COMPORTNUM!=0)
			{
				OnBnClickedButtonComOpen();
				s.Format(L"%d",iport);
				m_COM_Combo.SetWindowTextW(s);
			}
		}
		else
		{
			PORTREOPENFLAG=TRUE;
			s.Format(L"%d",iport);
			m_COM_Combo.SetWindowTextW(s);
		}	
	}

	if (m_Ini.GetIniPar(L"COMMON",L"ADDRESS",INT_PARAMETER_FORMAT,&iport)==0)
	{
			s.Format(L"%d",iport);
			m_ADDR_Combo.SetWindowTextW(s);
			m_Target.m_ADDR=iport;
	}

	if (m_Ini.GetIniPar(L"COMMON",L"OPERATOR",STR_PARAMETER_FORMAT,&s)==0)
	{
			m_USER_Combo.SetWindowTextW(s);
			OPERATOR=s;
	}

	if (m_Ini.GetIniPar(L"PROTOCOL",L"PACKETPAUSE",INT_PARAMETER_FORMAT,&iport)==0)
	{
			if (iport>=10) PACKTOUT=iport;
	}

	if (m_Ini.GetIniPar(L"PROTOCOL",L"READPERIOD",INT_PARAMETER_FORMAT,&iport)==0)
	{
			if (iport>=1000) READPERD=iport;
	}

	if (m_Ini.GetIniPar(L"PROTOCOL",L"READERRLIMIT",INT_PARAMETER_FORMAT,&iport)==0)
	{
			if (iport<100) READERRLIMIT=iport;
	}

	TCHAR tpar[1024];

	for (int i=0; i<10; i++)
	{		
		swprintf(tpar,L"%OPERATOR%d",i);
		if (m_Ini.GetIniPar(L"OPERATORS",tpar,STR_PARAMETER_FORMAT,&s)==0)
		{
			OPERATORS[i]=s;
		}
		

	}	

	for (int i=0; i<10; i++)
	{
		if (OPERATORS[i]!="") m_USER_Combo.AddString(OPERATORS[i]);
	}

	m_USER_Combo.SetWindowText(OPERATOR);

	//if (m_Ini.GetIniPar(L"METRICS",L"SCALEDEVIDER",INT_PARAMETER_FORMAT,&iport)==0)
	//{
	//		if ((iport==1)||(iport==10)) m_pGraph->m_TARGSCALEFACTOR=iport;
	//}

	if (m_Ini.GetIniPar(L"METRICS",L"EMULATEMODE",INT_PARAMETER_FORMAT,&iport)==0)
	{
			if ((iport==1)||(iport==10)) m_Target.m_EMULATEMODE=iport;
	}


	if (m_Ini.GetIniPar(L"METRICS",L"TARGETMETRIC",INT_PARAMETER_FORMAT,&iport)==0)
	{
			for (int i=0; i<14; i++)
			{
				if (TARGETSCALE[i]==iport)
				{
					m_pGraph->m_TARGETSCALE=iport;
					this->m_RANG_Slider.SetPos(i+1);
				}
			}
	}


	if (MODE==0)
	{
		this->m_COMPORT_Static.ShowWindow(SW_SHOW);
		this->m_ADDR_Static.ShowWindow(SW_SHOW);
		this->m_COMOPEN_Btn.ShowWindow(SW_SHOW);
		this->m_ADDR_Static.ShowWindow(SW_SHOW);
		this->m_COM_Combo.ShowWindow(SW_SHOW);
		this->m_ADDR_Combo.ShowWindow(SW_SHOW);

		m_SEEK_Btn.ShowWindow(SW_SHOW);
		m_ON_Btn.ShowWindow(SW_SHOW);
		m_OFF_Btn.ShowWindow(SW_SHOW);

		this->m_BANNER_FRAME_Static.ShowWindow(SW_HIDE);
		this->m_BANNER_static.ShowWindow(SW_HIDE);
	}
	else
	{
		this->m_COMPORT_Static.ShowWindow(SW_HIDE);
		this->m_ADDR_Static.ShowWindow(SW_HIDE);

		/*
		this->m_COMOPEN_Btn.ShowWindow(SW_HIDE);
		this->m_ADDR_Static.ShowWindow(SW_HIDE);
		this->m_COM_Combo.ShowWindow(SW_HIDE);
		this->m_ADDR_Combo.ShowWindow(SW_HIDE);
		*/

		this->m_COMOPEN_Btn.EnableWindow(FALSE);
		this->m_ADDR_Static.EnableWindow(FALSE);
		this->m_COM_Combo.EnableWindow(FALSE);
		this->m_ADDR_Combo.EnableWindow(FALSE);


		m_SEEK_Btn.ShowWindow(SW_HIDE);
		m_ON_Btn.ShowWindow(SW_HIDE);
		m_OFF_Btn.ShowWindow(SW_HIDE);

		//!!!this->m_BANNER_FRAME_Static.ShowWindow(SW_SHOW);
		//!!!this->m_BANNER_static.ShowWindow(SW_SHOW);

		this->m_BANNER_FRAME_Static.ShowWindow(SW_HIDE);
		this->m_BANNER_static.ShowWindow(SW_HIDE);
		this->m_COMPORT_Static.ShowWindow(SW_SHOW);
		this->m_ADDR_Static.ShowWindow(SW_SHOW);
		this->m_COMOPEN_Btn.SetWindowTextW(L"Авто");


		LISTENFLAG=TRUE;
		DWORD   id     = 0u;


		if (READMODE==0) 		COMLISTENTHREAD = CreateThread(NULL, 256U, SerialListenThread, &m_Com, 0U, &id);

		//SetTimer(10,1000,NULL);

		m_FIRE_Btn.EnableWindow(TRUE);
	}


	CString title;
	if (MODE==0)
	{
		title.Format(L"TargetM [%s]",m_VERSION);
	}
	else
	{
		if (COMPORTNUM>0)   title.Format(L"TargetM [%s] [COM-порт=%d] [АДРЕС=%d]",m_VERSION, COMPORTNUM, m_Target.m_ADDR);
		else				title.Format(L"TargetM [%s] [COM-порт=не выбран] [АДРЕС=%d]",m_VERSION, m_Target.m_ADDR);

	}


	if (GLOBALSEEK==1) title.Format(L"TargetM [%s] [АВТОМАТИЧЕСКИЙ ПОИСК]",m_VERSION, COMPORTNUM, m_Target.m_ADDR);

	SetWindowText(title);

}


void CTargetMDlg::OnCbnKillfocusComboUser()
{
	// TODO: добавьте свой код обработчика уведомлений

	CString s;
	CString& ss=s;

	m_USER_Combo.GetWindowTextW(ss);

	OPERATOR=s;

	for (int i=0; i<10; i++)
	{
		if (OPERATORS[i]==s)
		{
			OPERATOR=s;
			return;
		}

		if (OPERATORS[i]==L"")
		{
			OPERATORS[i]=s;
			break;
		}

		if (i==9)
		{
			for (int j=9; j>0; j--)
			{
				OPERATORS[j]=OPERATORS[j-1];
			}


			OPERATORS[0]=s;
		}		

	}


	int count=m_USER_Combo.GetCount();
	
	for (int i=count; i>=0; i--)
	{
		m_USER_Combo.DeleteString(i);
	}
	
/*
	m_USER_Combo.AddString(L"1");
	m_USER_Combo.AddString(L"2");
	m_USER_Combo.AddString(L"3");
	m_USER_Combo.AddString(L"4");
	m_USER_Combo.AddString(L"5");
	m_USER_Combo.AddString(L"6");
	m_USER_Combo.AddString(L"7");
	m_USER_Combo.AddString(L"8");
	m_USER_Combo.AddString(L"9");
	m_USER_Combo.AddString(L"10");

return;//!!!
*/

	for (int i=0; i<10; i++)
	{
		if (OPERATORS[i]!="") m_USER_Combo.AddString(OPERATORS[i]);
	}

	m_USER_Combo.SetWindowText(OPERATOR);

	return;
}


void CTargetMDlg::OnBnClickedButtonProtocol()
{
	ViewReport(REPORT);
}


void CTargetMDlg::CreateReport(void)
{

	this->m_Target.ResCalculate(0);

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString sdate("");
	CString sdatetime("");
	
	CString fsdate("");
	CString fsdatetime("");

//дата время с двоеточиями для документа
	sdatetime.Format(L"%02d-%02d-%04d %02d:%02d:%02d",st.wDay,st.wMonth,st.wYear,st.wHour,st.wMinute,st.wSecond);
//дата с черточками для документа
	sdate.Format(L"%02d-%02d-%04d",st.wDay,st.wMonth,st.wYear);

//дата время с подчеркиваниями для имени файла
	fsdatetime.Format(L"%02d_%02d_%04d %02d_%02d_%02d",st.wDay,st.wMonth,st.wYear,st.wHour,st.wMinute,st.wSecond);
//дата с подчеркиваниями для имени папки
	fsdate.Format(L"%02d_%02d_%04d",st.wDay,st.wMonth,st.wYear);


	TCHAR tpath[1024];
	CString spath("");
	CString sspath("");

//берем текущий директорий
	::GetCurrentDirectoryW(1024,tpath);

//добавляем к имени хранилища протоколов
	spath+=tpath;
//имя папки ПРОТОКОЛЫ
	spath+=L"\\ПРОТОКОЛЫ";

//создаем папку протоколы
	::CreateDirectoryW(spath,0);

//добавляем имя подпапки с текущей датой
	spath+=L"\\";
	spath+=fsdate;
//создаем подпапку с текущей датой
	::CreateDirectoryW(spath,0);
    spath+=L"\\";

//создаем имя файла с номеромпротокола текущей датой и временем
	sspath.Format(L"Протокол_№%d_%s.txt",NUMREPORT,fsdatetime); 
	spath+=sspath;

//открываем файл на запись
	FILE* freport=NULL;
	freport=_wfopen(spath,L"wb");
	
	TCHAR tpar[1024];
	char  cpar[1024];

	CString s("");
	CString ss;
	for (int i=0; i<100; i++) REPORT[i]="";
	int idx=0;

	s.Format(L" ");
	REPORT[idx]=s; idx++; 

	s.Format(L"\r\n\r\n ПРОТОКОЛ №%d %s",NUMREPORT, sdatetime );
	REPORT[idx]=s; idx++; 

	sprintf(cpar,"\r\n ПРОТОКОЛ №%d            %S",NUMREPORT, sdatetime);
	fwrite(cpar,1, strlen(cpar),freport);

	s.Format(L" -------------------------------------------------------------------------------");
	REPORT[idx]=s; idx++; 

	sprintf(cpar,"\r\n -------------------------------------------------------------------------------");
	fwrite(cpar,1, strlen(cpar),freport);


	//s.Format(L" ");
	//REPORT[idx]=s; idx++; 

	s.Format(L" Оператор:   [%s]",OPERATOR);
	REPORT[idx]=s; idx++; 

//конвертор из UNICODE в ASCII с кириллицейтолько он работает!
	USES_CONVERSION;
	char* cbuf;
	cbuf = T2A(OPERATOR.GetString());
//это макрос ATL

	sprintf(cpar,"\r\n Оператор:   [%s]",cbuf);
	fwrite(cpar,1, strlen(cpar),freport);


	s.Format(L" Мишень:     [%d]",m_Target.m_ADDR);
	REPORT[idx]=s; idx++; 

//

	sprintf(cpar,"\r\n Мишень:     [%d]",m_Target.m_ADDR);
	fwrite(cpar,1, strlen(cpar),freport);

	sprintf(cpar,"\r\n -------------------------------------------------------------------------------");
	fwrite(cpar,1, strlen(cpar),freport);

	s.Format(L" ");
	REPORT[idx]=s; idx++; 

	//s.Format(L" ");
	//REPORT[idx]=s; idx++; 

	//s.Format(L" Номер пробоины   Отметка времени      Область попадания   Положение X   Положение Y   Cтатус управления");

	s.Format(L" Номер      Отметка      Область     Положение X   Положение Y    Входит в ");

	REPORT[idx]=s; idx++; 

	s.Format(L" пробоины   времени      попадания   мм            мм             круг/прямоуг.");


	//sprintf(cpar,"\r\n  Номер пробоины  Отметка времени  Область попадания  Положение X  Положение Y  Cтатус управления");

	sprintf(cpar,"\r\n Номер      Отметка      Область     Положение X   Положение Y    Входит в");
	fwrite(cpar,1, strlen(cpar),freport);

	sprintf(cpar,"\r\n пробоины   времени      попадания   мм            мм            круг/прямоуг.");
	fwrite(cpar,1, strlen(cpar),freport);

	sprintf(cpar,"\r\n -------------------------------------------------------------------------------");
	fwrite(cpar,1, strlen(cpar),freport);

	REPORT[idx]=s; idx++; 
 
	s.Format(L" -------------------------------------------------------------------------------");
	REPORT[idx]=s; idx++; 

	
	//for (int i=0; i<60; i++)
	//for (int i=0; i<m_Target.m_TESTPAR_gapsnum; i++)	
	for (int i=0; i<m_Target.m_Test.m_GAll; i++)	
	{
	
		if (m_Target.m_PULL.record[i].act==1)
		//sprintf(cpar,"\r\n   %d\t  %02d:%02d:%02d:%01d %d %d\t\t%d\t\t%d",
		sprintf(cpar,"\r\n   %d\t  %02d:%02d:%02d:%01d\t %d\t     %06.1f    \t   %06.1f\t          %d",
			    m_Target.m_PULL.record[i].res.gap,				
				m_Target.m_PULL.record[i].res.tick_hour,
				m_Target.m_PULL.record[i].res.tick_min,
				m_Target.m_PULL.record[i].res.tick_sec,
				m_Target.m_PULL.record[i].res.tick_dec,
				m_Target.m_PULL.record[i].res.area,
				(double)m_Target.m_PULL.record[i].res.X/10.,
				(double)m_Target.m_PULL.record[i].res.Y/10.,
				//m_Target.m_PULL.record[i].res.status
				m_Target.m_PULL.record[i].res.inCyrcle|m_Target.m_PULL.record[i].res.inRect				
			   );	

		else
		sprintf(cpar,"\r\n   *\t  %02d:%02d:%02d:%01d\t %d\t     %06.1f\t           %06.1f\t          %d",
			    //m_Target.m_PULL.record[i].res.gap,
				
				m_Target.m_PULL.record[i].res.tick_hour,
				m_Target.m_PULL.record[i].res.tick_min,
				m_Target.m_PULL.record[i].res.tick_sec,
				m_Target.m_PULL.record[i].res.tick_dec,
				m_Target.m_PULL.record[i].res.area,
				(double)m_Target.m_PULL.record[i].res.X/10,
				(double)m_Target.m_PULL.record[i].res.Y/10,
				//m_Target.m_PULL.record[i].res.status
				m_Target.m_PULL.record[i].res.inCyrcle|m_Target.m_PULL.record[i].res.inRect
			   );	


		fwrite(cpar,1, strlen(cpar),freport);

		s="";

		if (m_Target.m_PULL.record[i].res.gap==0)
		{
			sprintf(cpar,"  *");
			ss.Format  (L"  *");

		}
		else
		{
			sprintf(cpar,"  %d",m_Target.m_PULL.record[i].res.gap);
			ss.Format  (L"  %d",m_Target.m_PULL.record[i].res.gap);
		}

		for (int j=0; j<9-strlen(cpar); j++) ss+=L" ";

		ss+=L" ";

		s+=ss;

		
		sprintf(cpar,"%02d:%02d:%02d:%01d",
			m_Target.m_PULL.record[i].res.tick_hour,
			m_Target.m_PULL.record[i].res.tick_min,
			m_Target.m_PULL.record[i].res.tick_sec,
			m_Target.m_PULL.record[i].res.tick_dec
			  );

		ss.Format(L"%02d:%02d:%02d:%01d",
			m_Target.m_PULL.record[i].res.tick_hour,
			m_Target.m_PULL.record[i].res.tick_min,
			m_Target.m_PULL.record[i].res.tick_sec,
			m_Target.m_PULL.record[i].res.tick_dec
			  );

		for (int j=0; j<17-strlen(cpar); j++) ss+=L" ";
		ss+=L" ";		
		s+=ss;

		ss.Format(L"%d",m_Target.m_PULL.record[i].res.area);
		ss+=L"         ";		
		s+=ss;

		sprintf(cpar,"%06.1f",
			(double)m_Target.m_PULL.record[i].res.X/10
			  );

		ss.Format ( L"%06.1f",
			(double)m_Target.m_PULL.record[i].res.X/10
			  );

		for (int j=0; j<12-strlen(cpar); j++) ss+=L" ";
		ss+=L"  ";		
		s+=ss;

		sprintf(cpar,"%06.1f",
			(double)m_Target.m_PULL.record[i].res.Y/10
			  );

		ss.Format(L"%06.1f",
			(double)m_Target.m_PULL.record[i].res.Y/10
			  );

		for (int j=0; j<12-strlen(cpar); j++) ss+=L" ";
		ss+=L"    ";		
		s+=ss;

		//ss.Format(L"%d",m_Target.m_PULL.record[i].res.status);
		ss.Format(L"%d",m_Target.m_PULL.record[i].res.inRect|m_Target.m_PULL.record[i].res.inCyrcle);
		s+=ss;


		//ss.Format(L"| ");
		//s+=ss;


		//ss.Format(L"%d",m_Target.m_PULL.record[i].res.gap);
		//for (int j=0; i<strlen(cpar); j++) ss+=L" ";
		//ss+=L"        "
		//s+=ss;



		/*
			//m_Target.m_PULL.record[i].res.gap,
			m_Target.m_PULL.record[i].res.tick_hour,
			m_Target.m_PULL.record[i].res.tick_min,
			m_Target.m_PULL.record[i].res.tick_sec,
			m_Target.m_PULL.record[i].res.tick_dec,
			m_Target.m_PULL.record[i].res.area,
			m_Target.m_PULL.record[i].res.X,
			m_Target.m_PULL.record[i].res.Y,
			m_Target.m_PULL.record[i].res.status
		*/
			
			//);

		REPORT[idx]=s; idx++; 

	}


	s.Format(L" -------------------------------------------------------------------------------");
	REPORT[idx]=s; idx++; 

	sprintf(cpar,"\r\n -------------------------------------------------------------------------------");
	fwrite(cpar,1, strlen(cpar),freport);

	//s.Format(L"%s",m_Target.m_TSETRESULT_Str);
	//REPORT[idx]=s; idx++; 

	REPORT[idx]=" "; idx++; 

	sprintf(cpar,"\r\n");
	fwrite(cpar,1, strlen(cpar),freport);


	for (int i=0; i<60; i++)
	{
		if (m_Target.m_Test.m_TPar_strs[i]=="") break;

		REPORT[idx]=m_Target.m_Test.m_TPar_strs[i];
		idx++;

		cbuf = T2A(m_Target.m_Test.m_TPar_strs[i].GetString());
		sprintf(cpar,"\r\n%s",cbuf);
		fwrite(cpar,1, strlen(cpar),freport);

	}

	sprintf(cpar,"\r\n");
	fwrite(cpar,1, strlen(cpar),freport);

	REPORT[idx]=" "; idx++; 

	for (int i=0; i<60; i++)
	{
		if (m_Target.m_Test.m_TRes_strs[i]=="") break;
		REPORT[idx]=m_Target.m_Test.m_TRes_strs[i];
		idx++;

		cbuf = T2A(m_Target.m_Test.m_TRes_strs[i].GetString());
		sprintf(cpar,"\r\n%s",cbuf);
		fwrite(cpar,1, strlen(cpar),freport);

	}


	fclose(freport);

	/*
	//конвертор из UNICODE в ASCII с кириллицейтолько он работает!
//	USES_CONVERSION;
	//char* cbuf;
	cbuf = T2A(m_Target.m_TSETRESULT_Str.GetString());
//это макрос ATL
	*/

/////////////////////////////////////////////////////////////для REPORT///////////////////////////////////////////////

	s=L"";
	ss=L"";
	for (int i=0; i<100; i++) REPORT[i]="";
	idx=0;

	s.Format(L" ");
	REPORT[idx]=s; idx++; 

	s.Format(L"\r\n\r\n ПРОТОКОЛ №%d %s",NUMREPORT, sdatetime );
	REPORT[idx]=s; idx++; 

	s.Format(L" ---------------------------------------------------------------------------------------------------");
	REPORT[idx]=s; idx++; 

	s.Format(L" Оператор:   [%s]",OPERATOR);
	REPORT[idx]=s; idx++; 

	s.Format(L" Мишень:     [%d]",m_Target.m_ADDR);
	REPORT[idx]=s; idx++; 

//

	s.Format(L" ");
	REPORT[idx]=s; idx++; 
	s.Format(L" Номер      Отметка       Область        Положение X        Положение Y        Попадает в ");
	REPORT[idx]=s; idx++; 
	s.Format(L" пробоины   времени       попадания      мм                 мм                 круг/прямоуг.");
	REPORT[idx]=s; idx++;  
	s.Format(L" ---------------------------------------------------------------------------------------------------");
	REPORT[idx]=s; idx++; 

	for (int i=0; i<m_Target.m_Test.m_GAll; i++)	
	{

		s="";

		if (m_Target.m_PULL.record[i].res.gap==0)
		{
			ss.Format  (L"   *");
		}
		else
		{
			ss.Format  (L"  %02d",m_Target.m_PULL.record[i].res.gap);
		}

		ss+=L"       ";
		s+=ss;
		
		ss.Format(L"%02d:%02d:%02d:%01d",
			m_Target.m_PULL.record[i].res.tick_hour,
			m_Target.m_PULL.record[i].res.tick_min,
			m_Target.m_PULL.record[i].res.tick_sec,
			m_Target.m_PULL.record[i].res.tick_dec
			  );

		ss+=L"       ";		
		s+=ss;

		ss.Format(L"%d",m_Target.m_PULL.record[i].res.area);
		ss+=L"              ";		
		s+=ss;

		ss.Format ( L"%06.1f",
			(double)m_Target.m_PULL.record[i].res.X/10
			  );

		ss+=L"             ";		
		s+=ss;

		ss.Format(L"%06.1f",
			(double)m_Target.m_PULL.record[i].res.Y/10
			  );

		ss+=L"             ";		
		s+=ss;

		ss.Format(L"%d",m_Target.m_PULL.record[i].res.inRect|m_Target.m_PULL.record[i].res.inCyrcle);
		s+=ss;

		REPORT[idx]=s; idx++; 

	}


	s.Format(L" ---------------------------------------------------------------------------------------------------");
	REPORT[idx]=s; idx++; 

	REPORT[idx]=" "; idx++; 

	for (int i=0; i<60; i++)
	{
		if (m_Target.m_Test.m_TPar_strs[i]=="") break;

		REPORT[idx]=m_Target.m_Test.m_TPar_strs[i];
		idx++;
	}


	REPORT[idx]=" "; idx++; 

	for (int i=0; i<60; i++)
	{
		if (m_Target.m_Test.m_TRes_strs[i]=="") break;
		REPORT[idx]=m_Target.m_Test.m_TRes_strs[i];
		idx++;

	}







	
}


void CTargetMDlg::ViewReport(CString* report)
{
	// TODO: добавьте свой код обработчика уведомлений
	CReportDlg dlg;
	dlg.m_Report=report;
	dlg.m_Target=&m_Target;
	dlg.DoModal();

}


void CTargetMDlg::OnEnSetfocusEditState()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_FIRE_Btn.SetFocus();
	//m_FIRE_Btn.SetFocus();
}


void CTargetMDlg::PutConsoleC(CString str)
{
	if (str=="") CMDCONSOLE="";
	else
	{
		CMDCONSOLE+=str;
		CMDCONSOLE+=L"\r\n";
	}

	this->m_CONSOLEC_Edit.SetWindowTextW(CMDCONSOLE);

	this->m_CONSOLEC_Edit.SetScrollRange(1,0,100000);
	this->m_CONSOLEC_Edit.LineScroll(100000);
	this->m_CONSOLEC_Edit.RedrawWindow();

}

void CTargetMDlg::PutConsoleR(CString str)
{
	if (str=="") RESPONDCONSOLE="";
	else
	{
		RESPONDCONSOLE+=str;
		RESPONDCONSOLE+=L"\r\n";
	}

	this->m_CONSOLER_Edit.SetWindowTextW(RESPONDCONSOLE);

	this->m_CONSOLER_Edit.SetScrollRange(1,0,100000);
	this->m_CONSOLER_Edit.LineScroll(100000);
	this->m_CONSOLER_Edit.RedrawWindow();

}


void CTargetMDlg::OnCbnEditchangeComboTestnum()
{
	// TODO: добавьте свой код обработчика уведомлений




	return;
}


void CTargetMDlg::OnCbnSelchangeComboTestnum()
{
	// TODO: добавьте свой код обработчика уведомлений

	//UpdateData(TRUE);

	SetTimer(30,100,NULL);

	/*
	int testnum=1;

	CString s;
	CString& ss=s;

	this->m_TESTNUM_Combo.GetWindowTextW(ss);

	testnum=StrToInt(s);

	this->m_Target.SetTestNгmber(testnum);
	*/

	return;
}


void CTargetMDlg::OnBnClickedButtonTestpar()
{
	// TODO: добавьте свой код обработчика уведомлений
	CTestParDlg dlg;
	dlg.m_Test=&this->m_Target.m_Test;

	dlg.DoModal();
}
