
// DlgProxy.cpp : файл реализации
//

#include "stdafx.h"
#include "TargetM.h"
#include "DlgProxy.h"
#include "TargetMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTargetMDlgAutoProxy

IMPLEMENT_DYNCREATE(CTargetMDlgAutoProxy, CCmdTarget)

CTargetMDlgAutoProxy::CTargetMDlgAutoProxy()
{
	EnableAutomation();
	
	// Чтобы приложение работало, пока объект 
	//	автоматизации активен, конструктор вызывает AfxOleLockApp.
	AfxOleLockApp();

	// Получает доступ к диалоговому окну через указатель
	//  главного окна приложения. Задает внутренний указатель прокси-сервера для
	//  доступа к диалогу, а также задает обратный указатель диалога на этот
	//  прокси-сервер.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CTargetMDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CTargetMDlg)))
		{
			m_pDialog = reinterpret_cast<CTargetMDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CTargetMDlgAutoProxy::~CTargetMDlgAutoProxy()
{
	// Чтобы прервать работу приложения, когда все объекты создаются
	// 	автоматически, деструктор вызывает AfxOleUnlockApp.
	//  Среди прочего будет уничтожено главное диалоговое окно
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CTargetMDlgAutoProxy::OnFinalRelease()
{
	// Когда будет освобождена последняя ссылка на объект автоматизации,
	// Будет вызван OnFinalRelease. Базовый класс автоматически
	// удалит объект. Перед вызовом базового класса требуется
	// дополнительная очистка объекта.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CTargetMDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTargetMDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Примечание. Добавлена поддержка для IID_ITargetM для поддержки привязки typesafe
//  из VBA. IID должен совпадать с GUID, который вложен в 
//  disp-интерфейс в файле .IDL.

// {5B753AAF-3787-4CE8-9B7E-702C3C22DDE7}
static const IID IID_ITargetM =
{ 0x5B753AAF, 0x3787, 0x4CE8, { 0x9B, 0x7E, 0x70, 0x2C, 0x3C, 0x22, 0xDD, 0xE7 } };

BEGIN_INTERFACE_MAP(CTargetMDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CTargetMDlgAutoProxy, IID_ITargetM, Dispatch)
END_INTERFACE_MAP()

// Макрос IMPLEMENT_OLECREATE2 определен в StdAfx.h этого проекта
// {AD700F4B-8CFB-4FAB-9F44-DCE4640C7B9E}
IMPLEMENT_OLECREATE2(CTargetMDlgAutoProxy, "TargetM.Application", 0xad700f4b, 0x8cfb, 0x4fab, 0x9f, 0x44, 0xdc, 0xe4, 0x64, 0xc, 0x7b, 0x9e)


// обработчики сообщений CTargetMDlgAutoProxy
