
// TargetM.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTargetMApp:
// О реализации данного класса см. TargetM.cpp
//

class CTargetMApp : public CWinApp
{
public:
	CTargetMApp();

// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTargetMApp theApp;