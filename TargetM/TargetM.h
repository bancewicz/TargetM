
// TargetM.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTargetMApp:
// � ���������� ������� ������ ��. TargetM.cpp
//

class CTargetMApp : public CWinApp
{
public:
	CTargetMApp();

// ���������������
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTargetMApp theApp;