
#pragma once

#include "Serial.h"
#include "SerialLight.h"
#include "MyEdit.h"
#include "Graph.h"
#include "IniFileManager.h"

//-------------------------------------------------------------------------------------------------------------


typedef enum {

	TCMD_NONE,
	TCMD_SEEK,
	TCMD_ON,
	TCMD_OFF,
	TCMD_READ

} TargetM_cmd;

typedef struct {

	int code_res;

	BYTE addr;
	BYTE len;
	BYTE cmd;
	BYTE gap;
	long X;
	long Y;
	BYTE area;
	unsigned int tick;

	unsigned int tick_hour;
	unsigned int tick_min;
	unsigned int tick_sec;
	unsigned int tick_dec;

	BYTE status;
	BYTE inCyrcle;
	BYTE inRect;

	unsigned short CRC;

} TargetM_res;


typedef struct {

	long stpX;
	long stpY;
	long stpR;

} TargetM_gres;

typedef enum {

	TSTATE_unknown,			
	TSTATE_urespond_on,	    
	TSTATE_urespond_off,    
	TSTATE_cmd_seek,
	TSTATE_cmd_on,
	TSTATE_cmd_off,
	TSTATE_read_norm,
	TSTATE_read_err,
	TSTATE_len_err,
	TSTATE_addr_err,
	TSTATE_crc_err,
	TSTATE_err,
	TSTATE_noanswer,
	TSTATE_unknown_answer

} TargetM_state;


typedef struct {

	int act;
	TargetM_res res;

} TargetM_res_record;

typedef struct {

	int full;
	int idx;
	TargetM_res_record record [60];

} TargetM_res_pull;


//������������� ��� �������� �������� �������

typedef struct{
//������
	long h;
//������
	long w;

} TestM_rect;

typedef struct{

	long x;
	long y;

} TestM_point;

typedef struct{

	int stpdevN;
	int inrectN;
	int incircleN;
	int bandwN;
	int bandhN;
	int bandeN;
	int m3846N;

	int resN;

} TestM_gres;



class CTestM
{

	public:
	CTestM();
	~CTestM();

//����� ���������
	int m_TNum;
//����� ��������� � ���������
	int m_NGaps;
//��������� ��������� ��������������
	TestM_rect m_Rect;
//������� �������� ����������	
	long m_NCircleD;
//����������� �����
	TestM_point m_TPoint;
//�������� ���������� �� ��
	long m_NTpointR;
//�������� ����� ������� ������ ��������������
	long m_NGinRect;
//�������� ����� ������� ������ �����
	long m_NGinCircle;
//�������� �������� ������� ������ ����������� �� ������
	long m_NBandH;
//�������� �������� ������� ������ ����������� �� �������� �����������
	long m_NBandW;
//�������� �������� ������������� ������ �����������
	long m_NBandE;
//��������� � ������� ������ 48�36
	int m_N4836;
//��������� � ������� ������ 48�36
	int m_4836;


//����������� - ����� ������ ����������� ����������
	CString m_Doc;	
//������ �����������
	CString m_Mark;

//����� ���������� �����/////////////////////////////////////////

//������������ ����� ������� ������ ��������������
	long m_GAll;

//������������ ����� ������� ������ ��������������
	long m_GinRect;
//������������ ����� ������� ������ �����
	long m_GinCircle;
//������������ ���������� �� ��
	long m_TpointR;

//������������ �������� ������� ������ ����������� �� ������
	long m_BandH;
//������������ �������� ������� ������ ����������� �� �������� �����������
	long m_BandW;
//������������ �������� ������������� ������ �����������
	long m_BandE;

//������������ ���������� ���
	TestM_point m_STP;
//���������� ��� �� ������
	long m_STP_R;

	BOOL m_TestOver;
	BOOL m_Itog;

//����� ������������ ����� 
	int m_RejectPointIdx;

///////////////////////////////////////////////////////////////////////

//��������� �� ���� � ������������ ������
	TargetM_res_pull* m_PULL;

//��������� �����
	CString m_TPar_strs[60];
//���������� �����
	CString m_TRes_strs[60];
	int ClearRes(void);

//��������� �� ���
	CIniFileManager* m_Ini;
//��������� �� ������� ����������� ����������
	CEdit* m_DisplayRes;
//��������� �� ������� ���������� �����
	CEdit* m_DisplayPar;

	int ClearPar(void);
	int SaveIniPar(void);
	int LoadIniPar(int num);
	void DisplayTestPar(void);
	void DisplayTestRes(void);
	int ResCalculate(void);


	int TInsideCyrcle(long x, long y, long cx, long cy, long cr);
    int TInsideRect(long x, long y, long cx, long cy, long h, long w);

	int TestRectCalculate(CPoint* p1,CPoint* p2,CPoint* p3,CPoint* p4);

	CPoint p1,p2,p3,p4;

	int RotatePoint(CPoint* p, CPoint* c, double a);

	double m_RectAngle;

	int CTestM::IsPointInsidePolygon (CPoint *p, int Number, int x, int y);

	int PointsInRect(CPoint* p1,CPoint* p2,CPoint* p3,CPoint* p4);

	TestM_gres m_Res;

	int Reset(void);
};


//--------------------------------------------------------------------------------------------------------------

#define WAIT_CMD_RESPOND				WM_USER+1

//#pragma once

typedef enum {

	Tarea_0,
	Tarea_1,
	Tarea_2,
	Tarea_3,
	Tarea_4,
	Tarea_5,
	Tarea_6,
	Tarea_7,
	Tarea_8

} TargetM_area;

typedef enum {

	Tcstatus_ON,
	Tcstatus_OFF

} TsrgetM_cstatus;

#define TARGETM_CMD_SEEK_REQUEST	0x9FABAC  
#define TARGETM_CMD_SEEK_RESPOND	0x9FBBCC

#define TARGETM_CMD_ON_REQUEST		0xA0FF00FFFFFF0000  
#define TARGETM_CMD_ON_RESPOND		0xA000

#define TARGETM_CMD_OFF_REQUEST		0xA100
#define TARGETM_CMD_OFF_RESPOND		0xA100

/*
typedef enum {

	TCMD_NONE,
	TCMD_SEEK,
	TCMD_ON,
	TCMD_OFF,
	TCMD_READ

} TargetM_cmd;

typedef struct {

	int code_res;

	BYTE addr;
	BYTE len;
	BYTE cmd;
	BYTE gap;
	long X;
	long Y;
	BYTE area;
	unsigned int tick;

	unsigned int tick_hour;
	unsigned int tick_min;
	unsigned int tick_sec;
	unsigned int tick_dec;

	BYTE status;
	BYTE inCyrcle;
	BYTE inRect;

	unsigned short CRC;

} TargetM_res;


typedef struct {

	long stpX;
	long stpY;
	long stpR;


} TargetM_gres;

typedef enum {

	TSTATE_unknown,			
	TSTATE_urespond_on,	    
	TSTATE_urespond_off,    
	TSTATE_cmd_seek,
	TSTATE_cmd_on,
	TSTATE_cmd_off,
	TSTATE_read_norm,
	TSTATE_read_err,
	TSTATE_len_err,
	TSTATE_addr_err,
	TSTATE_crc_err,
	TSTATE_err,
	TSTATE_noanswer,
	TSTATE_unknown_answer

} TargetM_state;


typedef struct {

	int act;
	TargetM_res res;

} TargetM_res_record;

typedef struct {

	int full;
	int idx;
	TargetM_res_record record [60];

} TargetM_res_pull;

*/

//----------------------------------------------------------------------------------------


class CTargetM
{
public:
	CTargetM(void);
	~CTargetM(void);

	BYTE m_bufTX[1024];
	BYTE m_bufRX[1024];

	int m_rxIDX;
	int m_txIDX;

	double m_resX;
	double m_resY;

	TargetM_area m_Tarea;

	long m_Timeticks;

	void Clear(void);

	void View(void);

	CEdit* m_TIMETICKS_Edit;
	CEdit* m_RESX_Edit;
	CEdit* m_RESY_Edit;
	CEdit* m_TAREA_Edit;
	CMyEdit* m_TSTATUS_Edit;

//	void Trace(CEdit* timeticks,CEdit* resx,CEdit* resy,CEdit* tarea,CMyEdit* tstatus, CSerial*  serial);
	void Trace(CEdit* timeticks,CEdit* resx,CEdit* resy,CEdit* tarea,CMyEdit* tstatus, CSerialLight*  serial);

	BYTE m_ADDR;

	void init_crc16_tab(void);
	unsigned short update_crc_16(unsigned short crc, char c);
	unsigned short CTargetM::CRC16(unsigned char* buf, int len);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CSerial* m_Com;
	CSerialLight* m_Com;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TargetM_cmd m_CMD;

	BOOL CMD_create(TargetM_cmd cmd);

	BYTE m_numGAP;

	TargetM_res RESPOND_proc(BYTE* buf, int len);

	int RESPOND_emul(BYTE* buf, TargetM_cmd cmd);

	void View(TargetM_res* res);

	CEdit* m_INFO_Edit;
	CEdit* m_GAP_Edit;

	CEdit* m_CONSOLEC_Edit;
	CEdit* m_CONSOLER_Edit;

	CString m_Info;

	void StatusView(TargetM_state state);

	void ClearForm(void);

	HWND m_MAINWND;

	BOOL m_Auto;
	BOOL m_STOP;

	CMyEdit* m_DISPLAY_Edit;

	CGraph* m_pGraph;

	TargetM_res_pull m_PULL;
	void ClearPULL(void);
	void DrawPull(void);

	CString* m_Operator;

//���� ������ ������
BOOL* TARGETSEEK;

//���� ��������� ������
BOOL* TARGON;

BOOL AskTargetRequest(TargetM_cmd cmd);
int ResCalculate(int test);

/*
	long m_TrectW;
	long m_TrectH;
	long m_TcircleR;
*/

//���������� ���
	long mdlX;
	long mdlY;

//������ ����������
	long tstR;
//������� ��������������
	long tstH;
	long tstW;

//���������� ����������� ����� 0,140
	long ctrlX;
	long ctrlY;

    BYTE InsideCyrcle(long x, long y, long cx, long cy, long cr);

//����� �������� �������
	int gapsInTotal;

//������ � ����
	int gapsInCyrcle;
//������ � �������������
	int gapsInRect;

//���� ��������
	BOOL m_TestOver;

	BYTE InsideRect(long x, long y);

//���������� ��� �� ��
	long ctrlR;

//���������� ���������� ��� �� ��
	long ctrlR_set;


//������ ��� ������������ 0
	long mdlR;

	long DispersRang_Side(void);
	long DisperseRang_Hight(void);
	long DisperseRang_Mdl(void);

	int m_TEST_NUmber;
	int SetTestN�mber(int test);

	CEdit* m_TESTPAR_Edit;

//��������� �����

//����� �������� ������� � �����
	int m_TESTPAR_gapsnum;

//������ � ������� �����
	CString m_TSETRESULT_Str;

	CString m_TEST_RESULT_Strings[60];

	int m_EMULATEMODE;


	CTestM m_Test;
};


/*

�������������� ��������� �����������

1. ������ ��������� ��������.  (4)
-------------------------------

�� ����� ���� ������� (��� ����� ������������) ������ ������������ � ������������� ������� 12 �� ������� 10 �� 

����� ������������ - �=0 �=0
����������� ����� �� - 14 �� ���� ����� ������������

������� ����� ��������� ��� ���������� �� ����� 5 �� �� ��

������������ ��������� ������� ��������� �� ��� ����� ��� �� 2,5 ������� ����� � ������� � ��� � ������������� ��� ��������


2. �������������� �������� ������� ���������� ��������� �� 2- ��������.   (10)
 -----------------------------------------------------------------------

 �� ����� 8 ������� ��������� � ���� ��������� 20 ��
 ���
 �� ����� 8 ������� ��������� � ���� ��������� 18 ��
 ��� ����������� �� �� �� ����� 5 ��

 �������� ����� ������� � ����� 20 �� �� ����� 90% 
 ������ ������� �� ����������� ���� �� ��� �������� ��������� � ������ 
 (�� 100% ����������� 8 ������� ������ ����� 20 ��)


 3. �������� 4 ��������� ��������
 ---------------------------------

 ������ �������� ��� 3 ��� ����� ������������ ��������� � ���� ��������� 10 ��
 ��� ���������� �� �� �� ����� 10��
 ���
 ��� ���������� �� �� �� ����� 5 ��

 4. �������� ������������� ���������
 ------------------------------------

 �� ����� 7 ������� �� 10 ��������� � ���� 20 ��
 ���
 �� ����� 7 ������� �� 10 ��������� � ���� 18 ��

 ���������� ��� �� �� �� ����� 10 ��

 5. ��������� 3 ����� �� 20 ��������� (��������� � 3-7 ���������) (��� ��������� �� 7-8 ���������)
 -------------------------------------

 ������� �������� ������������ ������ �����������
      �� ������        �� ��<= 18 ��
	  �� ��������      �� ��<= 18 ��

	  ������������� �������� ���������� ����������� �������

	  ����=SQR(C� �� � �� ��) <= 18 ��
	  ���
	  ����=SQR(C� �� � �� ��) <= 20 ��


6. ����������� �������� �������� 10 ���������
----------------------------------------------

�� �� 14 �� ���� ����� ������������

�� ����� 8 ������� ������ � ������������� ������� 16 �� ������� 14 �� ��� ����������� ���������� ���
��� �� ������� �� ������� ������ ������� 48 ������� 36 ��
��������� ������ �������������� ������������ ������ ������������

7. 10 ��������� ��������� �� 2-3 ��������
--------------------------------------------
�� ����� 8 ������� �� 10 ��������� � ���� 20 ��
��� �� ������� �� ������� ������ ������� 48 ������� 36 ��

8. ������� 10 ���������
------------------------

�� ����� 8 ������� ��������� � �������� ������ � ��������� � ������������� 14 �� 16 ��. ��������� ������ �������������� ������������ ������ ������������.


9. ����������� ������� 20 ���������.
------------------------------------

���=15 ��     ��<=12 ��


//----------------------------------------------------------------------------------------------------------------------------------
� ������ ������ ���������� ��������� (�� ���� �� ������) ��������� ������� ����� ��������� ������������ ��� ������� �������.
��� ������� ���������� �������� �������� ������� ������� ������ ��������� ������� ����� ��������� �������� �����������
�������������� � ������������ �����, �� ����� �������� ������� ������������� ���������� ���������� �������.
�� ����������� � ���� ������� ����� ���������[2].

������� ����� ��������� (���) ����� ����� ���������� �������� ���������� (�������).
��� ����� ����������:
� �������� ����� ����� (������) �������� (����� �������) ������������ �����, �������� ���������� ���������� �� ������ �������� (����� �������) �� ���� �����, ������� ��� ���������� �� ������������ ����� � ��������� ����� �� ����� ������� (����� �������);
� �������� ����� ������ (�������) �������� (����� �������) �������������� �����, �������� ���������� ���������� �� ������ �������� (����� �������) �� ���� �����, ������� ��� ���������� �� �������������� ����� � ��������� ����� �� ����� ������� (����� �������).
���������� ����� ���������� �������� ��� �� ��������� �����.

http://www.shooting-ua.com/books/book_65.htm

http://www.nastavleniya.ru/OSS/os12.html













//-----------------------------------------------------------------------------------------------------------------------------------
*/














