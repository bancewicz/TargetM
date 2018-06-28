// PrintListCtrl.h: interface for the CPrintListCtrl class.
// Author : S.Sunil Kumar
//			sam_sun_401@yahoo.co.uk
//
// This class is released into the public domain.
// You are free to use it in any way you like.
//
// This class is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this class may cause.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTLISTCTRL_H__7AFAF928_58E7_47B7_9520_F956A1505C6D__INCLUDED_)
#define AFX_PRINTLISTCTRL_H__7AFAF928_58E7_47B7_9520_F956A1505C6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcmn.h>			// MFC support for Windows Common Controls

class CPrintListCtrl  
{
public:
	bool PrintData(CListCtrl* pListCtrl, CString szPageTitle);
	CPrintListCtrl();
	virtual ~CPrintListCtrl();

private:
	// Print 
	RECT	m_PageRect;
	HDC		m_HdcPrint;
	SIZE	m_StSize;
	CFont	m_HeadingFont;
	CFont	m_ListFont;
	CFont   m_GeneralFont;
	long	m_CurrentYPos;
	long    m_RowHeight;
	CDC		*m_PrintDC;
	int		m_Space;
	int     *m_ColumnSpaces;	
	
	void PrintListCtrlHeading(CListCtrl* pListCtrl, CString szPageTitle);
	void PrintListData(CListCtrl* pListCtrl);
	int  GetColumnWidth(int theColumn, CListCtrl* pListCtrl);
	int	 GetRectangleHeight();

protected:

};

#endif // !defined(AFX_PRINTLISTCTRL_H__7AFAF928_58E7_47B7_9520_F956A1505C6D__INCLUDED_)
