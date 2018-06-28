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
	// ����������� ��������� ������ ���� ��������������
	afx_msg void OnEnChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CBrush m_Brush; // ���������� ������� ��������������
	CFont m_Font;   // ���������� ������� ��������������
	COLORREF m_ColorBG;   // ���� ���� ����
	COLORREF m_ColorText; // ���� ������

	CToolTipCtrl m_tooltip;

public: 
	// ��������� � ������ ����� ����
	void SetColorBG(const COLORREF& color);
	COLORREF GetColorBG(void) const;

	// ��������� � ������ ����� ������
	void SetColorText(const COLORREF& color); 
	COLORREF GetColorText(void) const;

	// ��������� ��������� ������
	void SetMyEditFont(const int& height, const int& weight, const CString& fontname);
	void SetMyEditFont(LOGFONT* logfont);
	// �������� ��������� �� CFont-������
	CFont* GetMyEditFont(void);

	// ������� CMyEdit
	BOOL CreateMyEdit(const CRect& rect, CWnd* parent, const unsigned int& ID=0);

	void SetToolTipText(CString s);
	void RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	//void OnMouseMove(UINT nFlags, CPoint point);

};


