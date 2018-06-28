
// DlgProxy.h: ���� ���������
//

#pragma once

class CTargetMDlg;


// �������� ������ ������� CTargetMDlgAutoProxy

class CTargetMDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CTargetMDlgAutoProxy)

	CTargetMDlgAutoProxy();           // ���������� �����������, ������������ ��� ������������ ��������

// ��������
public:
	CTargetMDlg* m_pDialog;

// ��������
public:

// ���������������
	public:
	virtual void OnFinalRelease();

// ����������
protected:
	virtual ~CTargetMDlgAutoProxy();

	// ��������� ������� ����� ���������

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CTargetMDlgAutoProxy)

	// ������������� ��������� ������� ������������� ����� OLE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

