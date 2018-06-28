
#pragma once
class CShotReport
{
public:
	CShotReport(void);
	~CShotReport(void);

	CString m_Header;
	CString m_Shots[60];

	void View(void);
};

