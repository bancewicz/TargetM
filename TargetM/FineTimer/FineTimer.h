#pragma once

class CFineTimer
{
public:

	CFineTimer(void);
	~CFineTimer(void);
	static void SetSysTimeRsesoution(void);

	int Init(void);
	int Sleep_one_us(void);
	int Delay_us(int par_us);

private:

	HANDLE hTimer;
    LARGE_INTEGER liDueTime;

};

