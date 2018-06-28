#include "StdAfx.h"
#include "SerialLight.h"


CSerialLight::CSerialLight(void)
{

	h=INVALID_HANDLE_VALUE;
	st.p=255;

	st.on=FALSE;st.run=FALSE;st.res=TRUE;st.cmd=0;
    to.ReadIntervalTimeout=0;
    to.ReadTotalTimeoutMultiplier=0;
    to.ReadTotalTimeoutConstant=50;
    to.WriteTotalTimeoutMultiplier=0;
    to.WriteTotalTimeoutConstant=50;
    InitDCB(&dcb);
    st.baud=19200;

/*
  
  br=0;bw=0;er=0;ew=0;ec=0;eo=0;lerr=0;
  st.on=FALSE;st.run=FALSE;st.res=TRUE;st.cmd=0;
  to.ReadIntervalTimeout=0;
  to.ReadTotalTimeoutMultiplier=0;
  to.ReadTotalTimeoutConstant=500;
  to.WriteTotalTimeoutMultiplier=0;
  to.WriteTotalTimeoutConstant=500;
  InitDCB(&dcb);
  TmOpen=Now();TmClose=TmOpen;
  s_protocol_type=PRT_DEFAULT;
  Protocol211=NULL;
  ServiceMode=FALSE;
  st.baud=115200;
*/

}


CSerialLight::~CSerialLight(void)
{
}


BOOL CSerialLight::Open(BOOL clr_err,DWORD port)
{
  CString Name;

  if(st.on) 
  {	
	  Close();
  }	

  st.p=port;
	  
  if (clr_err)
  {
	  /*
	  br=0;
	  bw=0;
	  er=0;
	  ew=0;
	  ec=0;
	  lerr=0;
	  */
  }


  st.run=TRUE;st.cmd=1;

  Sleep(100);
 
  Name.Format(L"\\\\.\\COM%d",st.p);

  h=CreateFile(Name,
	            GENERIC_READ|GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING,
				NULL);

    if(!(st.res=(h!=INVALID_HANDLE_VALUE)))
    {
      //eo++;
	  st.run=FALSE;
	  st.res=FALSE;
	  return st.res;
    }

    st.res=(SetCommState(h,&dcb)&&SetCommTimeouts(h,&(to)));

    if(st.res)
    {
      st.cmd=1;st.run=FALSE;st.on=TRUE;
      return st.res;
    }

    Close();
    //eo++;
	st.cmd=1;
	st.run=FALSE;
	st.res=FALSE;
	return st.res;
}


BOOL CSerialLight::PortNRead(DWORD n,BYTE *b)
{
  Nr=0;
  double d1,d2;
  st.run=TRUE;
  st.cmd=3;

  if(!st.on)
  {
   st.res=FALSE;
   st.run=FALSE;
   return st.res;
  }

   st.res=ReadFile(h,b,n,&Nr,0);
   //br+=Nr;
   //lbr=Nr;
   st.res=st.res&&(Nr==n);

  st.run=FALSE;
  return st.res;
}


BOOL CSerialLight::PortNWrite(DWORD n,BYTE *b)
{
  Nw=0;
  st.run=TRUE;
  st.cmd=4;
    
  if(!st.on)
  {
	  st.res=FALSE;
	  st.run=FALSE;
	  return st.res;
  }

  st.res=WriteFile(h,b,n,&Nw,NULL);
  //bw+=Nw;
  //if(WErr) st.res=FALSE;
  st.res=st.res&&(Nw==n);

  st.run=FALSE;
  return st.res;
}


BOOL CSerialLight::SetTimeouts(DWORD rtime,DWORD wtime)
{
  //st.cmd=5;
  //Sleep(1);
  //st.run=TRUE;
  st.run=TRUE;
  st.cmd=5; 
  
  if(!st.on)
  {
	  st.res=FALSE;
	  st.run=FALSE;
	  return FALSE;
  }

  to.ReadIntervalTimeout=0;
  to.ReadTotalTimeoutMultiplier=0;
  to.ReadTotalTimeoutConstant=rtime;
  to.WriteTotalTimeoutMultiplier=0;
  to.WriteTotalTimeoutConstant=wtime;
  st.res=SetCommTimeouts(h,&(to));
  st.run=FALSE;
  return st.res;

}

BOOL CSerialLight::Close(void)
{

  st.run=TRUE;
  st.cmd=2;

  if(!st.on)
  {
	  st.res=TRUE;
	  st.run=FALSE;
	  return TRUE;
  }

  Sleep(100);

  st.res=CloseHandle(h);
  h=INVALID_HANDLE_VALUE;
  st.on=FALSE;
  st.run=FALSE;
  return st.res;

}


void CSerialLight::InitDCB(DCB* PortDCB)
{
  PortDCB->DCBlength=sizeof(DCB);
  // Specifies the length, in bytes, of the DCB structure.
  PortDCB->BaudRate=19200;
  //PortDCB->BaudRate=38400;
  //PortDCB->BaudRate=57600;
  //PortDCB->BaudRate=115200;
  //PortDCB->BaudRate=230400;
  //PortDCB->BaudRate=460800;
  //PortDCB->BaudRate=921600;
  // Specifies the baud rate at which the communications device operates.
  // This member can be an actual baud rate value
  PortDCB->fBinary=1;
  // Specifies whether binary mode is enabled. The Win32 API does not support
  // nonbinary mode transfers, so this member should be TRUE. Trying to use
  // FALSE will not work.
  // Under Windows 3.1, if this member is FALSE, nonbinary mode is enabled,
  // and the character specified by the EofChar member is recognized on input
  // and remembered as the end of data.
  PortDCB->fParity=0;
  // Specifies whether parity checking is enabled. If this member is TRUE,
  // parity checking is performed and errors are reported.

  //PortDCB->fOutxCtsFlow=1;
  PortDCB->fOutxCtsFlow=0;
  // Specifies whether the CTS (clear-to-send) signal is monitored for output
  // flow control. If this member is TRUE and CTS is turned off, output is
  // suspended until CTS is sent again.
  PortDCB->fOutxDsrFlow=0;
  // Specifies whether the DSR (data-set-ready) signal is monitored for output
  // flow control. If this member is TRUE and DSR is turned off, output is
  // suspended until DSR is sent again.
  PortDCB->fDtrControl=0;
  // Specifies the DTR (data-terminal-ready) flow control. This member can
  // be one of the following values:
  // DTR_CONTROL_DISABLE   0x00	Disables the DTR line when the device is opened and
  //                            leaves it disabled.
  // DTR_CONTROL_ENABLE	   0x01 Enables the DTR line when the device is opened
  //                            and leaves it on.
  // DTR_CONTROL_HANDSHAKE 0x02	Enables DTR handshaking. If handshaking is
  //                            enabled, it is an error for the application
  //                            to adjust the line by using the
  //                            EscapeCommFunction function.
  PortDCB->fDsrSensitivity=0;
  // Specifies whether the communications driver is sensitive to the state of
  // the DSR signal. If this member is TRUE, the driver ignores any bytes
  // received, unless the DSR modem input line is high.
  PortDCB->fTXContinueOnXoff=0;
  // Specifies whether transmission stops when the input buffer is full and
  // the driver has transmitted the XoffChar character. If this member is TRUE, transmission
  // continues after the input buffer has come within XoffLim bytes of being full and
  // the driver has transmitted the XoffChar character to stop receiving bytes. If this
  // member is FALSE, transmission does not continue until the input buffer is within
  // XonLim bytes of being empty and the driver has transmitted the XonChar character to
  // resume reception.
  PortDCB->fOutX=0;
  // Specifies whether XON/XOFF flow control is used during transmission. If this
  // member is TRUE, transmission stops when the XoffChar character is received and
  // starts again when the XonChar character is received.
  PortDCB->fInX=0;
  // Specifies whether XON/XOFF flow control is used during reception. If this member is
  // TRUE, the XoffChar character is sent when the input buffer comes within XoffLim bytes
  // of being full, and the XonChar character is sent when the input buffer comes within
  // XonLim bytes of being empty.
  PortDCB->fErrorChar=0;
  // Specifies whether bytes received with parity errors are replaced with the character
  // specified by the ErrorChar member. If this member is TRUE and the fParity member is TRUE,
  // replacement occurs.
  PortDCB->fNull=0;
  // Specifies whether null bytes are discarded. If this member is TRUE, null bytes
  // are discarded when received.
  //PortDCB->fRtsControl=0x02;
  PortDCB->fRtsControl=0x0;
  // Specifies the RTS (request-to-send) flow control. If this value is zero,
  // the default is RTS_CONTROL_HANDSHAKE. This member can be one of the following values:
  // RTS_CONTROL_DISABLE	0x00   Disables the RTS line when the device is opened and
  //                               leaves it disabled.
  // RTS_CONTROL_ENABLE	    0x01   Enables the RTS line when the device is opened and
  //                               leaves it on.
  // RTS_CONTROL_HANDSHAKE	0x02   Enables RTS handshaking. The driver raises the RTS line
  //                               when the "type-ahead" (input) buffer is less
  //                               than one-half full and lowers the RTS line when the
  //                               buffer is more than three-quarters full. If handshaking
  //                               is enabled, it is an error for the application to adjust
  //                               the line by using the EscapeCommFunction function.
  // RTS_CONTROL_TOGGLE	    0x03   Specifies that the RTS line will be high if bytes are
  //                               available for transmission. After all buffered bytes
  //                               have been sent, the RTS line will be low.
  PortDCB->fAbortOnError=0;
  // Specifies whether read and write operations are terminated if an error occurs.
  // If this member is TRUE, the driver terminates all read and write operations with
  // an error status if an error occurs. The driver will not accept any further
  // communications operations until the application has acknowledged the error by
  // calling the ClearCommError function.
  PortDCB->fDummy2=0;
  // Reserved; do not use.
  //PortDCB->wReserved=0;
  // Not used; must be set to zero.
  PortDCB->XonLim=0;
  // Specifies the minimum number of bytes allowed in the input buffer before the XON
  // character is sent.
  PortDCB->XoffLim=0;
  // Specifies the maximum number of bytes allowed in the input buffer before the XOFF
  // character is sent. The maximum number of bytes allowed is calculated by subtracting
  // this value from the size, in bytes, of the input buffer.
  PortDCB->ByteSize=8;
  // Specifies the number of bits in the bytes transmitted and received.
  PortDCB->Parity=0;
  // Specifies the parity scheme to be used. This member can be one of the following values:
  // EVENPARITY	 2  Even
  // MARKPARITY	 3  Mark
  // NOPARITY	 0  No parity
  // ODDPARITY	 1  Odd
  // SPACEPARITY 4  Space
  PortDCB->StopBits=0;
  //Specifies the number of stop bits to be used. This member can be one of the following
  // values:
  // ONESTOPBIT	        0  1 stop bit
  // ONE5STOPBITS	1  1.5 stop bits
  // TWOSTOPBITS	2  2 stop bits
  PortDCB->XonChar=0;
  // Specifies the value of the XON character for both transmission and reception.
  PortDCB->XoffChar=0;
  // Specifies the value of the XOFF character for both transmission and reception.
  PortDCB->ErrorChar=0;
  // Specifies the value of the character used to replace bytes received with a
  // parity error.
  PortDCB->EofChar=0;
  // Specifies the value of the character used to signal the end of data.
  PortDCB->EvtChar=0;
  // Specifies the value of the character used to signal an event.
  PortDCB->wReserved1=0;
  //Reserved; do not use.
}


void CSerialLight::Scan(BYTE* list)
{
	HKEY hKey;
    DWORD i=0;
    TCHAR Name[4096];
    TCHAR Value[4096];
    DWORD cName=4096;
    DWORD cValue=4096;
	TCHAR keyName[100];

	LSTATUS stat = ERROR_NO_MORE_ITEMS;//ERROR_INVALID_FUNCTION;

	memset(list,0,256);

	int icom=0;
	int comidx=0;
	CString sport("");

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\SERIALCOMM", REG_OPTION_OPEN_LINK, KEY_QUERY_VALUE , &hKey) == ERROR_SUCCESS)
    {
        while((stat=RegEnumValue(hKey,i,Name,&cName,NULL,NULL,(LPBYTE)&Value,&cValue))!= ERROR_NO_MORE_ITEMS)
        {

			sport="";
			sport+=Value;
			sport.Remove('C');
			sport.Remove('O');
			sport.Remove('M');
			sport.Remove('.');
			sport.Remove('\\');

			icom=StrToInt(sport);
			list[comidx]=icom;
			comidx++;
            i++;
			memset(Value,0,1000);
			memset(Name,0,1000);
			cName=4096;
			cValue=4096;
        }
    }


//Sort
/*
	BYTE t=0;
	BYTE tmp[255];
	memset(tmp,0,255);
	int k=0;

	for (int i=1; i<255; i++)
	{
		for (int j=0; j<255; j++)
		{
			if (list[j]==i)
			{
				tmp[k]=i;
				list[j]=0;
				k++;
			}
		}
	}

	memcpy(list,tmp,255);
*/

	return;
}

void CSerialLight::Purge()
{
  
	PurgeComm(h,PURGE_TXCLEAR|PURGE_RXCLEAR);

}

BOOL CSerialLight::BytesAvilable()
{	
 if (st.on==FALSE)	return FALSE;
 COMSTAT comstat;
 DWORD dummy;
 ClearCommError(h, &dummy, &comstat);
 return comstat.cbInQue;
}
