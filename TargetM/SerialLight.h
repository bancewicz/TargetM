#pragma once


typedef struct
{

  int p;
  bool on;
  bool run;
  bool res;
  int cmd;
  int baud;

} SCOMState;


class CSerialLight
{
public:
	CSerialLight(void);
	~CSerialLight(void);

private:

  HANDLE h;
  void InitDCB(DCB* PortDCB);	
  COMMTIMEOUTS to;
  DCB  dcb;

public:

 SCOMState st;

 DWORD Nr;
 DWORD Nw;

 BOOL Open(BOOL clr_err,DWORD port);
 BOOL Close(void);
 BOOL PortNRead(DWORD n,BYTE *b);
 BOOL CSerialLight::PortNWrite(DWORD n,BYTE *b);
 BOOL CSerialLight::SetTimeouts(DWORD rtime,DWORD wtime);
 void CSerialLight::Scan(BYTE* list);
 void Purge();
 BOOL BytesAvilable();

};

