#include "StdAfx.h"
#include "IniFileManager.h"


CIniFileManager::CIniFileManager(void)
{
	memset(m_IniFilePath,0,2048);
	m_Init=FALSE;
}


CIniFileManager::~CIniFileManager(void)
{

}


int CIniFileManager::Init(TCHAR* appname)
{

	//::GetCurrentDirectory(200,(LPTSTR)dir);

	::GetCurrentDirectoryW(1024,m_IniFilePath);

	wcscat(m_IniFilePath,L"\\");
	wcscat(m_IniFilePath,appname);
	wcscat(m_IniFilePath,L".INI");


	m_Init=TRUE;
	return 0;
}

/*
#define STR_PARAMETER_FORMAT 0
#define INT_PARAMETER_FORMAT 1
#define FLT_PARAMETER_FORMAT 2
*/

int CIniFileManager::SaveIniPar(TCHAR* appname,TCHAR* keyname,int format, void* par)
{

	if (m_Init==FALSE)
	{
		AfxMessageBox(L"IniFileManager: Ini file path is not initialized!");
		return 2;
	}

	CString spar("");

	switch(format)
	{
		case STR_PARAMETER_FORMAT:
			spar.Format(L"%s",(*(CString*)par));
		break;

		case INT_PARAMETER_FORMAT:
			spar.Format(L"%d",(*(int*)par));
		break;

		case FLT_PARAMETER_FORMAT:
			spar.Format(L"%f",(*(float*)par));
		break;

		default:
			AfxMessageBox(L"IniFileManager: Unknown format of INI parameter!");
		return 1;
	}

		if (WritePrivateProfileString((LPCWSTR)appname,(LPCWSTR)keyname,(LPCWSTR)spar,m_IniFilePath))
		{
			return 0;
		}
		else
		{
			return 1;
		}
}


int CIniFileManager::GetIniPar(TCHAR* appname,TCHAR* keyname,int format, void* par)
{

		TCHAR tpar[256];

		//if (GetPrivateProfileString(lpAppName,lpKeyName,NULL,(__in LPTSTR)comport,200,(__in LPCTSTR)dir))

		
		if (GetPrivateProfileStringW((LPCWSTR)appname,(LPCWSTR)keyname,NULL,(LPWSTR)tpar,256,(LPWSTR)m_IniFilePath))
		{

			switch(format)
			{
				case STR_PARAMETER_FORMAT:
					*(CString*)par=tpar;
				break;

				case INT_PARAMETER_FORMAT:
					*(int*)par=StrToInt((LPCWSTR)tpar);
				break;

				case FLT_PARAMETER_FORMAT:
					swscanf(tpar,L"%f",(float*)par);
				break;

				default:
					AfxMessageBox(L"IniFileManager: Unknown format of INI parameter!");
				return 1;
			}						

			return 0;
		}
		else
		{
			return 1;
		}
	
}
