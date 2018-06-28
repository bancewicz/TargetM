
// DlgProxy.h: файл заголовка
//

#pragma once

class CTargetMDlg;


// конечный объект команды CTargetMDlgAutoProxy

class CTargetMDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CTargetMDlgAutoProxy)

	CTargetMDlgAutoProxy();           // защищенный конструктор, используемый при динамическом создании

// Атрибуты
public:
	CTargetMDlg* m_pDialog;

// Операции
public:

// Переопределение
	public:
	virtual void OnFinalRelease();

// Реализация
protected:
	virtual ~CTargetMDlgAutoProxy();

	// Созданные функции схемы сообщений

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CTargetMDlgAutoProxy)

	// Автоматически созданные функции диспетчерской карты OLE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

