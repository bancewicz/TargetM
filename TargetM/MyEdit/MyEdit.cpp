/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				Клочков Павел
//				http://www.interestprograms.ru
//				программы для работы и отдыха, исходники программ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MyEdit.cpp : implementation file
#include "stdafx.h"
#include "MyEdit.h"
#include ".\myedit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)
CMyEdit::CMyEdit()
{
	m_Brush.CreateSolidBrush(RGB(255,255,255));
	m_ColorText = RGB(0,0,0);
	m_Font.CreatePointFont(80,(LPCTSTR) "Microsoft Sans Serif");
}

CMyEdit::~CMyEdit()
{
	// удаляем соответствующие объекты
	if(m_Brush.m_hObject != NULL)
		m_Brush.DeleteObject();
	if(m_Font.m_hObject != NULL)
		m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyEdit message handlers

/*
Так быстро и с "малыми потерями" создать удобный для нас класс нам позволяет MFC -
серьезная поддержка для Window-программистов.

Возможности которыми мы используем при создании нашего класса окна редактирования,
появились в версии MFC выше 4.0.
CMyEdit посылает уведомление об изменении цветовых данных (WM_CTLCOLOR) 
родительскому окну (в нашем случае CTestMyEditDlg),
в свою очередь родительское окно отсылает обратно сообщение перерисовки дочернему окну 
определенным цветом кисти. Это родительское сообщение отлавливается в 
функции HBRUSH CtlColor(CDC* pDC, UINT nCtlColor), что дает нам возможность
устанавливать цветовые особенности дочернего окна по нашему усмотрению. Благодаря ClassWizard
нам предоставляется скелет функции HBRUSH CtlColor(CDC* pDC, UINT nCtlColor) и 
в карту сообщений вставляется макрос ON_WM_CTLCOLOR_REFLECT(). Остается дело за малым.
*/

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// установка параметров отображения текста
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_ColorText);


	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	// возвращение не нулевого объекта кисти если родительский обработчик не должен быть вызван
	// измените код возвращения на (return NULL) и никаких цветовых изменений вы здесь
	// произвести не сможете
	return m_Brush;
}


BOOL CMyEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEdit::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//cs.style &= ~WS_BORDER;
}

int CMyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// предварительная установка шрифта
	SetFont(&m_Font);

	return 0;
}

/*
создадим собстенную функцию создания CMyEdit на основе родительской, 
установим поля созданного окна редактирования слева и справа по 2 пиксела
*/

// непосредственное создание CMyEdit
BOOL CMyEdit::CreateMyEdit(const CRect& rect, CWnd* parent, const unsigned int& ID)
{
	BOOL result = Create(WS_VISIBLE|ES_AUTOHSCROLL, rect, parent, ID);
	SetMargins(2,2);
	return result;
}


// установка цвета фона
void CMyEdit::SetColorBG(const COLORREF& color)
{
	// при создании нового объекта предварительно удаляем старый
	if(m_Brush.m_hObject != NULL)
		m_Brush.DeleteObject();

	// создаем сплошную кисть определенного цвета
	m_Brush.CreateSolidBrush(color);
	Invalidate();
}
// получить цвет фона окна
COLORREF CMyEdit::GetColorBG(void) const
{
	return m_ColorBG;
}

/*
Изменив цвет текста обязательно перерисуем все окошко чтобы изменения вступили в силу
*/
// установка цвета текста
void CMyEdit::SetColorText(const COLORREF& color)
{
	m_ColorText = color;
	Invalidate();
}
// получить цвет текста
COLORREF CMyEdit::GetColorText(void) const
{
	return m_ColorText;
}

/*
Здесь мы можем изменить некоторые параметры шрифта - высоту, толщину, имя шрифта
*/

// установка параметров шрифта
void CMyEdit::SetMyEditFont(const int& height, const int& weight, const CString& fontname)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); //обнуляем структуру
	lf.lfHeight = height;  
	lf.lfWeight = weight;
	lf.lfCharSet = RUSSIAN_CHARSET;
//!!!	strcpy((char*)lf.lfFaceName, (const char*)fontname);
	if(m_Font.m_hObject != NULL)
		m_Font.DeleteObject();
	VERIFY(m_Font.CreateFontIndirect(&lf)); 
	SetFont(&m_Font);
}
/*
В этой функции мы можем изменить практически любые параметры шрифта
*/
// установка параметров шрифта
void CMyEdit::SetMyEditFont(LOGFONT* logfont)
{
	if(m_Font.m_hObject != NULL)
		m_Font.DeleteObject();
	VERIFY(m_Font.CreateFontIndirect(logfont)); 
	SetFont(&m_Font);
}

// получение объекта CFont
CFont* CMyEdit::GetMyEditFont(void)
{
	return &m_Font;
}
/*
Из этой функции мы можем отсылать сообщение родительскому 
окну об изменении текста в окне редактирования специальным сообщением
*/

// отслеживание события изменения текста в CMyEdit
void CMyEdit::OnEnChange()
{
	GetParent()->SendMessage( WM_MYEDIT_CHANGE, (WPARAM)m_hWnd);
}

void CMyEdit::SetToolTipText(CString s)
{
	if(m_tooltip.m_hWnd==NULL){
		if(m_tooltip.Create(this))	//first assignment
			if(m_tooltip.AddTool(this, (LPCTSTR)s))
				m_tooltip.Activate(1);
	} else {
		m_tooltip.UpdateTipText((LPCTSTR)s,this);
	}
}

void CMyEdit::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
// This function will create a MSG structure, fill it in a pass it to
// the ToolTip control, m_ttip.  Note that we ensure the point is in window
// coordinates (relative to the control's window).
	
	if(NULL != m_tooltip.m_hWnd){ 
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);

		m_tooltip.RelayEvent(&msg);
	}
}

void CMyEdit::OnMouseMove(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: Mouse\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	
	RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are in capture mode, button has been pressed down
    //recently and not yet released - therefore check is we are
    //actually over the button or somewhere else. If the mouse
    //position changed considerably (e.g. we moved mouse pointer
    //from the button to some other place outside button area)
    //force the control to redraw
    //
/*
    if ((m_button_down) && (::GetCapture() == m_hWnd)) {
	    POINT p2 = point;
        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        bool pressed = ((GetState() & BST_PUSHED) == BST_PUSHED);
        bool need_pressed = (mouse_wnd == m_hWnd);
        if (pressed != need_pressed) {
            //TRACE("* %08X Redraw\n", GetTickCount());
            SetState(need_pressed ? TRUE : FALSE);
            Invalidate();
        }
    } else {

	//Otherwise the button is released. That means we should
    //know when we leave its area - and so if we are not tracking
    //this mouse leave event yet, start now!
    //
        if (!m_tracking) {
            TRACKMOUSEEVENT t = {
                sizeof(TRACKMOUSEEVENT),
                TME_LEAVE,
                m_hWnd,
                0
            };
            if (::_TrackMouseEvent(&t)) {
                //TRACE("* Mouse enter\n");
                m_tracking = true;
                Invalidate();
            }
        }
    }
*/
    //Forward this event to superclass
    CEdit::OnMouseMove(nFlags, point);
}
