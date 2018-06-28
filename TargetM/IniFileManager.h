#define STR_PARAMETER_FORMAT 0
#define INT_PARAMETER_FORMAT 1
#define FLT_PARAMETER_FORMAT 2

#pragma once

class CIniFileManager
{
private:

	TCHAR m_IniFilePath[1024];
	BOOL  m_Init;

public:
	CIniFileManager(void);
	~CIniFileManager(void);
	int Init(TCHAR* appname);
	int SaveIniPar(TCHAR* appname,TCHAR* keyname,int format, void* par);
	int GetIniPar (TCHAR* appname,TCHAR* keyname,int format, void* par);
};

