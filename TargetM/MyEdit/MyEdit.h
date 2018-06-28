#pragma once

#define WM_MYEDIT_CHANGE WM_USER+1


class CMyEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyEdit)

public:
	CMyEdit();
	virtual ~CMyEdit();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_MESSAGE_MAP()

private:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// отслеживаем изменения техста окна редактирования
	afx_msg void OnEnChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CBrush m_Brush; // необходимо удалять самостоятельно
	CFont m_Font;   // необходимо удалять самостоятельно
	COLORREF m_ColorBG;   // цвет фона окна
	COLORREF m_ColorText; // цвет шрифта

	CToolTipCtrl m_tooltip;

public: 
	// установка и запрос цвета фона
	void SetColorBG(const COLORREF& color);
	COLORREF GetColorBG(void) const;

	// установка и запрос цвета шрифта
	void SetColorText(const COLORREF& color); 
	COLORREF GetColorText(void) const;

	// установка атрибутов шрифта
	void SetMyEditFont(const int& height, const int& weight, const CString& fontname);
	void SetMyEditFont(LOGFONT* logfont);
	// получаем указатель на CFont-объект
	CFont* GetMyEditFont(void);

	// создаем CMyEdit
	BOOL CreateMyEdit(const CRect& rect, CWnd* parent, const unsigned int& ID=0);

	void SetToolTipText(CString s);
	void RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	//void OnMouseMove(UINT nFlags, CPoint point);

};


