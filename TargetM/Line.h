#pragma once
class CLine
{
public:
	CLine(void);
	CLine(double x1, double x2, double y1, double y2);


	~CLine(void);

	double A,B,C;
	double Y(double x);
	double X(double y);

	CPoint P1;
	CPoint P2;
	int TransV(CPoint* p);
	int ReInit(double x1, double x2, double y1, double y2);
	int MoveP1(CPoint* p, double r);
};

