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


//������� �� ��� Y � ����� p
int CLine::TransV(CPoint* p)
{

//�������� �� ��� Y
	double dy=Y(p->x);
	dy=p->y-dy;

//��������� - ����������������� � ����� ����������� �� Y

	ReInit(P1.x,P2.x,P1.y+dy,P2.y+dy);

	return 0;
}



//�����������������
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

//���� ����� ����������� � ���� Y
	double y0=Y(0);
//���������� y �� ����������� � ���� Y
	double dyp=y0-P1.y;
//���������� �� ����� p �� ��� Y
	double rp=sqrt((p->x*p->x)+(dyp*dyp));
//���������� ����� ����� �� ��� Y
	double rr=r+rp;
//������� ���� �
	double tga=dyp/P1.x;
//��������� ����


	return 0;
}
