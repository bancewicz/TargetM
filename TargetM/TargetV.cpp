#include "StdAfx.h"
#include "TargetV.h"
#include "math.h"
#include "Line.h"

//--------------------------GLOBALS---------------------------------------------

/*
	long tstR=0;
	long tstH=0;
	long tstW=0;
*/

//координаты центра круга
	long tstCcenterX=0;
	long tstCcenterY=0;

//Координаты углов прямоугольника 
	long tstR1X=0;
	long tstR1Y=0;

	long tstR2X=0;
	long tstR2Y=0;

	long tstR3X=0;
	long tstR3Y=0;

	long tstR4X=0;
	long tstR4Y=0;


//-----------------------------------------------------------------------------


CTargetM::CTargetM(void)
{
	memset(m_bufTX,0,1024);
	memset(m_bufRX,0,1024);

	m_resX=0;
	m_resY=0;

	m_Tarea=TargetM_area::Tarea_0;

	m_Timeticks=0;

	m_numGAP=1;

	m_ADDR=0x1F;

	m_Auto=FALSE;

	m_STOP=FALSE;

	memset(&m_PULL,0,sizeof(TargetM_res_pull));

	m_TestOver=FALSE;

	ctrlX=0;
	ctrlY=140;

	m_TEST_NUmber=1;
	m_TESTPAR_gapsnum=60;
	m_EMULATEMODE=0;
}


CTargetM::~CTargetM(void)
{
}


void CTargetM::Clear(void)
{
	memset(m_bufTX,0,1024);
	memset(m_bufRX,0,1024);

	m_resX=0;
	m_resY=0;

	m_Tarea=TargetM_area::Tarea_0;

	m_Timeticks=0;

	m_rxIDX=0;
	m_txIDX=0;

	//this->ClearPULL();
	//m_numGAP=0;

}

//void CTargetM::Trace(CEdit* timeticks,CEdit* resx,CEdit* resy,CEdit* tarea,CMyEdit* tstatus, CSerial*  serial)
void CTargetM::Trace(CEdit* timeticks,CEdit* resx,CEdit* resy,CEdit* tarea,CMyEdit* tstatus, CSerialLight*  serial)
{
	Clear();

	m_TIMETICKS_Edit=timeticks;
	m_RESX_Edit=resx;
	m_RESY_Edit=resy;
	m_TAREA_Edit=tarea;
	m_TSTATUS_Edit=tstatus;
	m_Com=serial;

}

/*
Формирование команды в сторону мишени
*/

BOOL CTargetM::CMD_create(TargetM_cmd cmd)
{
	Clear();

	m_Info=L"команда = ";

	memset (m_bufTX,0,1024);
	m_txIDX=0;

//Байт 0 адрес мишени
	m_bufTX[m_txIDX]=m_ADDR; m_txIDX++;

//Байт 1 длина пакета пока 0
	m_bufTX[m_txIDX]=0; m_txIDX++;

//Заполняем код операции
	switch (cmd)
	{

//Команда поиска мишени 9F	BB	CC //9F	AB	AC
		case TCMD_SEEK:

			/*
			m_bufTX[m_txIDX]=0x9F; m_txIDX++;
			m_bufTX[m_txIDX]=0xAB; m_txIDX++;
			m_bufTX[m_txIDX]=0xAC; m_txIDX++;

			*/

			m_bufTX[m_txIDX]=0x9F; m_txIDX++;
			m_bufTX[m_txIDX]=0xBB; m_txIDX++;
			m_bufTX[m_txIDX]=0xCC; m_txIDX++;


			m_Info+="ПОИСК\r\n";
        break;

//Команда включить мишень A0	FF	00	FF	FF	FF	00	00
		case TCMD_ON:
			m_bufTX[m_txIDX]=0xA0; m_txIDX++;
			m_bufTX[m_txIDX]=0xFF; m_txIDX++;
			m_bufTX[m_txIDX]=0x00; m_txIDX++;
			m_bufTX[m_txIDX]=0xFF; m_txIDX++;
			m_bufTX[m_txIDX]=0xFF; m_txIDX++;
			m_bufTX[m_txIDX]=0xFF; m_txIDX++;
			m_bufTX[m_txIDX]=0x00; m_txIDX++;
			m_bufTX[m_txIDX]=0x00; m_txIDX++;
			m_Info+="ВКЛЮЧИТЬ\r\n";
        break;
		
//Команда выключить мишень A1	XX
		case TCMD_OFF:
			m_bufTX[m_txIDX]=0xA1; m_txIDX++;
			m_bufTX[m_txIDX]=0x00; m_txIDX++;
			m_Info+="ВЫКЛЮЧИТЬ\r\n";
        break;

//Команда читать данные A3	ХХ	00	00
		case TCMD_READ:
			m_bufTX[m_txIDX]=0xA3; m_txIDX++;
			m_bufTX[m_txIDX]=m_numGAP; m_txIDX++;
			//m_bufTX[m_txIDX]=0; m_txIDX++;
			m_bufTX[m_txIDX]=0x00; m_txIDX++;
			m_bufTX[m_txIDX]=0x00; m_txIDX++;
			m_Info+="ЧТЕНИЕ\r\n";
        break;

		default:
			AfxMessageBox(L"Ошибка создания команды: неопознанный код команды!");
			m_Info+="ОШИБКА\r\n";
        return false;
	}

	m_bufTX[1]=m_txIDX+2;

	unsigned short crc16=CRC16(m_bufTX,m_txIDX);

	m_bufTX[m_txIDX]=(BYTE)(crc16&0x00FF); m_txIDX++;
	m_bufTX[m_txIDX]=(BYTE)((crc16>>8)&0x00FF); m_txIDX++;

									//поиск оборудования 1F	07	9F	AB	AC	FF	D5
									//включить  1F	0С	A0	FF	00	FF	FF	FF	00	00	05	E5
									//выключить 1F	06	A1	XX	CRC	CRC
									//читать    1А	08	A3	ХХ	00	00	CRC	CRC

	//Sleep(1);


//	if (m_Com->IsOpen())
	if (m_Com->st.on)
	{
		CString ss("");
		CString s("");

		for (int i=0; i<m_txIDX; i++)
		{
			s.Format(L"%02X ",m_bufTX[i]);
			ss+=s;
		}

		//m_CONSOLEC_Edit->SetWindowTextW(ss);

		//m_Com->Write(m_bufTX,m_txIDX);
		m_Com->SetTimeouts(500,500);
		return m_Com->PortNWrite(m_txIDX,m_bufTX);

	}

}

/*
Обработка принятого от мишени пакета
*/

TargetM_res CTargetM::RESPOND_proc(BYTE* buf, int len)
{

	    memcpy(m_bufRX,buf,len);
		m_rxIDX=len;

/*		
		if (len==15)//!!!
		{
			m_ADDR=31;

			buf[0]=0x1f;
			buf[1]=0x0f;
			buf[2]=0xa3;
			buf[3]=0x00;
			buf[4]=0x39;
			buf[5]=0x2a;
			buf[6]=0x03;
			buf[7]=0xb3;
			buf[8]=0xb8;
			buf[9]=0xd8;
			buf[10]=0x7b;
			buf[11]=0xe7;
			buf[12]=0x01;
			buf[13]=0xc5;
			buf[14]=0x81;
			
	

			unsigned short cr;

			cr=this->CRC16(buf,len-2);

			unsigned short ccc=cr;
		}
*/	

		//ClearForm();
	
	    //m_DISPLAY_Edit->SetWindowTextW(L"");

		CString ss("");
		CString s("");

		

/*
		for (int i=0; i<len; i++)
		{
			s.Format(L"%02X ",buf[i]);
			ss+=s;
		}
		m_CONSOLER_Edit->SetWindowTextW(ss);
*/

	int idx=0;

	TargetM_res res;
	memset(&res,0,sizeof(TargetM_res));
	res.code_res=0;
	
		if ((buf[0]==0)&&(buf[1]==0)&&(buf[2]==0))
		{
			res.code_res=-10;
			return res;
		}


//Байт 0 всегда адрес ответившей мишени
	res.addr=buf[idx]; idx++;


//Байт 1 всегда полная длина ответа включая CRC
	res.len=buf[idx]; idx++;


/*
//Проверяем длину
	if (res.len!=len)
	{
//Ошибка длины пакета
		res.code_res=-1;
		s.Format(L"ОШИБКА = ДЛИНА\r\n");
		m_Info+=s;
		m_INFO_Edit->SetWindowTextW(m_Info);
		StatusView(TargetM_state::TSTATE_len_err);
		return res;
	}
	
	s.Format(L"длина = %d\r\n",res.len);
	m_Info+=s;
*/


//Проверяем crc

	unsigned short  crc=CRC16(buf,len-2);
	unsigned short  ccrc=(((unsigned short)(buf[len-1]))<<8)|(((unsigned short)(buf[len-2]))<<0);

	if (crc!=ccrc)
	{
//Ошибка CRC
		res.code_res=-2;
		s.Format(L"ОШИБКА = CRC\r\n");
		m_Info+=s;
		m_INFO_Edit->SetWindowTextW(m_Info);

		StatusView(TargetM_state::TSTATE_crc_err);
		return res;
	}

	res.CRC=crc;

	s.Format(L"CRC = 0x%04X\r\n",res.CRC);
	m_Info+=s;

//Проверяем адрес
	if (res.addr!=m_ADDR)
	{
//Ошибка адреса
		res.code_res=-3;
		s.Format(L"ОШИБКА = АДРЕС\r\n");
		m_Info+=s;
		m_INFO_Edit->SetWindowTextW(m_Info);
		StatusView(TargetM_state::TSTATE_addr_err);
		return res;
	}


//Определяем тип ответа

	switch(len)
	{
		case 7:	
//Команда поиска оборудования проверяем код команды 9F	AB	AC // 9F	BB	CC

			/*
			if (buf[2]!=0x9F) {res.code_res=2; return res;}
			if (buf[3]!=0xBB) {res.code_res=2; return res;}
			if (buf[4]!=0xCC) {res.code_res=2; return res;}
			*/

			if (buf[2]!=0x9F) {res.code_res=2; return res;}
			if (buf[3]!=0xAB) {res.code_res=2; return res;}
			if (buf[4]!=0xAC) {res.code_res=2; return res;}

			res.cmd=TargetM_cmd::TCMD_SEEK;
			m_Info+=L"ответ = ПОИСК\r\n";
			StatusView(TargetM_state::TSTATE_cmd_seek);

			*TARGETSEEK=TRUE;

		break;
	


		case 6:	
//Команда включить-выключить мишень A0/A1
			if ((buf[2]!=0xA0)&&(buf[2]!=0xA1)) {res.code_res=2; return res;}

			if (buf[2]==0xA0) 
			{
				res.cmd=TargetM_cmd::TCMD_ON;
				m_Info+=L"ответ = ВКЛЮЧИТЬ\r\n";
				StatusView(TargetM_state::TSTATE_cmd_on);

				*TARGON=TRUE;

			}

			if (buf[2]==0xA1)
			{			
				res.cmd=TargetM_cmd::TCMD_OFF;
				m_Info+=L"ответ = ВЫКЛЮЧИТЬ\r\n";
				StatusView(TargetM_state::TSTATE_cmd_off);
			}

		break;

		case 5:	
//Команда включить-выключить мишень A0/A1
			if ((buf[2]!=0xA0)&&(buf[2]!=0xA1)) {res.code_res=2; return res;}

			if (buf[2]==0xA0) 
			{
				res.cmd=TargetM_cmd::TCMD_ON;
				m_Info+=L"ответ = ВКЛЮЧИТЬ\r\n";
				StatusView(TargetM_state::TSTATE_cmd_on);
				*TARGON=TRUE;
			}

			if (buf[2]==0xA1)
			{			
				res.cmd=TargetM_cmd::TCMD_OFF;
				m_Info+=L"ответ = ВЫКЛЮЧИТЬ\r\n";
				StatusView(TargetM_state::TSTATE_cmd_off);
			}

		break;


		case 15:	
//Команда чтения данных A3
		if (buf[2]!=0xA3) {res.code_res=2; return res;}
		res.cmd=TargetM_cmd::TCMD_READ;
//проверяем область попадания
		res.area=buf[8];
		s.Format(L"область = %d\r\n",res.area);
	    m_Info+=s;

		if (buf[8]==0xAD)
		{
			m_Info+=L"ответ = ОШИБКА КООРДИНАТЫ\r\n";
			StatusView(TargetM_state::TSTATE_read_err);
		}
		else
		{

//Номер пробоины
			res.gap=buf[3];
//			res.X=(unsigned short)(((unsigned short)(buf[5]))<<8)|(((unsigned short)(buf[4]))<<0);
//			res.Y=(unsigned short)(((unsigned short)(buf[7]))<<8)|(((unsigned short)(buf[6]))<<0);

			res.X=(long)(((unsigned short)(buf[4]))<<8)|(((unsigned short)(buf[5]))<<0);
	   		res.Y=(long)(((unsigned short)(buf[6]))<<8)|(((unsigned short)(buf[7]))<<0);

			if (m_EMULATEMODE==1)
			{
				res.X*=10;
				res.Y*=10;
			}

			res.tick =((unsigned int)buf[11])<<0;
			res.tick|=((unsigned int)buf[10])<<8;
			res.tick|=((unsigned int)buf[9])<<16;


			unsigned int total=res.tick;

			unsigned short hour=total/(60*60*10);
			total-=hour*(60*60*10);


			unsigned short min=total/(60*10);
			total-=min*(60*10);

			unsigned short sec=total/10;
			total-=sec*10;

			unsigned short dec=total;

			res.tick_hour=hour;
			res.tick_min=min;
			res.tick_sec=sec;
			res.tick_dec=dec;

			s.Format(L"номер = %d\r\n",res.gap);
			m_Info+=s;

			res.status=buf[12];

			s.Format(L"статус = %d\r\n",res.status);
			m_Info+=s;

			if (res.status==1)
			{
				m_Info+=L"ответ = ЧТЕНИЕ НОРМА\r\n";
				StatusView(TargetM_state::TSTATE_read_norm);
			}
			else
			{
				m_Info+=L"ответ = УПР. ВЫКЛ.\r\n";
				StatusView(TargetM_state::TSTATE_urespond_off);
			}
		}


		if (res.status==1)
		{

		    switch (res.area)
			{
				case 0:
				break;

				case 1:
					res.X*=-1;
				break;

				case 2:
				break;

				case 3:
				break;

				case 4:
				break;

				case 5:
					res.Y*=-1;
				break;

				case 6:
					res.Y*=-1;
				break;

				case 7:
					res.X*=-1; res.Y*=-1; 
				break;

				case 8:
					res.X*=-1; 
				break;

			}


//Помещаем результат в пулл
//В пулл кидаем только те ответы что содержат правильный номер пробоины

			if (res.gap==m_numGAP) 
			{
				//int idx=res.gap-1;

				int idx=this->m_numGAP-1;

				m_PULL.full=1;
				m_PULL.idx=idx;
				m_PULL.record[idx].act=1;
				memcpy(&m_PULL.record[idx].res,&res,sizeof(TargetM_res));
				
			}

//Пишем на дисплей
		CString display("");

		s.Format(L"\r\nЗапрос данных пробоина \t%d", m_numGAP);
		display+=s;

		if (res.gap==0)
		{
		 s.Format(L"\r\nНЕТ ДАННЫХ");
		 display+=s;
		}
		else
		{
			s.Format(L"\r\nОтметка времени\t\t%02d:%02d:%02d:%01d", res.tick_hour,res.tick_min,res.tick_sec,res.tick_dec);
			display+=s;

			s.Format(L"\r\nСтатус управления\t\t%02d", res.status);
			display+=s;

			s.Format(L"\r\nНомер пробоины\t\t%02d", res.gap);
			display+=s;

			s.Format(L"\r\nОбласть попадания\t\t%02d", res.area);
			display+=s;

			s.Format(L"\r\nКоордината X\t\t%.1f", (double) res.X/10);
			display+=s;

			s.Format(L"\r\nКоордината Y\t\t%.1f", (double) res.Y/10);
			display+=s;

			m_pGraph->DrawGap(res.X, res.Y, res.gap);
		}

		m_DISPLAY_Edit->SetWindowTextW(display);

			//if (m_Auto) 
			{

				if (m_numGAP<60)
				{
					if (m_PULL.record[m_PULL.idx].act==1)
					{
//Если даные пришли по запрошенной пробоине инктементируем
						if (res.gap==m_numGAP) m_numGAP++;
					}
				}
				else
				{
//Если уже 60 и данные пришли корректно то стоп цикла

					if (m_PULL.record[m_PULL.idx].act==1)
					{
						if (res.gap==m_numGAP)
						{
							m_STOP=TRUE;
							m_numGAP=1;
						}
					}
				}
			}
		}

        break;

		default:

			m_Info+=L"ответ = НЕ ОПОЗНАН\r\n";	
			StatusView(TargetM_state::TSTATE_err);

			
	}


//	m_ADDR=1;//!!!
//	Проверяем адрес ответившей мишени

//	s.Format(L"адрес = 0x%02X",res.addr);
//	m_Info+=s;
	
//	m_INFO_Edit->SetWindowTextW(m_Info);

	return res;

}


/*
Эмуляция принятого от мишени пакета
*/

int CTargetM::RESPOND_emul(BYTE* buf, TargetM_cmd cmd)
{
	BYTE b=0;
	BYTE gap=m_numGAP;
	unsigned short X=1234;
	unsigned short Y=5678;
	BYTE area=22;//0xAD;//7;
	unsigned int tick=123456;
	BYTE status=1;

	memset (buf,0,1024);
	int idx=0;

//Байт 0 адрес мишени
	buf[idx]=m_ADDR; idx++;

//Байт 1 длина пакета пока 0
	buf[idx]=0; idx++;

//Проверяем код операции
	switch (cmd)
	{

//Команда поиска мишени 9F	BB	CC
		case TCMD_SEEK:
			buf[idx]=0x9F; idx++;
			buf[idx]=0xBB; idx++;
			buf[idx]=0xCC; idx++;
        break;


//Команда включить мишень 1F	06	A0	XX	CRC	CRC
		case TCMD_ON:
			buf[idx]=0xA0; idx++;
			buf[idx]=0x00; idx++;
        break;
		
//Команда выключить мишень A1	XX
		case TCMD_OFF:
			buf[idx]=0xA1; idx++;
			buf[idx]=0x00; idx++;			
        break;

		

//Команда читать данные A3	
		case TCMD_READ:
//Код команды
			buf[idx]=0xA3; idx++;
//Номер пробоины
			
			buf[idx]=gap; idx++;

//Координата X
			
			buf[idx]=(BYTE)(X&0xFF); idx++;
			buf[idx]=(BYTE)((X>>8)&0xFF); idx++;

//Координата Y
			
			buf[idx]=(BYTE)(Y&0xFF); idx++;
			buf[idx]=(BYTE)((Y>>8)&0xFF); idx++;

//Область попадания
			
			buf[idx]=area; idx++;

//Время в тиках			

			b=(BYTE)(tick&0xFF);
			buf[idx]= b; idx++;
			b=(BYTE)((tick>>8)&0xFF);
			buf[idx] = b; idx++;
			b=(BYTE)((tick>>16)&0xFF);
			buf[idx]  = b; idx++;

//Стстус управления
			
			buf[idx]=status; idx++;

        break;

		default:
			AfxMessageBox(L"Ошибка эмуляции ответа: неопознанный код команды!");
        return 0;
	}
	
	buf[1]=idx+2;

	unsigned short crc=CRC16(buf,idx);

	buf [idx]=(BYTE)crc&0xFF; idx++;
	buf [idx]=(BYTE)((crc>>8)&0xFF); idx++;


	return idx;

}


/*
Вывод результата стрельбы
*/

void CTargetM::View(TargetM_res* res)
{
	CString s;

//Время стрельбы в тиках
	//s.Format(L"%d",res->tick);

	s.Format(L"%02d:%02d:%02d:%03d",res->tick_hour,res->tick_min,res->tick_sec,res->tick_dec);

	this->m_TIMETICKS_Edit->SetWindowTextW(s);

//X Y
	s.Format(L"%d",res->X);
	this->m_RESX_Edit->SetWindowTextW(s);

    s.Format(L"%d",res->Y);
	this->m_RESY_Edit->SetWindowTextW(s);

//Область попадания

	s.Format(L"%d",res->area);
	this->m_TAREA_Edit->SetWindowTextW(s);

//Номер пробоины

	s.Format(L"%d",res->gap);
	this->m_GAP_Edit->SetWindowTextW(s);


	m_INFO_Edit->SetWindowTextW(m_Info);


/*
	CString display("");
	s.Format(L"\r\nОтметка времени\t\t%02d:%02d:%02d:%01d", res->tick_hour,res->tick_min,res->tick_hour,res->tick_sec,res->tick_dec);
	display+=s;

	s.Format(L"\r\nСтатус управления\t\t%02d", res->status);
	display+=s;

	s.Format(L"\r\nНомер пробоины\t\t%02d", res->gap);
	display+=s;

	s.Format(L"\r\nОбласть попадания\t%02d", res->area);
	display+=s;

	s.Format(L"\r\nКоордината X\t\t%02d", res->X);
	display+=s;

	s.Format(L"\r\nКоордината Y\t\t%02d", res->Y);
	display+=s;

	m_DISPLAY_Edit->SetWindowTextW(display);
*/

}

void CTargetM::StatusView(TargetM_state state)
{
	switch(state)
	{
			case TSTATE_unknown:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(150,150,150));
				this->m_TSTATUS_Edit->SetWindowTextW(L"ОТКЛЮЧЕНО");
				break;
			}

			case TSTATE_urespond_on:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(0,250,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"УПР. ВКЛЮЧЕНО");
				break;
			}

			case TSTATE_urespond_off:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,250,0));
				//this->m_TSTATUS_Edit->SetWindowTextW(L"УПР. ВЫКЛЮЧЕНО");
				this->m_TSTATUS_Edit->SetWindowTextW(L"ОЖИДАНИЕ ДАННЫХ");
				break;
			}

			case TSTATE_cmd_seek:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(0,250,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"КОМАНДА ПОИСК");
				break;
			}

			case TSTATE_cmd_on:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(0,250,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"КОМАНДА ВКЛ.");
				break;
			}

			case TSTATE_cmd_off:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,250,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"КОМАНДА ВЫКЛ.");
				break;
			}

			case TSTATE_read_norm:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(0,250,0));
				//this->m_TSTATUS_Edit->SetWindowTextW(L"КОМАНДА ЧТЕНИЯ");
				this->m_TSTATUS_Edit->SetWindowTextW(L"ПОЛУЧЕНЫ ДАННЫЕ");
				break;
			}

			case TSTATE_read_err:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,0,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"ОШИБКА КООРДИНАТЫ");
				break;
			}

			case TSTATE_len_err:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,0,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"ОШИБКА ДЛИНЫ");
				break;
			}

			case TSTATE_addr_err:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,0,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"ОШИБКА АДРЕСА");
				break;
			}

			case TSTATE_crc_err:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,0,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"ОШИБКА CRC");
				break;
			}

			case TSTATE_err:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,0,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"ОШИБКА");
				break;
			}

			case TSTATE_noanswer:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(150,150,150));
				this->m_TSTATUS_Edit->SetWindowTextW(L"НЕТ ОТВЕТА");
				break;
			}

			case TSTATE_unknown_answer:
			{
				this->m_TSTATUS_Edit->SetColorBG(RGB(250,250,0));
				this->m_TSTATUS_Edit->SetWindowTextW(L"НЕИЗВЕСТНАЯ ОШИБКА");
				break;
			}
			
	}

}

void CTargetM::ClearForm(void)
{

	TargetM_res res;
	memset(&res,0,sizeof(TargetM_res));
	View(&res);

	//this->m_CONSOLEC_Edit->SetWindowText(L"");
	//this->m_CONSOLER_Edit->SetWindowText(L"");
	//this->m_INFO_Edit->SetWindowText(L"");
	//this->m_DISPLAY_Edit->SetWindowText(L"");
}

///////////////////////////////////////////CRC16//////////////////////////////////////////////////
//https://www.lammertbies.nl/comm/info/crc-calculation.html
//#define                 P_SICK      0x8005
//generating polynomial for MODBYS RTU
#define					  P_16        0xA001

//crc calculate table
unsigned short			crc_tab16[256];
BOOL					crc_tab16_init = FALSE;


//this function correct for dallas crc16 version
unsigned short CTargetM::CRC16(unsigned char* buf, int len)
{
		//start value of crc 
		unsigned short crc=0xffff;

		for (int i=0; i<len; i++)
		crc=update_crc_16 (crc, buf[i]);	

	return crc;
}


unsigned short CTargetM::update_crc_16(unsigned short crc, char c)
{
    unsigned short tmp, short_c;

    short_c = 0x00ff & (unsigned short) c;

    if ( ! crc_tab16_init )
		init_crc16_tab();

    tmp =  crc       ^ short_c;
    crc = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];
    return crc;

} /* update_crc_16 */

void CTargetM::init_crc16_tab(void)
{
	int i, j;
    unsigned short crc, c;

    for (i=0; i<256; i++) {

        crc = 0;
        c   = (unsigned short) i;

        for (j=0; j<8; j++) {

            if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ P_16;
            else                      crc =   crc >> 1;

            c = c >> 1;
        }

        crc_tab16[i] = crc;
    }

    crc_tab16_init = TRUE;

} /* init_crc16_tab */

/////////////////////////////////////////////////////////////////////

void CTargetM::ClearPULL(void)
{
	for (int i=0; i<60; i++)
	{
		m_PULL.full=0;
		m_PULL.idx=0;
		m_PULL.record[i].act=0;
		memset(&m_PULL.record[i].res,0,sizeof(TargetM_res));
	}
}


void CTargetM::DrawPull(void)
{

	if (m_PULL.full)
	{
		for (int i=0; i<60; i++)
		{
			if (m_PULL.record[i].act)
			{
				m_pGraph->DrawGap(m_PULL.record[i].res.X,m_PULL.record[i].res.Y,m_PULL.record[i].res.gap);
			}

		}
	}


	if (m_TestOver)
	{
		m_pGraph->DrawMdlGap(this->m_Test.m_STP.x, this->m_Test.m_STP.y);
		m_pGraph->DrawCtrlGap(this->m_Test.m_TPoint.x*100,this->m_Test.m_TPoint.y*100);


		//m_pGraph->DrawMdlGap();
		//m_pGraph->DrawCtrlGap();

		//m_pGraph->DrawTestCircle(this->m_pGraph->mdlX,this->m_pGraph->mdlY,tstR);
		//m_pGraph->DrawTestRect(this->m_pGraph->mdlX,this->m_pGraph->mdlY,tstH,tstW);

		m_pGraph->DrawTestCircle(this->m_Test.m_STP.x,this->m_Test.m_STP.y,this->m_Test.m_NCircleD*10/2);


		long x[4],y[4];

		for (int i=0; i<4; i++)
		{
			x[i]=this->m_PULL.record[i].res.X;
			y[i]=this->m_PULL.record[i].res.Y;
		}


		//m_pGraph->DrawTestRectEx(this->m_Test.m_RejectPointIdx,x,y,this->m_Test.m_STP.x,this->m_Test.m_STP.y,m_Test.m_Rect.h*10,m_Test.m_Rect.w*10);

		if (m_Test.m_Rect.h>0) m_pGraph->DrawTestRectCalculated(m_Test.p1,m_Test.p2,m_Test.p3,m_Test.p4);

		//!!!m_pGraph->DrawTestRect(this->m_Test.m_STP.x,this->m_Test.m_STP.y,m_Test.m_Rect.h*10,m_Test.m_Rect.w*10);

	}

	
}


BOOL CTargetM::AskTargetRequest(TargetM_cmd cmd)
{

	DWORD tout=500;

//	TCMD_SEEK,
//	TCMD_ON,
//	TCMD_OFF,
//	TCMD_READ

	double tonebyte=(1000000000/19200);


		tonebyte*=10;
		
		tonebyte/=1000000;
		
		tonebyte*=2;


	BYTE buf[128];
	int n=15;

	switch (cmd)
	{

		case TCMD_SEEK:
			n=7;
			tout=(DWORD)(7+1)*tonebyte;
		break;

		case TCMD_ON:
			n=6;
			tout=(DWORD)(6+1)*tonebyte;
		break;

		case TCMD_OFF:
			n=6;
			tout=(DWORD)(6+1)*tonebyte;
		break;

		case TCMD_READ:
			n=15;
			tout=(DWORD)(15+1)*tonebyte;
		break;
	}

	m_Com->SetTimeouts(500,500);

	if ((cmd==TCMD_ON)||(cmd==TCMD_OFF))
	{
	
		if (m_Com->PortNRead(5,buf)==TRUE)
		{

			if (buf[1]==5)
			{
				RESPOND_proc(buf,5);
				return TRUE;
			}
			else
			{

			}

			if (buf[1]==6)
			{
				if (m_Com->PortNRead(1,&buf[5])==TRUE)
				{
					RESPOND_proc(buf,6);
					return TRUE;
				}
			}
		}

	}
	else
	{
		if (m_Com->PortNRead(n,buf)==TRUE)
		{
			RESPOND_proc(buf,n);
			return TRUE;
		}
		else
		{
			/*
			CString s("");
			CString ss("");

			while(m_Com->PortNRead(1,buf)==TRUE)
			{
				s.Format(L"%02X ",buf[0]);
				ss+=s;
			}

			this->m_CONSOLER_Edit->SetWindowText(ss);
			*/

			return FALSE;
		}
	}

	return FALSE;
}


int CTargetM::ResCalculate(int test)
{

	this->m_Test.ResCalculate();

//Отображаем на графике

	this->m_Test.DisplayTestRes();	
    m_TestOver=TRUE;
	this->DrawPull();
	
	return 0;
}


BYTE CTargetM::InsideCyrcle(long x, long y, long cx, long cy, long cr)
{

//Считаем расстояние от центра окружности до заданной точки

	if (cr==0) return 0;

	//cr*=this->m_pGraph->m_TARGSCALEFACTOR;

//Радиус окружности приводим к десятым миллиметра
	cr*=10;

	double dx=x-cx;
	double dy=y-cy;

    double dr=sqrt((dx*dx)+(dy*dy));

	if (dr<cr)  return 1;
	else		return 0;
}


BYTE CTargetM::InsideRect(long x, long y)
{

	if (tstH==0) return 0;

	//long upx=this->mid

/*
	double mdlX=0;
	double mdlY=0;

	long tstH;
	long tstW;
*/

	//long top=(long)mdlY+(tstH*(this->m_pGraph->m_TARGSCALEFACTOR))/2;
	//long bottom=(long)mdlY-(tstH*(this->m_pGraph->m_TARGSCALEFACTOR))/2;

	//long left=(long)mdlX-(tstW*(this->m_pGraph->m_TARGSCALEFACTOR))/2;
	//long right=(long)mdlX+(tstW*(this->m_pGraph->m_TARGSCALEFACTOR))/2;

//Длина и ширин прямоугольника приводится к десятым миллиметра
	long top=(long)mdlY+(tstH*10)/2;
	long bottom=(long)mdlY-(tstH*10)/2;

	long left=(long)mdlX-(tstW*10)/2;
	long right=(long)mdlX+(tstW*10)/2;


	if (x<left) return 0;
	if (x>right) return 0;
	if (y<bottom) return 0;
	if (y>top) return 0;

	return 1;
}


long CTargetM::DispersRang_Side(void)
{

	long rang=0;

//Массиы ккординат по x
	long xx[60];
	memset(xx,0,sizeof(long));

	long tmp[60];
	memset(tmp,0,sizeof(long));


//Копируем сюда значимые координаты по x
	for (int i=0; i<gapsInTotal; i++)
	{
		xx[i]=this->m_PULL.record[i].res.X;
	}

//ищем первый минимум
	
	
	return 0;
}


long CTargetM::DisperseRang_Hight(void)
{
	return 0;
}


long CTargetM::DisperseRang_Mdl(void)
{
	return 0;
}


int CTargetM::SetTestNгmber(int test)
{

	if ((test<1)||(test>9)) test=1;

	this->m_TEST_NUmber=test;
	this->ctrlR_set=50;

	switch(test)
	{
		case 1:
		
			this->m_TESTPAR_gapsnum=4;
			tstR=0;
			tstH=120;
			tstW=100;
			ctrlR_set=50;

		break;

		case 2:
			this->m_TESTPAR_gapsnum=10;
			tstR=200/2;
			tstH=0;
			tstW=0;
		break;

		case 3:
			this->m_TESTPAR_gapsnum=4;
			tstR=100/2;
			tstH=0;
			tstW=0;
		break;

		case 4:
			this->m_TESTPAR_gapsnum=10;
			tstR=200/2;
			tstH=0;
			tstW=0;
			ctrlR_set=100;
		break;

		case 5:
			this->m_TESTPAR_gapsnum=60;
			tstR=0;
			tstH=0;
			tstW=0;
		break;
		
		case 6:
			this->m_TESTPAR_gapsnum=10;
			tstR=0;
			tstH=160;
			tstW=140;
		break;

		case 7:
			this->m_TESTPAR_gapsnum=10;
			tstR=0;
			tstH=120;
			tstW=100;
		break;

		case 8:
			this->m_TESTPAR_gapsnum=10;
			tstR=0;
			tstH=140;
			tstW=160;
		break;

		case 9:
			this->m_TESTPAR_gapsnum=20;
			tstR=0;
			tstH=120;
			tstW=100;
		break;

	}

	CString s("");
	CString ss("");

	int idx=0;

	s.Format(L" Номер испытания = %d\r\n",m_TEST_NUmber);
	ss+=s; this->m_TEST_RESULT_Strings[idx]=s; idx++; 

	s.Format(L" Выстрелов в испытании = %d\r\n",m_TESTPAR_gapsnum);
	ss+=s;  this->m_TEST_RESULT_Strings[idx]=s; idx++;
	if (tstR>0)
	{
		s.Format(L" Критерий = круг R=%d\r\n",tstR);
		ss+=s;  this->m_TEST_RESULT_Strings[idx]=s; idx++;
	}

	if (tstH>0)
	{
		s.Format(L" Критерий = прямоугольник %dх%d\r\n",tstH,tstW);
		ss+=s;  this->m_TEST_RESULT_Strings[idx]=s; idx++;
	}

	if ((tstR==0)&&(tstH==0))
	{
     s=" ";
	 this->m_TEST_RESULT_Strings[idx]=s; idx++;

	}

     s=" ";
	 this->m_TEST_RESULT_Strings[idx]=s; idx++;

	this->m_TSETRESULT_Str=ss;

	this->m_TESTPAR_Edit->SetWindowTextW(ss);

	return 0;
}



//---------------------------------------------------------------------------------------------------------------------------------------

CTestM::CTestM()
{
	ClearRes();
	ClearPar();

	m_RectAngle=0;
}

CTestM::~CTestM()
{

}


int CTestM::ClearRes(void)
{
	BOOL m_TestOver=FALSE;

	for (int i=0; i<60; i++)
	{
		//this->m_TPar_strs[i]="";
		this->m_TRes_strs[i]="";
	}

	int m_4836=0;

//Общее число пробоин в мишени
	m_GAll=0;
//Подсчитанное число пробоин внутри прямоугольника
	m_GinRect=0;
//Подсчитанное число пробоин внутри круга
	m_GinCircle=0;
//Подсчитанное отклонение от КТ
	m_TpointR=0;

//Подсчитанное значение средней полосы рассеивания по высоте
	m_BandH=0;
//ПОдсчитанное значение средней полосы рассеивания по боковому направлению
	m_BandW=0;
//Подсчитанное значение эквивалентной полосы рассеивания
	m_BandE=0;

//Подсчитанные координаты СТП
	m_STP.x=0;
	m_STP.y=0;

	m_Itog=FALSE;

	Reset();

	return 0;
}


int CTestM::ClearPar(void)
{

//Номер испытания
	m_TNum=0;
//Число выстрелов в испытании
	m_NGaps=0;
//Параметры тестового прямоугольника
	m_Rect.h=0;
	m_Rect.w=0;
//Диаметр тестовой окружности	
	m_NCircleD=0;
//Контрольная точка
	m_TPoint.x=0;
	m_TPoint.y=0;
//Заданное отклонение от КТ
	m_NTpointR=0;
//Заданное число пробоин внутри прямоугольника
	m_NGinRect=0;
//Заданное число пробоин внутри круга
	m_NGinCircle=0;
//Заданное значение средней полосы рассеивания по высоте
	m_NBandH=0;
//Заданное значение средней полосы рассеивания по боковому направлению
	m_NBandW=0;
//Заданное значение эквивалентной полосы рассеивания
	m_NBandE=0;
//Комментарий - номер пункта нормативных требований
    m_Doc="";	
//Прочие комментарии
	m_Mark="";

 m_N4836=FALSE;

	return 0;
}


int CTestM::SaveIniPar(void)
{

	TCHAR SECT[1024];
	wsprintf(SECT,L"TEST%d",this->m_TNum);
//Число выстрелов в испытании
	this->m_Ini->SaveIniPar(SECT, L"GAPSINTEST",INT_PARAMETER_FORMAT,&m_NGaps);
//Параметры тестового прямоугольника
	this->m_Ini->SaveIniPar(SECT, L"TESTRECT_H",INT_PARAMETER_FORMAT,&m_Rect.h);
	this->m_Ini->SaveIniPar(SECT, L"TESTRECT_W",INT_PARAMETER_FORMAT,&m_Rect.w);
//Диаметр тестовой окружности	
	this->m_Ini->SaveIniPar(SECT, L"TESTCIRCLE_D",INT_PARAMETER_FORMAT,&m_NCircleD);
//Контрольная точка
	this->m_Ini->SaveIniPar(SECT, L"TESTPOINT_X",INT_PARAMETER_FORMAT,&m_TPoint.x);
	this->m_Ini->SaveIniPar(SECT, L"TESTPOINT_Y",INT_PARAMETER_FORMAT,&m_TPoint.y);
//Заданное отклонение от КТ
	this->m_Ini->SaveIniPar(SECT, L"TESTPOINT_R",INT_PARAMETER_FORMAT,&m_NTpointR);
//Заданное число пробоин внутри прямоугольника
	this->m_Ini->SaveIniPar(SECT, L"TEST_INRECT",INT_PARAMETER_FORMAT,&m_NGinRect);
//Заданное число пробоин внутри круга
	this->m_Ini->SaveIniPar(SECT, L"TEST_INCIRCLE",INT_PARAMETER_FORMAT,&m_NGinCircle);
//Заданное значение средней полосы рассеивания по высоте
	this->m_Ini->SaveIniPar(SECT, L"TEST_BANDH",INT_PARAMETER_FORMAT,&m_NBandH);
//Заданное значение средней полосы рассеивания по боковому направлению
	this->m_Ini->SaveIniPar(SECT, L"TEST_BANDW",INT_PARAMETER_FORMAT,&m_NBandW);
//Заданное значение эквивалентной полосы рассеивания
	this->m_Ini->SaveIniPar(SECT, L"TEST_BANDE",INT_PARAMETER_FORMAT,&m_NBandE);

//Комментарий - номер пункта нормативных требований
		this->m_Ini->SaveIniPar(SECT, L"TEST_DOC",STR_PARAMETER_FORMAT,&m_Doc);
//Прочие комментарии
		this->m_Ini->SaveIniPar(SECT, L"TEST_MARK",STR_PARAMETER_FORMAT,&m_Mark);

		this->m_Ini->SaveIniPar(SECT, L"TEST_4836",INT_PARAMETER_FORMAT,(int*)&m_N4836);

	return 0;
}


int CTestM::LoadIniPar(int num)
{

	
	this->ClearPar();

	this->m_TNum=num;


	TCHAR SECT[1024];
	wsprintf(SECT,L"TEST%d",this->m_TNum);
//Число выстрелов в испытании
	this->m_Ini->GetIniPar(SECT, L"GAPSINTEST",INT_PARAMETER_FORMAT,&m_NGaps);
//Параметры тестового прямоугольника
	this->m_Ini->GetIniPar(SECT, L"TESTRECT_H",INT_PARAMETER_FORMAT,&m_Rect.h);
	this->m_Ini->GetIniPar(SECT, L"TESTRECT_W",INT_PARAMETER_FORMAT,&m_Rect.w);
//Диаметр тестовой окружности	
	this->m_Ini->GetIniPar(SECT, L"TESTCIRCLE_D",INT_PARAMETER_FORMAT,&m_NCircleD);
//Контрольная точка
	this->m_Ini->GetIniPar(SECT, L"TESTPOINT_X",INT_PARAMETER_FORMAT,&m_TPoint.x);
	this->m_Ini->GetIniPar(SECT, L"TESTPOINT_Y",INT_PARAMETER_FORMAT,&m_TPoint.y);
//Заданное отклонение от КТ
	this->m_Ini->GetIniPar(SECT, L"TESTPOINT_R",INT_PARAMETER_FORMAT,&m_NTpointR);
//Заданное число пробоин внутри прямоугольника
	this->m_Ini->GetIniPar(SECT, L"TEST_INRECT",INT_PARAMETER_FORMAT,&m_NGinRect);
//Заданное число пробоин внутри круга
	this->m_Ini->GetIniPar(SECT, L"TEST_INCIRCLE",INT_PARAMETER_FORMAT,&m_NGinCircle);
//Заданное значение средней полосы рассеивания по высоте
	this->m_Ini->GetIniPar(SECT, L"TEST_BANDH",INT_PARAMETER_FORMAT,&m_NBandH);
//Заданное значение средней полосы рассеивания по боковому направлению
	this->m_Ini->GetIniPar(SECT, L"TEST_BANDW",INT_PARAMETER_FORMAT,&m_NBandW);
//Заданное значение эквивалентной полосы рассеивания
	this->m_Ini->GetIniPar(SECT, L"TEST_BANDE",INT_PARAMETER_FORMAT,&m_NBandE);

//Комментарий - номер пункта нормативных требований
		this->m_Ini->GetIniPar(SECT, L"TEST_DOC",STR_PARAMETER_FORMAT,&m_Doc);
//Прочие комментарии
		this->m_Ini->GetIniPar(SECT, L"TEST_MARK",STR_PARAMETER_FORMAT,&m_Mark);

		this->m_Ini->GetIniPar(SECT, L"TEST_4836",INT_PARAMETER_FORMAT,(int*)&m_N4836);


	return 0;
}


void CTestM::DisplayTestPar(void)
{
	int idx=0;
	CString s;
	CString ss;

	for (int i=0; i<60; i++)
	{
		this->m_TPar_strs[i]="";
		//this->m_TRes_strs[i]="";
	}


	s.Format(L"Номер испытания %d",this->m_TNum);
	this->m_TPar_strs[idx]=s; idx++; // ss+=s; ss+="\r\n";


	s.Format(L"%s",this->m_Mark);
	this->m_TPar_strs[idx]=s; idx++; // ss+=s; ss+="\r\n";

	s.Format(L"Число выстрелов = %d",this->m_NGaps);
	this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";

	s.Format(L"КТ:   x = %d y = %d",this->m_TPoint.x, m_TPoint.y);
	this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";

	if (this->m_NTpointR)
	{
		s.Format(L"Допустимое отклонение СТП от КТ = %d",this->m_NTpointR);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_NCircleD>0)
	{
		s.Format(L"Диаметр круга = %d",this->m_NCircleD);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_Rect.h>0)
	{
		s.Format(L"Прямоугольник: H = %d W = %d",this->m_Rect.h,this->m_Rect.w);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_NGinCircle)
	{
		s.Format(L"Пробоин в круге >= %d",m_NGinCircle);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_NGinRect)
	{
		s.Format(L"Пробоин в прямоугольнике >= %d",m_NGinRect);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_NBandH)
	{
		s.Format(L"Свср <= %d",this->m_NBandH);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_NBandW)
	{
		s.Format(L"Сбср <= %d",this->m_NBandW);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_NBandE)
	{
		s.Format(L"Сэкв <= %d",this->m_NBandE);
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}

	if (this->m_N4836)
	{
		s.Format(L"Вхождение в габарит мишени 48х36");
		this->m_TPar_strs[idx]=s; idx++; ss+=s; ss+="\r\n";
	}


	this->m_DisplayPar->SetWindowTextW(ss);


}


void CTestM::DisplayTestRes(void)
{
	CString ss(""),s("");
	int idx=0;

	this->m_Itog=TRUE;

	//s.Format(L"Номер испытания %d",this->m_TNum);
	//ss+=s; ss+="\r\n";
	//this->m_TRes_strs[idx]=s; idx++;


	s.Format(L"Всего пробоин = %d", this->m_GAll);
	ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;

	s.Format(L"КТ: x = %d  y = %d", this->m_TPoint.x*10, this->m_TPoint.y*10);
	ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;

	s.Format(L"СТП: x = %.1f  y = %.1f  r = %.1f", (double)this->m_STP.x/10, (double)this->m_STP.y/10, (double)this->m_STP_R/10);
	ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;


	if (m_NTpointR>0)
	{
		if (m_TpointR<this->m_NTpointR*100)
		{
			s.Format(L"Откл. СТП = %.1f (<%.1f) НОРМА", (double)this->m_TpointR/10, double(m_NTpointR*10) );
		}
		else
		{
			s.Format(L"Откл. СТП = %.1f (>%.1f) брак", (double)this->m_TpointR/10, double(m_NTpointR*10) );
			m_Itog=FALSE;
			this->m_Res.stpdevN=0;
			this->m_Res.resN=0;
		}
	}

	ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;

	if ((this->m_Rect.h)&&(m_NGinRect>0))
	{
		if ((this->m_GinRect>=this->m_NGinRect)||((this->m_GinRect>=this->m_NGinRect-1)&&(this->m_RejectPointIdx>=0)))
		{
			s.Format(L"В прямоуг. = %d (=%d) НОРМА", this->m_GinRect, this->m_NGinRect);

			if (m_NGinRect==4)
			{
				if (this->m_RejectPointIdx>=0)
				{
					s.Format(L"В прямоуг. = %d (=%d+1 оторвавш.) НОРМА", this->m_GinRect, this->m_NGinRect-1);
				}
			}

		}
		else
		{
			s.Format(L"В прямоуг. = %d (<%d) брак", this->m_GinRect, this->m_NGinRect);
			m_Itog=FALSE;
		}

		ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;
	}

	if ((this->m_NCircleD)&&(m_NGinCircle>0))
	{	
		if ((this->m_GinCircle>=this->m_NGinCircle)||((this->m_GinRect>=this->m_NGinCircle-1)&&(this->m_RejectPointIdx>=0)))
		{
			s.Format(L"В круге = %d (=%d) НОРМА", this->m_GinCircle, this->m_NGinCircle);

			if (m_NGinCircle==4)
			{
				if (this->m_RejectPointIdx>=0)
				{
					s.Format(L"В круге = %d (=%d+1 оторвавш.) НОРМА", this->m_GinCircle, this->m_NGinCircle);
				}
			}


		}
		else
		{
			s.Format(L"В круге = %d (<%d) брак", this->m_GinCircle, this->m_NGinCircle);
			m_Itog=FALSE;
		}

		ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;
	}

	if (this->m_NBandH)
	{
		if (this->m_BandH<=this->m_NBandH*100)
		{
			s.Format(L"Св ср = %.1f (<=%.1f) НОРМА",(double) this->m_BandH/10, (double) this->m_NBandH*10);
		}
		else
		{
			s.Format(L"Св ср = %.1f (>%.1f) брак",(double) this->m_BandH/10, (double) this->m_NBandH*10);
			m_Itog=FALSE;
		}		

		ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;
	}

	if (this->m_NBandW)
	{
		if (this->m_BandW<=this->m_NBandW*100)
		{
			s.Format(L"Сб ср = %.1f (<=%.1f) НОРМА",(double) this->m_BandW/10, (double) this->m_NBandW*10);
		}
		else
		{
			s.Format(L"Сб ср = %.1f (>%.1f) брак",(double) this->m_BandW/10, (double) this->m_NBandW*10);
			m_Itog=FALSE;
		}

		ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;
	}

	if (this->m_NBandE)
	{
		if (m_BandE<=m_NBandE*100)
		{
			s.Format(L"Сэкв = %.1f (<=%.1f) НОРМА",(double) this->m_BandE/10, (double) this->m_NBandE*10);
		}
		else
		{
			s.Format(L"Сэкв = %.1f (>%.1f) брак",(double) this->m_BandE/10, (double) this->m_NBandE*10);
			m_Itog=FALSE;
		}

		ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;
	}

	if (this->m_N4836)
	{
		if ((abs(this->m_STP.x)<=36*100/2)&&(abs(this->m_STP.y)<=48*100/2))
		{
			this->m_4836=1;
			s.Format(L"СТП в в габарите 48х36 НОРМА");
		}
		else
		{
			this->m_4836=0;
			s.Format(L"СТП за габаритом 48х36 брак");
		}

		ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;
	}


	s.Format(L" ");
	ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;

	if (m_Itog==TRUE) s.Format(L"итог: НОРМА");
	else  s.Format(L"итог: брак");
	
	ss+=s; ss+="\r\n"; this->m_TRes_strs[idx]=s; idx++;

	this->m_DisplayRes->SetWindowText(ss);

}


int CTestM::ResCalculate(void)
{


//Определяем имеющееся число пробоин

	for (int i=0; i<60; i++)	
	{
		if (this->m_PULL->record[i].res.gap!=0) this->m_GAll++;
	}
//Считаем среднюю точку попадания

	double leftX=0xFFFFFF;
	double lowY=0xFFFFFF;

//Ищем самую левую и самую нижноо координаты
	for (int i=0; i<60; i++)
	{

		if (this->m_PULL->record[i].res.gap!=0)
		{
			if (this->m_PULL->record[i].res.X<leftX)  leftX=this->m_PULL->record[i].res.X;
			if (this->m_PULL->record[i].res.Y<lowY)   lowY=this->m_PULL->record[i].res.Y;
		}

	}

//Вычисляес среднее расстояние по X Y

	double sumX=0;
	double sumY=0;

	double difX=0;
	double difY=0;

	for (int i=0; i<60; i++)
	{

		if (this->m_PULL->record[i].res.gap!=0)
		{
			difX+=this->m_PULL->record[i].res.X-leftX;
			difY+=this->m_PULL->record[i].res.Y-lowY;
		}

	}


	double midX=difX/m_GAll;
	double midY=difY/m_GAll;

	midX+=leftX;
	midY+=lowY;

	this->m_STP.x=(long)midX;
	this->m_STP.y=(long)midY;


//Проверяем на предмет оторвавшейся точки

//Сбрасываем номер оторвавшейся точки
	m_RejectPointIdx=-1;

	if (m_GAll==4)
	{

//Считаем все отклонения от средней точки
		double stpr[4];

		for (int i=0; i<4; i++)
		{

			double drx=midX-this->m_PULL->record[i].res.X;
			double dry=midY-this->m_PULL->record[i].res.Y;

			stpr[i]=sqrt((drx*drx)+(dry*dry));
		}

//Находим максимум

		int dmaxidx=0;

		for (int i=0; i<4; i++)
		{
			if (stpr[i]>stpr[dmaxidx]) dmaxidx=i;
		}

		double rmax=stpr[dmaxidx];

//Считаем среднее отклонение от СТП

		double rmid=0;
		for (int i=0; i<4; i++)
		{
			if (i!=dmaxidx) rmid+=stpr[i];
		}

		rmid/=3;

//Если дальняя точка в 2 раза дальше среднего отклонения ее не учитываем при расчете СТП

		//if ((rmax/rmid)>2)
		if (rmax>rmid)
		{
			
//Считаем СТП для трех точек

//////////////////////////////////////////////////////////////////////////////////////////

		double leftX1=0xFFFFFF;
		double lowY1=0xFFFFFF;

//Ищем самую левую и самую нижноо координаты
		for (int i=0; i<60; i++)
		{

			if (this->m_PULL->record[i].res.gap!=0)
			{
				if (i!=dmaxidx)
				{
					if (this->m_PULL->record[i].res.X<leftX)  leftX1=this->m_PULL->record[i].res.X;
					if (this->m_PULL->record[i].res.Y<lowY)   lowY1=this->m_PULL->record[i].res.Y;
				}
			}

		}

//Вычисляеь среднее расстояние по X Y

				double sumX1=0;
				double sumY1=0;

				double difX1=0;
				double difY1=0;

				for (int i=0; i<60; i++)
				{

					if (this->m_PULL->record[i].res.gap!=0)
					{
						if (i!=dmaxidx)
						{
							difX1+=this->m_PULL->record[i].res.X-leftX1;
							difY1+=this->m_PULL->record[i].res.Y-lowY1;
						}
					}

				}


				double midX1=difX1/(m_GAll-1);
				double midY1=difY1/(m_GAll-1);

				midX1+=leftX1;
				midY1+=lowY1;

//считаем максимум отклонения от новой СТП (для 3 точек)

				double dr1=0;
				double drmax1=0;
				double dx1=0;
				double dy1=0;

				for (int i=0; i<60; i++)
				{
					if (this->m_PULL->record[i].res.gap!=0)
					{
						if (i!=dmaxidx)
						{
							dx1=midX1-(double)this->m_PULL->record[i].res.X;
							dy1=midY1-(double)this->m_PULL->record[i].res.Y;
							dr1=sqrt((dx1*dx1)+(dy1*dy1));
							if (dr1>drmax1) drmax1=dr1;
						}
					}

				}


//Считаем отклонение оторвавшейся точки от новой СТП

				//dx1=midX1-(double)this->m_PULL->record[dmaxidx].res.X;
				//dy1=midY1-(double)this->m_PULL->record[dmaxidx].res.Y;

				dx1=midX-(double)this->m_PULL->record[dmaxidx].res.X;
				dy1=midY-(double)this->m_PULL->record[dmaxidx].res.Y;

				dr1=sqrt((dx1*dx1)+(dy1*dy1));

//Переносим СТП из расчета на 3 точки

				double dr2=dr1/drmax1;

				//if ((dr1/drmax1)>2.5)
				if (dr2>2.5)
				{
					this->m_STP.x=(long)midX1;
					this->m_STP.y=(long)midY1;
//Фиксируем номер оторвавшейся точки
					m_RejectPointIdx=dmaxidx;

//Если одна точка оторвалась тест идет по 3 точкам
					//this->m_NGinRect=3;
					//this->m_NGinCircle=3;
				}
				else
				{
//Если оторвавшихся нет тест идет по 4 точкам
					//this->m_NGinRect=4;
					//this->m_NGinCircle=4;
				}
			
//Считаем результат испытания по СТП	
					


//////////////////////////////////////////////////////////////////////////////////////////


		}


		
	}



//Считаем сколько точек вошло в тестовую окружность

	if (this->m_NGinCircle>0)
	{
		for (int i=0; i<60; i++)
		{

			if (this->m_PULL->record[i].res.status)
			{
				if (this->m_NCircleD>0)
				{
					if (this->m_PULL->record[i].res.inCyrcle=this->TInsideCyrcle(this->m_PULL->record[i].res.X,this->m_PULL->record[i].res.Y,this->m_STP.x,this->m_STP.y,this->m_NCircleD*100/2))
					{
						this->m_GinCircle++;
					}
				}
			}


		}
	}


//Считаем сколько точек вошло в тестовый прямоугольник


//////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (this->m_NGinRect>0)
	{
		TestRectCalculate(&p1,&p2,&p3,&p4);
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//Считаем отклонение от контрольной точки

	double dx=(this->m_TPoint.x*100)-m_STP.x;
	double dy=(this->m_TPoint.y*100)-m_STP.y;

    this->m_TpointR=(long)sqrt((dx*dx)+(dy*dy));

//	if (this->m_TpointR<=this->m_NTpointR) this->m_Res.stpdevN=1;

//Расстояние СТП от центра мишени

	dx=(double)m_STP.x;
	dy=(double)m_STP.y;

	this->m_STP_R=(long)sqrt((dx*dx)+(dy*dy));

//Серединная полоса рассеивания по вертикали

//Временный буфер
	long tmp[60];
	memset(tmp,0,sizeof(long)*60);
//Забираем данные по Y
	for (int i=0; i<this->m_GAll; i++) tmp[i]=this->m_PULL->record[i].res.Y;

//Сортируем
	for (int i=0; i<this->m_GAll; i++)
	{
		long min=tmp[i];
		for (int j=i+1; j<this->m_GAll; j++)
		{
			if (tmp[j]<min)
			{
				min=tmp[j];
				long t=tmp[i];
				tmp[i]=tmp[j];
				tmp[j]=t;
				
			}
		}
	}

	if (this->m_GAll>=10)
	{
		this->m_BandH=abs(tmp[0+3]-tmp[this->m_GAll-3]);
	}
	else this->m_BandH=0;


//Серединная полоса рассеивания по боковому направлению

//Временный буфер

	memset(tmp,0,sizeof(long)*60);
//Забираем данные по Y
	for (int i=0; i<this->m_GAll; i++) tmp[i]=this->m_PULL->record[i].res.X;

//Сортируем
	for (int i=0; i<this->m_GAll; i++)
	{
		long min=tmp[i];
		for (int j=i+1; j<this->m_GAll; j++)
		{
			if (tmp[j]<min)
			{
				min=tmp[j];
				long t=tmp[i];
				tmp[i]=tmp[j];
				tmp[j]=t;
				
			}
		}
	}

	if (this->m_GAll>=10)
	{
		this->m_BandW=abs(tmp[0+3]-tmp[this->m_GAll-3]);
	}
	else this->m_BandW=0;

//Эквивалентная серединная полоса рассеивания

	this->m_BandE=(long)sqrt((double)this->m_BandH*(double)this->m_BandW);


//Подбиваем итоги
//Отклонение СТП
	if (this->m_NTpointR)
	{
		if (this->m_TpointR<=this->m_NTpointR*100) this->m_Res.stpdevN=1;
		else this->m_Res.resN=0;
	}

//Попадание в прямоугольник
	if (this->m_NGinRect)
	{
		int ninrect=this->m_NGinRect;
		if (ninrect==4)
		{
			if (this->m_RejectPointIdx>=0) ninrect=3;
		}
		if (this->m_GinRect>=ninrect) this->m_Res.inrectN=1;
		else this->m_Res.resN=0;
	}


//ПОпадание в круг
	if (this->m_NGinCircle)
	{
		int nincircle=this->m_NGinCircle;
		if (nincircle==4)
		{
			if (this->m_RejectPointIdx>=0) nincircle=3;
		}
		if (this->m_GinCircle>=nincircle) this->m_Res.incircleN=1;
		else this->m_Res.resN=0;
	}

//Полоса рассеивания

	if (m_NBandW)
	{
		if (m_BandW<=m_NBandW*100)   this->m_Res.bandwN=1;
		else					 this->m_Res.resN=0;
	}

	if (m_NBandH)
	{
		if (m_BandH<=m_NBandH*100)   this->m_Res.bandhN=1;
		else					 this->m_Res.resN=0;
	}

	if (m_NBandE)
	{
		if (m_BandE<=m_NBandE*100)   this->m_Res.bandeN=1;
		else					 this->m_Res.resN=0;
	}


	if (this->m_N4836)
	{
		if ((abs(this->m_STP.x)<=36*100/2)&&(abs(this->m_STP.y)<=48*100/2))
		{
			this->m_4836=1;
			this->m_Res.m3846N=1;
		}
		else
		{
			this->m_4836=0;
			this->m_Res.resN=0;
		}
	}


	return 0;
}


int CTestM::TInsideCyrcle(long x, long y, long cx, long cy, long cr)
{

//Считаем расстояние от центра окружности до заданной точки

	if (cr==0) return 0;

	//cr*=this->m_pGraph->m_TARGSCALEFACTOR;

//Радиус окружности приводим к десятым миллиметра

	double dx=x-cx;
	double dy=y-cy;

    double dr=sqrt((dx*dx)+(dy*dy));

	if (dr<cr)  return 1;
	else		return 0;
}


int CTestM::TInsideRect(long x, long y, long cx, long cy, long h, long w)
{

	if (h==0) return 0;

//Длина и ширин прямоугольника приводится к десятым миллиметра
	long top=(long)cy+(h)/2;
	long bottom=(long)cy-(h)/2;

	long left=(long)cx-(w)/2;
	long right=(long)cx+(w)/2;

	if (x<left) return 0;
	if (x>right) return 0;
	if (y<bottom) return 0;
	if (y>top) return 0;

	return 1;
}


int CTestM::TestRectCalculate(CPoint* p1,CPoint* p2,CPoint* p3,CPoint* p4)
{

//Ставим точки на тестовый прямоугольник относительно СТП

	p1->x=this->m_STP.x-this->m_Rect.w*100/2;
	p2->x=this->m_STP.x+this->m_Rect.w*100/2;

	p3->x=p2->x;
	p4->x=p1->x;

	p1->y=this->m_STP.y+this->m_Rect.h*100/2;
	p2->y=p1->y;

	p3->y=this->m_STP.y-this->m_Rect.h*100/2;
	p4->y=p3->y;

//#define M_PI           3.14159265358979323846  /* pi */ 

	double ang=0;//m_RectAngle;

//	ang*=M_PI;
//	ang/=180;

	CPoint stp;
	stp.x=m_STP.x;
	stp.y=m_STP.y;

	int imax=0;
	double angmax=0;

	double mstep=1;

	CPoint pp1,pp2,pp3,pp4;

	pp1.x=p1->x;
	pp2.x=p2->x;
	pp3.x=p3->x;
	pp4.x=p4->x;

	pp1.y=p1->y;
	pp2.y=p2->y;
	pp3.y=p3->y;
	pp4.y=p4->y;

	//ang=43;

	while(1)
	{

		this->RotatePoint(p1,&stp,ang);
		this->RotatePoint(p2,&stp,ang);
		this->RotatePoint(p3,&stp,ang);
		this->RotatePoint(p4,&stp,ang);
		this->m_GinRect=this->PointsInRect(p1,p2,p3,p4);


		if (m_GinRect>imax)
		{
			imax=m_GinRect;
			angmax=ang;

			pp1.x=p1->x;
			pp2.x=p2->x;
			pp3.x=p3->x;
			pp4.x=p4->x;

			pp1.y=p1->y;
			pp2.y=p2->y;
			pp3.y=p3->y;
			pp4.y=p4->y;

		}

		if (this->m_GinRect==m_NGinRect)
		{
			break;
		}

		if (this->m_RejectPointIdx>=0)
		{	
			if (this->m_GinRect==m_NGinRect-1)
			{
				break;
			}
		}

		if (ang>=360)
		{

			p1->x=pp1.x;
			p2->x=pp2.x;
			p3->x=pp3.x;
			p4->x=pp4.x;

			p1->y=pp1.y;
			p2->y=pp2.y;
			p3->y=pp3.y;
			p4->y=pp4.y;

			/*
			this->RotatePoint(p1,&stp,angmax);
			this->RotatePoint(p2,&stp,angmax);
			this->RotatePoint(p3,&stp,angmax);
			this->RotatePoint(p4,&stp,angmax);
			this->m_GinRect=this->PointsInRect(p1,p2,p3,p4);
			*/

			this->m_GinRect=this->PointsInRect(p1,p2,p3,p4);

			break;
		}

		ang+=mstep;
	}


	return 0;
}

/*
Поворот точки p вокруг цента c на угол a

*/


#define M_PI           3.14159265358979323846  /* pi */ 


int CTestM::RotatePoint(CPoint* p, CPoint* c, double a)
{

//Приводим радианы к градусам

	double ang=a;

	ang*=M_PI;
	ang/=180;

	double x1=p->x;
	double y1=p->y;

	double x0=c->x;
	double y0=c->y;

	double x2= ((x1 - x0) * cos(ang)) - ((y1 - y0) * sin(ang)) + x0;
	double y2= ((x1 - x0) * sin(ang)) + ((y1 - y0) * cos(ang)) + y0;

	p->x=x2;
	p->y=y2;

	return 0;
}

int CTestM::IsPointInsidePolygon (CPoint *p, int Number, int x, int y)
{
  int i1, i2, n, N, S, S1, S2, S3, flag;
  N = Number;
  for (n=0; n<N; n++)
  {
   flag = 0;
   i1 = n < N-1 ? n + 1 : 0;
   while (flag == 0)
   {
    i2 = i1 + 1;
    if (i2 >= N)
      i2 = 0;
    if (i2 == (n < N-1 ? n + 1 : 0))
      break;
    S = abs (p[i1].x * (p[i2].y - p[n ].y) +
             p[i2].x * (p[n ].y - p[i1].y) +
             p[n].x  * (p[i1].y - p[i2].y));
    S1 = abs (p[i1].x * (p[i2].y - y) +
              p[i2].x * (y       - p[i1].y) +
              x       * (p[i1].y - p[i2].y));
    S2 = abs (p[n ].x * (p[i2].y - y) +
              p[i2].x * (y       - p[n ].y) +
              x       * (p[n ].y - p[i2].y));
    S3 = abs (p[i1].x * (p[n ].y - y) +
              p[n ].x * (y       - p[i1].y) +
              x       * (p[i1].y - p[n ].y));
    if (S == S1 + S2 + S3)
    {
     flag = 1;
     break;
    }
    i1 = i1 + 1;
    if (i1 >= N)
      i1 = 0;
   }
   if (flag == 0)
     break;
  }
  return flag;
}

int CTestM::PointsInRect(CPoint* p1,CPoint* p2,CPoint* p3,CPoint* p4)
{

	int res=0;

	CPoint rect[4];

	rect[0].x=p1->x;
	rect[1].x=p2->x;
	rect[2].x=p3->x;
	rect[3].x=p4->x;

	rect[0].y=p1->y;
	rect[1].y=p2->y;
	rect[2].y=p3->y;
	rect[3].y=p4->y;

	for (int i=0; i<this->m_NGaps; i++)
	{
		if (this->m_PULL->record[i].act)
		{
			long xx=this->m_PULL->record[i].res.X;
			long yy=this->m_PULL->record[i].res.Y;

			if (IsPointInsidePolygon (rect, 4, xx, yy))
			{
				res++;
				this->m_PULL->record[i].res.inRect=1;
			}
		}
	}



	return res;
}


int CTestM::Reset(void)
{

//Очищаем результат
	memset(&m_Res,0,sizeof(TestM_gres));

	m_Res.resN=1;

	return 0;
}
