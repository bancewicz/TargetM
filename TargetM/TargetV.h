
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


//Прямоугольник для подсчета входящих пробоин

typedef struct{
//Высота
	long h;
//Ширина
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

//Номер испытания
	int m_TNum;
//Число выстрелов в испытании
	int m_NGaps;
//Параметры тестового прямоугольника
	TestM_rect m_Rect;
//Диаметр тестовой окружности	
	long m_NCircleD;
//Контрольная точка
	TestM_point m_TPoint;
//Заданное отклонение от КТ
	long m_NTpointR;
//Заданное число пробоин внутри прямоугольника
	long m_NGinRect;
//Заданное число пробоин внутри круга
	long m_NGinCircle;
//Заданное значение средней полосы рассеивания по высоте
	long m_NBandH;
//Заданное значение средней полосы рассеивания по боковому направлению
	long m_NBandW;
//Заданное значение эквивалентной полосы рассеивания
	long m_NBandE;
//Вхождение в габарит мишени 48х36
	int m_N4836;
//Вхождение в габарит мишени 48х36
	int m_4836;


//Комментарий - номер пункта нормативных требований
	CString m_Doc;	
//Прочие комментарии
	CString m_Mark;

//Итоги выполнения теста/////////////////////////////////////////

//Подсчитанное число пробоин внутри прямоугольника
	long m_GAll;

//Подсчитанное число пробоин внутри прямоугольника
	long m_GinRect;
//Подсчитанное число пробоин внутри круга
	long m_GinCircle;
//Подсчитанное отклонение от КТ
	long m_TpointR;

//Подсчитанное значение средней полосы рассеивания по высоте
	long m_BandH;
//ПОдсчитанное значение средней полосы рассеивания по боковому направлению
	long m_BandW;
//Подсчитанное значение эквивалентной полосы рассеивания
	long m_BandE;

//Подсчитанные координаты СТП
	TestM_point m_STP;
//Расстояние СТП от центра
	long m_STP_R;

	BOOL m_TestOver;
	BOOL m_Itog;

//Номер оторвавшейся точки 
	int m_RejectPointIdx;

///////////////////////////////////////////////////////////////////////

//Указатель на пулл с результатами сессии
	TargetM_res_pull* m_PULL;

//Параметры теста
	CString m_TPar_strs[60];
//Результаты теста
	CString m_TRes_strs[60];
	int ClearRes(void);

//Указатель на ини
	CIniFileManager* m_Ini;
//Указатель на дисплей отображения результата
	CEdit* m_DisplayRes;
//Указатель на дисплей параметров теста
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

//Флаг поиска мишени
BOOL* TARGETSEEK;

//Флаг включения мишени
BOOL* TARGON;

BOOL AskTargetRequest(TargetM_cmd cmd);
int ResCalculate(int test);

/*
	long m_TrectW;
	long m_TrectH;
	long m_TcircleR;
*/

//Координаты СТП
	long mdlX;
	long mdlY;

//Радиус окружности
	long tstR;
//Размеры прямоугольника
	long tstH;
	long tstW;

//Координаты контрольной точки 0,140
	long ctrlX;
	long ctrlY;

    BYTE InsideCyrcle(long x, long y, long cx, long cy, long cr);

//Всего получено пробоин
	int gapsInTotal;

//Входит в круг
	int gapsInCyrcle;
//Входит в прямоугольник
	int gapsInRect;

//Тест закончен
	BOOL m_TestOver;

	BYTE InsideRect(long x, long y);

//Отклонение СТП от КТ
	long ctrlR;

//Допустимое отклонение СТП от КТ
	long ctrlR_set;


//Радиус СТП относительно 0
	long mdlR;

	long DispersRang_Side(void);
	long DisperseRang_Hight(void);
	long DisperseRang_Mdl(void);

	int m_TEST_NUmber;
	int SetTestNгmber(int test);

	CEdit* m_TESTPAR_Edit;

//Параметры теста

//Число заданных пробоин в тесте
	int m_TESTPAR_gapsnum;

//Строка с итогами теста
	CString m_TSETRESULT_Str;

	CString m_TEST_RESULT_Strings[60];

	int m_EMULATEMODE;


	CTestM m_Test;
};


/*

Статистическая обработка результатов

1. Четыре одиночных выстрела.  (4)
-------------------------------

Не менее трех пробоин (при одной оторвавшейся) должны укладываться в прямоугольник высотой 12 см шириной 10 см 

Точка прицеливания - х=0 у=0
Контрольная точка КТ - 14 см выше точки прицеливания

Средняя точка попадания СТП расстояние не более 5 см от КТ

Оторвавшейся считается побоина отстоящая от СТП более чем на 2,5 радиуса круга с центром в СТП и охватывающего три пробоины


2. Автоматическая стрельба десятью выстрелами очередями по 2- выстрела.   (10)
 -----------------------------------------------------------------------

 Не менее 8 пробоин вмещаются в круг диаметром 20 см
 или
 Не менее 8 пробоин вмещаются в круг диаметром 18 см
 СТП отклоняется от КТ не более 5 см

 суммарое число пробоин в круге 20 см не менее 90% 
 первая очередь не учитывается если не все пробоины находятся в мишени 
 (за 100% принимается 8 пробоин внутри круга 20 см)


 3. Контроль 4 одиночных выстрела
 ---------------------------------

 Четыре пробоины или 3 при одной оторвавшейся вмещаются в круг диаметром 10 см
 СТП отклонение от КТ не более 10см
 или
 СТП отклонение от КТ не более 5 см

 4. Проверка автматической стрельбой
 ------------------------------------

 Не менее 7 пробоин из 10 вмещаются в круг 20 см
 или
 Не менее 7 пробоин из 10 вмещаются в круг 18 см

 Отклонение СТП от КТ не более 10 см

 5. Испытание 3 серии по 20 выстрелов (очередями о 3-7 выстрелов) (или очередями по 7-8 выстрелов)
 -------------------------------------

 Средняя величина сердцевинной полосы рассеивания
      по высоте        Св ср<= 18 см
	  по боковому      Сб ср<= 18 см

	  Эквивалентная величина сердцевины рассеивания пробоин

	  Сэкв=SQR(Cв ср Х Сб ср) <= 18 см
	  или
	  Сэкв=SQR(Cв ср Х Сб ср) <= 20 см


6. Непрерывная стрельба очередью 10 выстрелов
----------------------------------------------

КТ на 14 см выше точки прицеливания

Не менее 8 пробоин входит в прямоугольник высотой 16 см шириной 14 см без определения отклонения СТП
СТП не выходит за габарит мишени высотой 48 шириной 36 см
Положение сторон прямоугольника относительно мишени произвольное

7. 10 выстрелов очередями по 2-3 выстрела
--------------------------------------------
Не менее 8 пробоин из 10 вмещаются в круг 20 см
СТП не выходит за габарит мишени высотой 48 шириной 36 см

8. Очередь 10 выстрелов
------------------------

Не менее 8 пробоин находятся в пределах мишени и вмещаются в прямоугольник 14 на 16 см. Положение сторон прямоугольника относительно мишени произвольное.


9. Непрерывная очередь 20 выстрелов.
------------------------------------

СвБ=15 см     Сб<=12 см


//----------------------------------------------------------------------------------------------------------------------------------
В случае малого количества попаданий (от двух до четырёх) положение средней точки попадания определяется как простое среднее.
При большом количестве снарядов наиболее простым методом оценки положения средней точки попадания является определение
горизонтальной и вертикальной линий, по обеим сторонам которых располагается одинаковое количество пробоин.
Их пересечение и дает среднюю точку попадания[2].

Среднюю точку попадания (СТП) можно также определить способом вычисления (расчета).
Для этого необходимо:
— провести через левую (правую) пробоину (точку встречи) вертикальную линию, измерить кратчайшее расстояние от каждой пробоины (точки встречи) до этой линии, сложить все расстояния от вертикальной линии и разделить сумму на число пробоин (точек встречи);
— провести через нижнюю (верхнюю) пробоину (точку встречи) горизонтальную линию, измерить кратчайшее расстояние от каждой пробоины (точки встречи) до этой линии, сложить все расстояния от горизонтальной линия и разделить сумму на число пробоин (точек встречи).
Полученные числа определяют удаление СТП от указанных линий.

http://www.shooting-ua.com/books/book_65.htm

http://www.nastavleniya.ru/OSS/os12.html













//-----------------------------------------------------------------------------------------------------------------------------------
*/














