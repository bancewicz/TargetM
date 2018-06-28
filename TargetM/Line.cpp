#include "StdAfx.h"
#include "Line.h"
#include <math.h>

CLine::CLine(void)
{
}

CLine::CLine(double x1, double x2, double y1, double y2)
{
	//A=a; B=b; C=c;

	//y1+=1000;
	//y2+=1000;

	//x1+=5;
	//x2+=5;

	A=y1-y2;
	B=x2-x1;
	C=x1*y2-x2*y1;

	P1.x=x1;
	P1.y=y1;

	P2.x=x2;
	P2.y=y2;

}


CLine::~CLine(void)
{
	A=1; B=1; C=1;
}


double CLine::Y(double x)
{
	double res;
	res=C+A*x;
	res*=-1;
	res/=B;

	return res;
}


double CLine::X(double y)
{
	double res;

	res=C+B*y;
	res/=A;
	res*=-1;

	return res;
}


//Перенос по оси Y в точку p
int CLine::TransV(CPoint* p)
{

//Поправка по оси Y
	double dy=Y(p->x);
	dy=p->y-dy;

//Применяем - переинициализация к новым координатам по Y

	ReInit(P1.x,P2.x,P1.y+dy,P2.y+dy);

	return 0;
}



//Переинициализация
int CLine::ReInit(double x1, double x2, double y1, double y2)
{

	A=y1-y2;
	B=x2-x1;
	C=x1*y2-x2*y1;

	P1.x=x1;
	P1.y=y1;

	P2.x=x2;
	P2.y=y2;

	return 0;
}


int CLine::MoveP1(CPoint* p, double r)
{

//Ищем точку пересечения с осью Y
	double y0=Y(0);
//Приращение y до пересечения с осью Y
	double dyp=y0-P1.y;
//расстояние от точки p до оси Y
	double rp=sqrt((p->x*p->x)+(dyp*dyp));
//Расстояние новой точки до оси Y
	double rr=r+rp;
//Тангенс угла А
	double tga=dyp/P1.x;
//Вычисляем угол


	return 0;
}
