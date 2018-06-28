#include "StdAfx.h"
#include "FineTimer.h"


CFineTimer::CFineTimer(void)
{
	hTimer = NULL;
}


CFineTimer::~CFineTimer(void)
{

}


/*
	
	Переключение системного таймера Windows на разрешение 1 мс

*/

void CFineTimer::SetSysTimeRsesoution(void)
{
	//check current system timer resolution
	ULONG ulMinRes = 0;
    ULONG ulMaxRes = 0;
    ULONG ulCurRes = 0;
	HMODULE NTDLLModuleHandle=NULL;
	NTDLLModuleHandle = LoadLibrary(L"NTDLL.DLL");

	if (NTDLLModuleHandle!=NULL)
	{
		typedef int (WINAPI *pNtQueryTimerResolution)(ULONG* ulMinRes, ULONG* ulMaxRes, ULONG* ulCurRes );
		pNtQueryTimerResolution GetSysRes;
		GetSysRes = (pNtQueryTimerResolution)::GetProcAddress(NTDLLModuleHandle,"NtQueryTimerResolution");
		int code = (*GetSysRes)(&ulMinRes, &ulMaxRes, &ulCurRes );

		//set current system timer resolution as 1ms
		ulCurRes=10000;
		typedef int (WINAPI *pNtSetTimerResolution)(ULONG ulMinRes, BOOL par, ULONG* ulCurRes );
		pNtSetTimerResolution SetSysRes;
		SetSysRes = (pNtSetTimerResolution)::GetProcAddress(NTDLLModuleHandle,"NtSetTimerResolution");
		SetSysRes(5000,TRUE,&ulCurRes);
	}
	else
	{
		AfxMessageBox(L"Fine Timer: Load NTDLL.DLL fail!");
	}

}

/*
Инициализация точного таймера разрешение по умолчанию 1мкс
*/

int CFineTimer::Init(void)
{
	//liDueTime.QuadPart = -10000000LL/per;

	liDueTime.QuadPart = -10LL;

	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);

    if (NULL == hTimer)
    {
        AfxMessageBox(L"CreateWaitableTimer failed \n");
        return 0;
    }

	/*
	if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		AfxMessageBox(L"SetWaitableTimer failed \n");
		return 0;
	}
	*/

	return 1;
}

/*
	Задержка на 1 период 1 мкс
*/

int CFineTimer::Sleep_one_us(void)
{

	if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		AfxMessageBox(L"SetWaitableTimer failed!");
		return 0;
	}


	if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
	{
		AfxMessageBox(L"WaitForSingleObject failed \n");
		return 0;
	}	

	return 1;
}

/*
	Задержка на заданное время в мкс до 1000000 мкс
*/

int CFineTimer::Delay_us(int par_us)
{

	if (par_us>1000000) par_us=1000000;

	LARGE_INTEGER mliDueTime;

	mliDueTime.QuadPart = -10LL*par_us;

	if (!SetWaitableTimer(hTimer, &mliDueTime, 0, NULL, NULL, 0))
	{
		AfxMessageBox(L"SetWaitableTimer failed!");
		return 0;
	}


	if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
	{
		AfxMessageBox(L"WaitForSingleObject failed \n");
		return 0;
	}	

	return 1;
}