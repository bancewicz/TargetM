
// DlgProxy.cpp : ���� ����������
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
	
	// ����� ���������� ��������, ���� ������ 
	//	������������� �������, ����������� �������� AfxOleLockApp.
	AfxOleLockApp();

	// �������� ������ � ����������� ���� ����� ���������
	//  �������� ���� ����������. ������ ���������� ��������� ������-������� ���
	//  ������� � �������, � ����� ������ �������� ��������� ������� �� ����
	//  ������-������.
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
	// ����� �������� ������ ����������, ����� ��� ������� ���������
	// 	�������������, ���������� �������� AfxOleUnlockApp.
	//  ����� ������� ����� ���������� ������� ���������� ����
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CTargetMDlgAutoProxy::OnFinalRelease()
{
	// ����� ����� ����������� ��������� ������ �� ������ �������������,
	// ����� ������ OnFinalRelease. ������� ����� �������������
	// ������ ������. ����� ������� �������� ������ ���������
	// �������������� ������� �������.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CTargetMDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTargetMDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ����������. ��������� ��������� ��� IID_ITargetM ��� ��������� �������� typesafe
//  �� VBA. IID ������ ��������� � GUID, ������� ������ � 
//  disp-��������� � ����� .IDL.

// {5B753AAF-3787-4CE8-9B7E-702C3C22DDE7}
static const IID IID_ITargetM =
{ 0x5B753AAF, 0x3787, 0x4CE8, { 0x9B, 0x7E, 0x70, 0x2C, 0x3C, 0x22, 0xDD, 0xE7 } };

BEGIN_INTERFACE_MAP(CTargetMDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CTargetMDlgAutoProxy, IID_ITargetM, Dispatch)
END_INTERFACE_MAP()

// ������ IMPLEMENT_OLECREATE2 ��������� � StdAfx.h ����� �������
// {AD700F4B-8CFB-4FAB-9F44-DCE4640C7B9E}
IMPLEMENT_OLECREATE2(CTargetMDlgAutoProxy, "TargetM.Application", 0xad700f4b, 0x8cfb, 0x4fab, 0x9f, 0x44, 0xdc, 0xe4, 0x64, 0xc, 0x7b, 0x9e)


// ����������� ��������� CTargetMDlgAutoProxy
