// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__70FB8DF3_88AC_40C5_802C_58621127B9E9__INCLUDED_)
#define AFX_GRAPH_H__70FB8DF3_88AC_40C5_802C_58621127B9E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Some  defines

//colorschemes
#define G_DEFAULTSCHEME 0
#define G_WHITESCHEME 1
#define G_REDSCHEME 2
#define G_BLUESCHEME 3
#define G_GREENSCHEME 4
#define G_MAGENTASCHEME 5
#define G_YELLOWSCHEME 6
#define G_CYANSCHEME 7
#define G_TSMCHEME	8

//default graphsize
#define G_MINGRAPHWIDTH 350
#define G_MINGRAPHHEIGHT 200

//default axies scaling
#define G_DEFAULTXMIN -100
#define G_DEFAULTXMAX 100
#define G_DEFAULTYMIN -100
#define G_DEFAULTYMAX 100

//miscellaneous
#define G_NUMTICKSANDGRID 10 //how may parts the dataarea is divided 
#define G_TICKLENGTH 10 //size of those little ticks on the axes

/*
Function related  defines and stuff
*/
//for the builtin functions
#define NUMFUNCTIONS 12
#define G_XSQUARED	0
#define G_XCUBED	1
#define G_MXPLUSC	2
#define G_SINX		3
#define G_COSX		4
#define G_LOGEX		5
//Plot function
#define G_PLOTXY	6
#define G_PLOTDEV_PERCENT	  7
#define G_PLOTDEV_ABS	8
#define G_MULTIPLOTXY 9
#define G_HISTOGRAM_PERCENT 10
#define G_HISTOGRAM_ABS 11
#define G_TSM_SENSOR	12

//plot type bar,line, etc...
#define NUMCHARTTYPES 3
#define G_BARCHART 0 //a rect (may be one pixel thick)
#define G_LINECHART 1 // each point is a line drawn from each x-y point to the next
#define G_DOTCHART 2 //a dot is placed at X-Y

#define G_BARCHARTWIDTH 5 //default width of a bar chart

//the colors for multiplot graphs
#define G_NUM_MULTIPLOT_COLORS 8
#define G_MULTIPLOT_COLOR_ONE (RGB(192,255,255))
#define G_MULTIPLOT_COLOR_TWO (RGB(0,0,255))
#define G_MULTIPLOT_COLOR_THREE (RGB(255,0,0))
#define G_MULTIPLOT_COLOR_FOUR (RGB(255,255,0))
#define G_MULTIPLOT_COLOR_FIVE (RGB(255,192,192))
#define G_MULTIPLOT_COLOR_SIX (RGB(0,255,0))
#define G_MULTIPLOT_COLOR_SEVEN (RGB(0,255,255))
#define G_MULTIPLOT_COLOR_EIGHT (RGB(255,0,255))


#define ABS_ZERO_X		 (m_iGraphX)	
#define ABS_ZERO_Y		 (m_iGraphY)

#define GRAPH_ZERO_X	 (m_iGraphX+CalcLeftMargin())		//Область сетки X
#define GRAPH_ZERO_Y	 (m_iGraphY+CalcTopMargin())		//Область сетки Y

#define ABS_RIGHT_X		 (m_iGraphX+m_iGraphWidth)			//конец области отображния X
#define ABS_DOWN_Y		 (m_iGraphY+m_iGraphHeight)		//конец области отображния Y

#define GRAPH_RIGHT_X    (m_iGraphX+this->m_iGraphWidth-CalcRightMargin())		//конец области сетки X
#define GRAPH_DOWN_Y     (m_iGraphY+m_iGraphHeight-CalcBottomMargin())	//конец области сетки Y

#define GRAPH_ZERO_POINT	(GRAPH_ZERO_X,GRAPH_ZERO_Y)					//начало координат области сетки
#define GRAPH_WIDE			(GRAPH_RIGHT_X-GRAPH_ZERO_X)					//ширина областисетки
#define	GRAPH_HEIGHT		(GRAPH_DOWN_Y-GRAPH_ZERO_Y)					//высота области сетки

#define GRAPH_CENTER_X		(GRAPH_ZERO_X+GRAPH_WIDE/2)					//координата цента X
#define GRAPH_CENTER_Y		(GRAPH_ZERO_Y+GRAPH_HEIGHT/2)					//координата цента Y

#define GRAPH_CENTER_POINT	(GRAPH_CENTER_X,GRAPH_CENTER_Y)


//some structures for passing data
typedef struct 
{
	UINT FuncType;//e.g G_SINX
	UINT ChartType;//bar,line, dot etc..
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	//char   *szGraphTitle;
	TCHAR  *szGraphTitle;
	//char   *szYLegend;
	TCHAR   *szYLegend;
	//char	*szXLegend;
	TCHAR	*szXLegend;
	double *pPlotXYItems;
	UINT    num_PlotXYItems;
	double  Const_1;
	double Const_2;
}G_FUNCTIONSTRUCT, *LPG_FUNCTIONSTRUCT;







class CGraph  
{
public:
	void ShowTicks(BOOL bShow);
	void ClearFunction(void);
	BOOL DoFunction(G_FUNCTIONSTRUCT *pFunctionParams);
	void SetFunctionName(CString FunctionName);
	void ShowGrid(BOOL bShow);
	void SetYLineAtLeft(BOOL AtLeft);
	void GraphSetAllDefaults();
	void SetYLegendText(CString YText);
	void SetXLegendText(CString XText);
	void SetGraphTitle(CString GraphTitle);
	void PaintGraph(void);
	CGraph(CWnd *pParentWnd,int xPos=0, int yPos=0, int Width =0, int Height=0, UINT colorscheme=G_DEFAULTSCHEME);
	void SetYAxisScale(double min, double max);
	void SetXAxisScale(double min,double max);
	void SetGraphSizePos(int xPos, int yPos, int Width, int Height);
	void SetColorScheme(int Scheme, BOOL bRedraw=FALSE);
	void CreateGraphFont(CString FaceName,UINT size);
	CGraph();
	virtual ~CGraph();

	
	int target_LEFT_X(void) {return GRAPH_ZERO_X;}
	int target_RIGHT_X(void) {return GRAPH_RIGHT_X;}
	int target_TOP_Y(void) {return GRAPH_ZERO_Y;}
	int target_BOTTOM_Y(void) {return GRAPH_DOWN_Y;}
	int target_CENTER_X(void) {return GRAPH_CENTER_X;}
	int target_CENTER_Y(void) {return GRAPH_CENTER_Y;}
	int target_WIDE(void) {return GRAPH_WIDE;}
	int target_HEIGHT(void) {return GRAPH_HEIGHT;}


private:
	void DoHistogram();
	LONG ConvertToGraphCoords(double x, double y);	
	COLORREF GetMultiPlotPenColor(UINT PenNumber);
	void DoMultiPlotXY(void);
	void CGraph::DoDeviationPercentAbs(void);	
	double ConstrainY (double y);
	void DoPlotXY();
	void DoLogeX();
	void DoMXPlusC(void);
	void DoXCubed(void);
	void DoXSquared();
	void DoCosX(void);
	void DrawBar(UINT x, UINT y);
	void DrawConnectLine(UINT FromX, UINT FromY, UINT ToX, UINT ToY);
	void PlotPoints(UINT x, UINT y, UINT prevx, UINT prevy);
	void DrawDot (UINT X, UINT Y);
	void DoSineX();
	void DrawFunction();
	void SetDefaultColorScheme(void);
	void DrawFunctionName(void);
	void SetDefaultFunctionName(void);
	CString m_szFunctionNameText;
	COLORREF m_crFunctionNameColor;
	void DrawYAxisNumbers();
	void DrawXAxisNumbers(void);
	void DrawTicks(void);
	double CalcYAxisGridAndTicks(void);
	CRect CalcDataArea(void);
	double CalcXAxisGridAndTicks(void);
	void DrawGrid(void);
	void DrawYLegend();
	void DrawXLegend();
	void SetDefaultYLegend(void);
	void SetDefaultXLegend(void);
	void SetDefaultGraphTitle(void);
	void DrawGraphTitle();
	UINT CalcRightMargin();
	UINT CalcLeftMargin();
	UINT CalcBottomMargin();
	UINT CalcTopMargin();
	BOOL m_bShowTicks;//Ticks are the little things on the x & y axis
	BOOL m_bShowGrid;
	BOOL m_bAutofit;
	BOOL m_bYLineAtLeft;
	CString m_szFontFace;
	CString m_szYLegendText;
	CString m_szXLegendText;
	CString m_szGraphTitle;
	COLORREF m_crYTickColor;
	COLORREF m_crXTickColor;
	COLORREF m_crYLegendTextColor;
	COLORREF m_crXLegendTextColor;
	COLORREF m_crGraphTitleColor;
	COLORREF m_crGraphPenColor;
	COLORREF m_crGraphBkColor;
	COLORREF m_crGridColor;
	int m_iFontSize;//
	int m_iGraphWidth;//
	int m_iGraphHeight;//
	int m_iGraphX;//location of the fraph within the window
	int m_iGraphY;//location of the graph within the window
	double m_dXAxisMin;// the start value of X
	double m_dYAxisMin;//start value of Y
	double m_dXAxisMax;
	double m_dYAxisMax;
	CWnd *m_pWnd;//parent/owner
	//Helper calculated values - that is to say that these
	//values are not passed in to the graph
	//they are calculated from other given
	CFont m_GraphFont;//default font font created from default fontface, & point size
	int m_iCharHeight;//calculated from the font
	int m_iCharWidth;//calculated from the font
	int m_iOriginX;//location of the origin within the graph
	int m_iOriginY;//location of the origin within the graph
	double m_dPixelsPerY;//scaling
	double m_dPixelsPerX;//scaling	
	int m_iScrollPosX;//
	int m_iScrollPosY;//
	//Data related variables
	G_FUNCTIONSTRUCT *m_pFunctionParams;

public:

	void DoCustomFun(void);
	double*				m_PullData;
	int 				m_PullLen;
	int					m_YScaleMin;
	int					m_YScaleMax;
	double				m_SetPoint;
	double				m_HPoint;
	double				m_LPoint;
	CString m_Units;

	int m_TARGETSCALE;
	void DrawGap(int x, int y, int n);

    //int m_TARGSCALEFACTOR;

	void (*drawgaps)( void );


	BOOL m_DrawGaps;

	long mdlX;
	long mdlY;


	int DrawMdlGap(long x, long y);
	int DrawTestCircle(long x, long y, long r);
	int DrawTestRect(long x, long y, long h, long w);
	int DrawCtrlGap(long x, long y);

	int DrawTestRectEx( int rejectidx, //номер оторвавшейся точки если -1 считаем все точки
						long x[4],	   //координаты 4 точек	
						long y[4],	   //	
						long stpx,	   //координаты СТП 	
						long stpy,	   //	
						long h,		   //высота прямоугольника	
						long w		   //ширина прямоугольника	
						);


	int DrawTestRectCalculated(CPoint left,CPoint right,CPoint top,CPoint bottom);

};

#endif // !defined(AFX_GRAPH_H__70FB8DF3_88AC_40C5_802C_58621127B9E9__INCLUDED_)
