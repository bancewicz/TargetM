//////////////////////////////////////////////////////////////////////////
/*Updates/Fixes/etc..:
16 October 2005
Not all GetDC() were matched with a corresponding ReleaseDC() - corrected

12/13 September 2009
problem with memory leaks - caused by not deleting GDIObject correctly.*/
//////////////////////////////////////////////////////////////////////////////




// Graph.cpp: implementation of the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
//!!!#include "CStats.h"
#include "Graph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
#pragma warning (disable:4244 4018 4701)

CGraph::CGraph()
{
	GraphSetAllDefaults();
	m_pWnd=NULL;
	m_TARGETSCALE=700;
	//m_TARGSCALEFACTOR=1;
	drawgaps=NULL;
	m_DrawGaps=FALSE;
}

///////////////////// Another constructor //////////////////////////////
/*																	////
*/																	////
////////////////////////////////////////////////////////////////////////
CGraph::CGraph(CWnd *pParentWnd, int xPos, int yPos, int Width, int Height, UINT colorscheme)
{
	//First set everything to their default values
	GraphSetAllDefaults();
	
	//setup default values
	m_pWnd=pParentWnd;
	SetColorScheme(colorscheme);
	//set graph position 
	m_iGraphX=xPos<0?0:xPos;
	m_iGraphY=yPos<0?0:yPos;
	//set graphsize (0 or less means default)
	m_iGraphWidth = Width < G_MINGRAPHWIDTH?G_MINGRAPHWIDTH:Width;
	m_iGraphHeight=Height < G_MINGRAPHHEIGHT ?G_MINGRAPHHEIGHT:Height;
	m_TARGETSCALE=50;
	//m_TARGSCALEFACTOR=1;
	drawgaps=NULL;
	m_DrawGaps=FALSE;
}
//////////////////// Default Destructor ///////////////////////////
/*																///
*/																///
///////////////////////////////////////////////////////////////////
CGraph::~CGraph()
{

}


/////////////////////// GraphSetAllDefaults ///////////////////////////////
/*	This function sets all the member variables to their default values	///
	creates the default font, etc....									///
	Because there are so may member variables, attempting to make		///
	constructors to cover all possibilites will be a pain - so all		///
	constructors should call this										///
	function first, then overwrite the default values as required.		///
*/																		///
///////////////////////////////////////////////////////////////////////////
void CGraph::GraphSetAllDefaults()
{
	//setup default values
	m_pWnd=NULL;
	//create default font
	CreateGraphFont(L"Courier",8);
	SetDefaultColorScheme();
	m_bAutofit=TRUE;
	m_bShowGrid=TRUE;
	m_bShowTicks=TRUE;
	m_bYLineAtLeft=FALSE;
	//set default graph position
	m_iGraphX=0;
	m_iGraphY=0;
	//set default graph size
	SetGraphSizePos(0, 0,G_MINGRAPHWIDTH,G_MINGRAPHHEIGHT);
	//set the axis scaling
	SetXAxisScale(G_DEFAULTXMIN,G_DEFAULTXMAX);
	SetYAxisScale(G_DEFAULTYMIN,G_DEFAULTYMAX);
	//set the legend and Title Texts
	SetDefaultGraphTitle();
	SetDefaultXLegend();
	SetDefaultYLegend();
	SetDefaultFunctionName();

	//other stuff
	m_pFunctionParams=NULL;
}

/////////////////// SetColorScheme ////////////////////////////////////////////
/*																			///
	SetColorScheme set the color scheme for the graph						///
	There are 8 colorschemes in total but default colour scheme is the		///
	same as as the WHITE colorscheme (which is really grey)					///
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CGraph::SetColorScheme(int Scheme, BOOL bRedraw)
{
	/*
	This sets up the colors for various graph elements
	*/
	switch (Scheme)
	{
	case G_DEFAULTSCHEME:
	case G_WHITESCHEME:
		m_crYTickColor=RGB(0,0,0);
		m_crXTickColor=RGB(0,0,0);
		m_crYLegendTextColor=RGB(0,0,0);
		m_crXLegendTextColor=RGB(0,0,0);
		m_crGraphTitleColor=RGB(0,0,0);
		m_crGraphPenColor=RGB(0,0,0);
		m_crGraphBkColor=RGB(192,192,192);
		m_crFunctionNameColor=RGB(0,0,0);
		m_crGridColor=RGB(220,220,220);	
		break;
	case G_REDSCHEME:
		m_crYTickColor=RGB(255,255,255);
		m_crXTickColor=RGB(255,255,255);
		m_crYLegendTextColor=RGB(255,200,200);
		m_crXLegendTextColor=RGB(255,150,150);
		m_crGraphTitleColor=RGB(255,255,255);
		m_crGraphPenColor=RGB(255,255,0);
		m_crGraphBkColor=RGB(128,0,0);
		m_crFunctionNameColor=RGB(255,0,0);
		m_crGridColor=RGB(200,0,0);	
		break;
	case G_BLUESCHEME:
		m_crYTickColor=RGB(255,255,0);
		m_crXTickColor=RGB(255,255,0);
		m_crYLegendTextColor=RGB(200,200,255);
		m_crXLegendTextColor=RGB(150,150,255);
		m_crGraphTitleColor=RGB(255,255,255);
		m_crGraphPenColor=RGB(0,255,255);
		m_crGraphBkColor=RGB(0,0,128);
		m_crFunctionNameColor=RGB(0,0,255);
		m_crGridColor=RGB(0,0,200);	
		break;
	case G_GREENSCHEME:
		m_crYTickColor=RGB(255,255,255);
		m_crXTickColor=RGB(255,255,255);
		m_crYLegendTextColor=RGB(200,255,200);
		m_crXLegendTextColor=RGB(150,255,150);
		m_crGraphTitleColor=RGB(0,255,255);
		m_crGraphPenColor=RGB(255,255,255);
		m_crGraphBkColor=RGB(0,128,0);
		m_crFunctionNameColor=RGB(255,0,255);
		m_crGridColor=RGB(0,200,0);	
		break;
	case G_CYANSCHEME:
		m_crYTickColor=RGB(255,0,255);
		m_crXTickColor=RGB(255,0,255);
		m_crYLegendTextColor=RGB(200,255,200);
		m_crXLegendTextColor=RGB(150,255,150);
		m_crGraphTitleColor=RGB(192,192,192);
		m_crGraphPenColor=RGB(255,255,0);
		m_crGraphBkColor=RGB(0,128,128);
		m_crFunctionNameColor=RGB(255,255,255);
		m_crGridColor=RGB(0,64,64);	
		break;
	case G_YELLOWSCHEME:
		m_crYTickColor=RGB(0,255,255);
		m_crXTickColor=RGB(0,255,255);
		m_crYLegendTextColor=RGB(200,200,0);
		m_crXLegendTextColor=RGB(255,255,0);
		m_crGraphTitleColor=RGB(192,192,192);
		m_crGraphPenColor=RGB(192,192,192);
		m_crGraphBkColor=RGB(128,128,0);
		m_crFunctionNameColor=RGB(255,255,255);
		m_crGridColor=RGB(0,64,64);	
		break;
	case G_MAGENTASCHEME:
		m_crYTickColor=RGB(192,192,255);
		m_crXTickColor=RGB(100,100,255);
		m_crYLegendTextColor=RGB(255,0,0);
		m_crXLegendTextColor=RGB(255,128,128);
		m_crGraphTitleColor=RGB(128,255,128);
		m_crGraphPenColor=RGB(255,255,255);
		m_crGraphBkColor=RGB(128,0,128);
		m_crFunctionNameColor=RGB(255,255,192);
		m_crGridColor=RGB(192,0,192);
		break;

	case G_TSMCHEME:

		m_crYLegendTextColor=RGB(255,255,0);
		m_crXLegendTextColor=RGB(255,255,0);
		m_crGraphTitleColor=RGB(255,255,0);				
		m_crFunctionNameColor=RGB(255,255,0);

		m_crYTickColor=RGB(0,255,0);
		m_crXTickColor=RGB(0,255,0);
		m_crGraphPenColor=RGB(0,255,0);
		m_crGridColor=RGB(0,75,0);
		m_crGraphBkColor=RGB(0,0,0);
		break;

	default:
		break;

	}
	if(bRedraw)
	{
		PaintGraph();
	}
}

///////////////////////////// CreateGraphFont /////////////////////////////////////
/*																				///	
	CreateGraphFont - Creates a font using the specified facename				///
	and point size																///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
 void CGraph::CreateGraphFont(CString FaceName, UINT size)
 {
 	//at this point we may not have a handle to a window - so to fill
 	//things like char-width & height where we need a dc we will get a 
	 //whole screen dc
 
 	CFont *poldfont;
 	HDC hDC;
 	CDC *dc;
 	TEXTMETRIC textmetrics;
 	if (!m_pWnd)//if we don't have a holding window
 	{
 		hDC=GetDC(0);//get a whole screen dc
 		dc=new CDC;
 		dc->Attach(hDC);
 	}
 	else
 	{
 		dc=m_pWnd->GetDC();
 	}
  	
	//detach old font if any
	m_GraphFont.Detach();
	//create the new one
	m_GraphFont.CreatePointFont(size*10,FaceName,dc);
	
 	poldfont=dc->SelectObject(&m_GraphFont);
 	
 	dc->GetTextMetrics(&textmetrics);
 	m_iCharHeight=textmetrics.tmHeight;
 	m_iCharWidth=textmetrics.tmAveCharWidth;
 
 	dc->SelectObject(poldfont);
 	if(!m_pWnd)
 	{
 		dc->Detach();
 		ReleaseDC(0,hDC);
 		delete dc;
 	}
 	else
 	{
 		m_pWnd->ReleaseDC(dc);
 	}
 	
 	m_iFontSize=size;
 	m_szFontFace=FaceName;

	//we need to rescale the graph
	SetXAxisScale(m_dXAxisMin, m_dXAxisMax);
	SetYAxisScale(m_dYAxisMin, m_dYAxisMax);
 }

//////////////////////////   SetGraphSizePos /////////////////////////////////////////////
/*																						///
	Sets the size of the entire graph size and position with the holding parent window.
	A value of -1 for any parameter means don't change that parameter.
	A value of 0 for the Width or Height(or any value between 0 the minimum graph width
	or height) means use the default graph width or height
*/
///////////////////////////////////////////////////////////////////////////////////
void CGraph::SetGraphSizePos(int xPos, int yPos, int Width, int Height)
{
	
	CRect rect;
	rect.left=m_iGraphX;
	rect.top=m_iGraphY;
	rect.right=rect.left+m_iGraphWidth;
	rect.bottom=rect.top+m_iGraphHeight;

	/*
	If xPos or yPos <=0 then  position will not be changed
	*/
	m_iGraphX=xPos < 0?m_iGraphX:xPos;
	m_iGraphY=yPos < 0?m_iGraphY:yPos;

	/*
	A negative number or zero means no change of current width or height
	if a +ive size  given thats smaller than the default min size for
	height or width then use the default min size for those parameters;
	*/
	Width= (Width>0 && Width <G_MINGRAPHWIDTH)?G_MINGRAPHWIDTH:Width;
	m_iGraphWidth = Width <= 0? m_iGraphWidth :Width;

	Height = (Height>0 && Height <G_MINGRAPHHEIGHT)?G_MINGRAPHHEIGHT:Height;
	m_iGraphHeight = Height <= 0? m_iGraphHeight : Height;

	//remember to rescale the graph
	SetXAxisScale(m_dXAxisMin,m_dXAxisMax);
	SetYAxisScale(m_dYAxisMin,m_dYAxisMax);
	
	//clear old graph from the screen
	if(m_pWnd)
	{
		m_pWnd->InvalidateRect(&rect,TRUE);
		m_pWnd->SendMessage(WM_PAINT,0,0);
	}
}	


////////////////////////// CalcTopMargin ///////////////////////////////////
/*
	Returns the spacing between the top adge of the graph and the top of the	
	plotting area.
*/
////////////////////////////////////////////////////////////////////////////
UINT CGraph::CalcTopMargin()
{
	/*the top margin comprises the following
	1* blank line + 1*Line (graph Title) + 
	1*blank line +1*Line (graph function);
	This margin is fixed
  */
	//!!!return 4*m_iCharHeight;
	return 4*m_iCharHeight;
}


/////////////////////  CalcBottommargin  //////////////////////////////////
/*
	Returns the space between the bottom of the grapgh and the bottom
	of the actual plotting area
*/
//////////////////////////////////////////////////////////////////////////
UINT CGraph::CalcBottomMargin()
{
	/* The bottom margin comprises the following
	1*blank line +1*Line(x axis title) + 1* line(x-values)+1*line(ticks);
	Mind you this is a 'minimum margin' - required below the x-axis
	There might be a lot of space below this depending on the x-axis scale
	*/
//	return 3*m_iCharHeight;
//	return m_iCharHeight;

	return 4*m_iCharWidth;
}

/////////////////////// CalcLeftmargin ////////////////////////////////////
/*
	Returns the margin between the left of the graph and the left of the
	plotiing area
*/
////////////////////////////////////////////////////////////////////////////
UINT CGraph::CalcLeftMargin()
{
	/*The Left margin comprise the following;
	1*blank line + 5*lines (for the yaxis values)+1*line (yticks);
	This margin is a 'minimum' value - there may be more space to the 
	left of this depending on negative scaling of the x-axis
	*/
//	return 13*m_iCharWidth;
//	return m_iCharWidth;

	return 4*m_iCharWidth;
}

///////////////////////// CalcRightMargin ////////////////////////////////////
/*
	Returns the space between the right side of the plotting area and the right
	sode of the graph
*/
//////////////////////////////////////////////////////////////////////////////
UINT CGraph::CalcRightMargin()
{
	/*This margin is simple - there is a blank line
	on the right side of width:
	*/
	//return m_iCharWidth;

	return 4*m_iCharWidth;
}

/////////////////////// SetXAxisScale /////////////////////////////////////////
/*
	This sets the min and max values of the x-axis (of the plotting area)
	It also sets what proportion of the x-range is denoted by one pixel
	a.ka. PixelsPerX
	It also caluclates where the x-origin (the x==0) point lies along
	the x-axis - if x does not pass through zero the x-origin gidline is shown
	at the top or bottom of the graph.
	(the x-origin point on screen is stored in the m_iOriginX parameter as 
	an offset from the LHS of the (total) graph
	26/2/2005 - The autofit parameter is always TRUE as scrolling is not yet
	implemented.
*/
//////////////////////////////////////////////////////////////////////////////
void CGraph::SetXAxisScale(double min, double max)
{
	//swap min and max if they are the wrong way around
	double temp,scale;
	if (max < min)
	{
		temp =min;
		min=max;
		max=temp;
	}
	//if min and max are the same (especially if they are both zero
	//it can be a problem - so give them a bit of room
	if(min==max)
	{
		max+= 0.1;
	}
	
	
	//set the member variables
	m_dXAxisMax=max;
	m_dXAxisMin=min;
	//
	UINT lmargin=CalcLeftMargin();
	UINT rmargin=CalcRightMargin();

	if (m_bAutofit) //Autofit is always true @ 26 Feb 2005
	{
		temp=max-min;//the spread of the x-axiz
		scale=(m_iGraphWidth-lmargin-rmargin)/temp;//calc pixels/x
		m_dPixelsPerX=scale;
	}
	else
	{
		m_dPixelsPerX=1;//1:1
	}
	
	//where would the x-origin be located?
	if( (min < 0) && (max >0))
	{
		m_iOriginX=abs(min) * m_dPixelsPerX + lmargin;
	}
	else if ( (min <0) && (max <=0))
	{
		m_iOriginX=(m_iGraphWidth)-rmargin;
	}
	else if (min >=0 && (max>=0))
	{
		m_iOriginX=lmargin;
	}
}

/////////////////////// SetYAxisScale ///////////////////////////////////////
/*
	This sets the min and max values of the y-axis (of the plotting area)
	It also sets what proportion of the grapgh axis pixels represents 1Y
	(PixelsPerY =plotheightinpixels/yrange)
	It also caluclates where the y-origin (the y==0) point lies along
	the y-axis - if y does not pass through zero the y-origin gidline is shown
	at the left or right of the graph (this can be overridden by using the SetYLineAtLeft()
	function.
	(the y-origin point on screen is stored in the m_iOriginY parameter as 
	an offset from the bottom of the (total) graph
	26/2/2005 - The autofit parameter is always TRUE as scrolling is not yet
	implemented.
*/	
/////////////////////////////////////////////////////////////////////////////
void CGraph::SetYAxisScale(double min, double max)
{
	double temp,scale;
	//swap min and max if they are the wrong way around
	if (max < min)
	{
		temp=min;
		min=max;
		max=temp;
	}

	//if min and max are the same (especially if they are both zero
	//it can be a problem - so give them a bit of room
	if(min==max)
	{
		max+= 0.1;
	}

	//set the member variables
	m_dYAxisMin=min;
	m_dYAxisMax=max;

	//calculate scaling
	UINT bmargin=CalcBottomMargin();
	UINT tmargin=CalcTopMargin();
	CRect dataarea=CalcDataArea();

	if (m_bAutofit)//Always TRUE as at 26/02/2005
	{
		temp=max-min;//the spread of the y-axiz
		scale=(m_iGraphHeight-(bmargin+tmargin))/temp;//calc pixels/x
		m_dPixelsPerY=scale;
	}
	else
	{
		m_dPixelsPerY=1;//1:1
	}

	//where should the Y origin be?

	if (min<0 && max >0)//if Y passes through zero
	{
		//from the bottom of the graph
		m_iOriginY=abs(min)*m_dPixelsPerY+bmargin;
	}
	else if(min<0 && max<=0)//if Y values are all negative
	{
		m_iOriginY=(dataarea.bottom-dataarea.top)+bmargin;
	}
	else if (min >=0 && max >=0)//if Y values are all positive
	{
		m_iOriginY=bmargin;
	}		

}


//////////////////////// PaintGraph //////////////////////////////////////
/*
	This paints the entire graph on to the holding window's
	client area;
	It does it in steps starting from the background and working forward.
	As the graph is NOT a window object in it's own right, it uses the
	display context of the holding window. If it has not been given a 
	pointer to the holding window, it will not paint.
	Any CGraph routine that paints to the screen, checks the window pointer
	first.
	The last thing to be painted is the plotting of the function data (if any)
*/
///////////////////////////////////////////////////////////////////////////
void CGraph::PaintGraph()
{
	//here we draw the graph
	//step 1: Draw the surrounding rectangle
	//for the whole graph

	if (m_pWnd==NULL) return;

	CRect rect;
	CPen pen, *oldpen;
	CDC *dc=m_pWnd->GetDC();
	//some useful calculations
	UINT lmargin=CalcLeftMargin();
	UINT rmargin=CalcRightMargin();
	UINT bmargin=CalcBottomMargin();
	UINT tmargin=CalcTopMargin();
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;

	//step 2: color the background
	CBrush brush,*poldbrush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	pen.CreatePen(PS_SOLID,1,m_crGraphBkColor);
	rect.left=m_iGraphX;
	rect.right =rect.left+m_iGraphWidth;
	rect.top=m_iGraphY;
	rect.bottom=rect.top+m_iGraphHeight;
	oldpen=dc->SelectObject(&pen);
	poldbrush=dc->SelectObject(&brush);
	dc->Rectangle(&rect);
	dc->SelectObject(oldpen);
	dc->SelectObject(poldbrush);
	pen.DeleteObject();// updated 12/9/2009 - fixed memory leak caused by using pen.Detach() 
	
	//step 3: Draw Grid if required
	DrawGrid();

	//step 4: Draw Axies
	//draw x-axis
	//dc->MoveTo(m_iGraphX+lmargin,Graphbottom-m_iOriginY);

	//UINT centerY=(Graphbottom-m_iGraphY)/2+CalcBottomMargin()+CalcTopMargin();
	

	UINT centerY=this->m_iGraphY+(this->m_iGraphHeight-CalcTopMargin()-CalcBottomMargin())/2+CalcTopMargin();



	dc->MoveTo(m_iGraphX+lmargin,centerY);


//	this->m_iGraphHeight;
//	this->m_iGraphWidth;
//	this->m_iGraphX;
//	this->m_iGraphY;


	pen.CreatePen(PS_SOLID,1,m_crXTickColor);
	oldpen=dc->SelectObject(&pen);

//	dc->LineTo(Graphright-rmargin,Graphbottom-m_iOriginY);

	dc->LineTo(Graphright-rmargin,centerY);

	dc->SelectObject(oldpen);
	pen.DeleteObject(); //// updated 12/9/2009 - fixed memory leak caused by using pen.Detach() 


	//draw the Y Axis

	UINT centerX=m_iGraphX+this->m_iGraphWidth/2;

	pen.CreatePen(PS_SOLID,1,m_crXTickColor);
	oldpen=dc->SelectObject(&pen);
	if (!m_bYLineAtLeft)
	{
		//draw the Y Line so that it intercepts
		//the x-line like crosshairs

		//dc->MoveTo(m_iGraphX+m_iOriginX,m_iGraphY+tmargin);
		//dc->MoveTo(m_iGraphX,m_iGraphY);
		//dc->LineTo(m_iGraphX,m_iGraphY+100);

		dc->MoveTo(centerX,m_iGraphY+CalcTopMargin());

		dc->LineTo(centerX,m_iGraphY+m_iGraphHeight-CalcBottomMargin());

		//dc->LineTo(m_iGraphX+m_iOriginX,Graphbottom-bmargin);

		//dc->MoveTo(m_iGraphX+m_iOriginX,centerX);
		//dc->LineTo(m_iGraphX+m_iOriginX,centerX);


	}
	else
	{
		//draw the Y Line at the LHS
		dc->MoveTo(m_iGraphX+lmargin,m_iGraphY+tmargin);
		dc->LineTo(m_iGraphX+lmargin,Graphbottom-bmargin);
	}


//Рисуем внутренний квадрат
//Поправка на погрешность пикселей

#define dxy 2  

//Шаг по Х Y на 10 клеток
	UINT stepx=(m_iGraphWidth-CalcLeftMargin()-CalcLeftMargin())/10;
	UINT stepy=(this->m_iGraphHeight-CalcTopMargin()-CalcBottomMargin())/10;

//Первая точка внутреннего квадрата
	UINT x1=m_iGraphX+CalcLeftMargin()+stepx*2+stepx/2+dxy;
	UINT y1=m_iGraphY+CalcTopMargin()+stepy*2+stepy/2+dxy;

//Из левой верхней точки до оси Y
	dc->MoveTo(x1,y1);
	dc->LineTo(x1,centerY);

//От оси у до нижней левой точки
	UINT y2=centerY+stepy*2+stepy/2+dxy;
	dc->LineTo(x1,y2);

//От нижней левой точки до оси Y
	UINT x2=centerX;
	dc->LineTo(x2,y2);

//От оси Y до нижней правой точки
	UINT x3=centerX+stepx*2+stepx/2+dxy;
	dc->LineTo(x3,y2);

//Вверх до оси х
	UINT y3=centerY;
	dc->LineTo(x3,y3);

//Вверх до правой верхней точки 
	UINT y4=centerY-stepy*2-stepy/2-dxy;
	dc->LineTo(x3,y4);

//Замыкаем
	dc->LineTo(x1,y1);

	dc->SelectObject(oldpen);
	pen.DeleteObject(); // updated 12/9/2009 - fixed memory leak caused by using pen.Detach() 

	//step 5: draw ticks if required
	DrawTicks();

	//step 6: Write Graph title
	DrawGraphTitle();

/*
	//step 7: Write Function name
	DrawFunctionName();

	//step 8: Write x-legend
	DrawXLegend();

	//step 9: Write y legend
	//doing this is very similar to doing the graph title or the
	//x-legend but the Y axis is either at the LHS or set to match the
	//x-origin
	DrawYLegend();

	//step 10: write the x & y axes values
	DrawXAxisNumbers();
	DrawYAxisNumbers();

	//Step 11
	//draw Function

	DrawFunction();
*/

	//Cleanup
	m_pWnd->ReleaseDC(dc);
    brush.DeleteObject(); // updated 12/9/2009 - fixed memory leak  

	if (m_DrawGaps) if (drawgaps!=NULL) (*drawgaps)();
}

//////////////////// DrawGrid ////////////////////////////////////////////////
/*
The grid comprises two parts - the rectangle drawn around the plotting area
and the vertical&horizontal gridlines.
The outline rectangle is always drawn, the drawing of the gidlines
is contrlled by the m_bShowGrid member parameter using the ShowGrid function.
The grid color is  m_crGridColor.
*/
//////////////////////////////////////////////////////////////////////////////
void CGraph::DrawGrid()
{
	
	if(!m_pWnd)
	{
		return;
	}
	
	/*
	Always draw the dataarea outline rectangle
	*/
	
	CRect dataarea=CalcDataArea();;//where the graph data is actually drawn

	//need a pen of colour m_crGridColor
	CPen pen, *poldpen;
	pen.CreatePen(PS_SOLID,1,m_crGridColor);

	CDC *pdc=m_pWnd->GetDC();
	//to make a rectangle outline we have to use a polyline
	//need an array of points
	//a recatangle comprises 5 point
	CPoint points[5];
	//topleft
	points[0].x=dataarea.left;
	points[0].y=dataarea.top;
	//topright
	points[1].x=dataarea.right;
	points[1].y=dataarea.top;
	//bottomright
	points[2].x=dataarea.right;
	points[2].y=dataarea.bottom;
	//leftbottom
	points[3].x=dataarea.left;
	points[3].y=dataarea.bottom;
	//back to topleft
	points[4].x=dataarea.left;
	points[4].y=dataarea.top;

	poldpen=pdc->SelectObject(&pen);
	pdc->Polyline(points,5);//draw the outline rectangle

	/* Now check whether the grid itself should
	be shown
	*/
	if(!m_bShowGrid)
	{
		pdc->SelectObject(poldpen);
        pen.DeleteObject();// updated 12/9/2009 - fixed memory leak 
		m_pWnd->ReleaseDC(pdc);
		return;
	}

	//draw the X-axis gridlines
	//note x-axis grid lines run top tom bottom
	double GridSpacing;
	GridSpacing=CalcXAxisGridAndTicks();
	int n; //for the loop
	for(n=1; n<G_NUMTICKSANDGRID;n++)
	{
		pdc->MoveTo(dataarea.left+GridSpacing*n , dataarea.top);
		pdc->LineTo(dataarea.left+GridSpacing*n,dataarea.bottom);
	}
	//do the Y grid lines
	//note Y gridlines run left - right
	GridSpacing=CalcYAxisGridAndTicks();
	for(n=1; n<G_NUMTICKSANDGRID;n++)
	{
		pdc->MoveTo(dataarea.left , dataarea.top+GridSpacing*n);
		pdc->LineTo(dataarea.right,dataarea.top+GridSpacing*n);
	}

	pdc->SelectObject(poldpen);
    pen.DeleteObject();// updated 12/9/2009 - fixed memory leak 
	m_pWnd->ReleaseDC(pdc);
}

////////////////////// DrawTicks ///////////////////////////////////////////
/*
	DrawTicks does two things - it draws the x & Y axis lines and it also
	draws the little 'tick' lines.
	The axis lines are always shown but the 'ticks' are controlled by the
	m_bShowTicks member (using the ShowTicks function);
	The length of the ticks are set by the #define in the header file
	the axis lines and ticks use the same color (m_crXTickColor for
	the x-axis and m_crYTickColor for the y-axis)
*/
//*****************************************************************
void CGraph::DrawTicks()
{

	//pretty much the same as showing the grid because the ticks and
	//tick align
	if (!m_pWnd)
	{
		return;
	}
	if (!m_bShowTicks)
	{
		return;
	}
	CPen TickPen, *poldpen;
		
	CRect dataarea=CalcDataArea();
	UINT GraphBottom=m_iGraphY+m_iGraphHeight;

	CDC *pdc=m_pWnd->GetDC();
	//start with the x-axis
	TickPen.CreatePen(PS_SOLID,1,m_crXTickColor);
	poldpen=pdc->SelectObject(&TickPen);
	double GridSpacing=CalcXAxisGridAndTicks();
	//the ticklines vertically straddle the x-axis
	//two problems though - if the x-line is at or very close to the
	//top or bottom of the dataarea

	UINT xtoptick=((GraphBottom-m_iOriginY)-dataarea.top < G_TICKLENGTH/2)?(GraphBottom-m_iOriginY)-dataarea.top:G_TICKLENGTH/2;
	UINT xbottick=(dataarea.bottom-(GraphBottom-m_iOriginY) <G_TICKLENGTH/2)?dataarea.bottom-(GraphBottom-m_iOriginY):G_TICKLENGTH/2;

	int n;

	//UINT centerY=(dataarea.bottom-m_iGraphY)/2+CalcBottomMargin()+CalcTopMargin();
	//UINT centerY=this->m_iGraphY+this->m_iGraphHeight/2;

	UINT centerY=this->m_iGraphY+(this->m_iGraphHeight-CalcTopMargin()-CalcBottomMargin())/2+CalcTopMargin();


	UINT centerX=m_iGraphX+this->m_iGraphWidth/2;

/*
	for(n=1;n<G_NUMTICKSANDGRID;n++)
	{
		//loop and do the ticks
		//topticks
		//pdc->MoveTo(dataarea.left+n*GridSpacing,GraphBottom-m_iOriginY);
		pdc->MoveTo(dataarea.left+n*GridSpacing,centerY);

		//pdc->LineTo(dataarea.left+n*GridSpacing,GraphBottom-m_iOriginY-xtoptick);
		pdc->LineTo(dataarea.left+n*GridSpacing,centerY-xtoptick);

		//bottom ticks
		//pdc->LineTo(dataarea.left+n*GridSpacing,GraphBottom-m_iOriginY+xbottick);
		pdc->LineTo(dataarea.left+n*GridSpacing,centerY+xtoptick);

	}
*/	


	for(n=1;n<G_NUMTICKSANDGRID*2;n++)
	{
		//loop and do the ticks
		//topticks
		//pdc->MoveTo(dataarea.left+n*GridSpacing,GraphBottom-m_iOriginY);
		pdc->MoveTo(dataarea.left+n*GridSpacing/2,centerY);

		//pdc->LineTo(dataarea.left+n*GridSpacing,GraphBottom-m_iOriginY-xtoptick);
		pdc->LineTo(dataarea.left+n*GridSpacing/2,centerY-xtoptick);

		//bottom ticks
		//pdc->LineTo(dataarea.left+n*GridSpacing,GraphBottom-m_iOriginY+xbottick);
		pdc->LineTo(dataarea.left+n*GridSpacing/2,centerY+xtoptick);

	}



	//now do the x axis ticks	
	pdc->SelectObject(poldpen);
	TickPen.DeleteObject();// updated 12/9/2009 - fixed memory leak 
	TickPen.CreatePen(PS_SOLID,1,m_crYTickColor);
	poldpen=pdc->SelectObject(&TickPen);
	GridSpacing=CalcYAxisGridAndTicks();
	//the tick horizontally straddle the the Y axis
	//some problems though - if the y-axis is at or very close to the
	//left or right of the data area or if the Y-line memeber is set to left
	//handside

	UINT ylefttick=( (m_iGraphX+m_iOriginX)-dataarea.left <G_TICKLENGTH/2)? (m_iGraphX+m_iOriginX)-dataarea.left:G_TICKLENGTH/2;
	UINT yrighttick=( dataarea.right-(m_iGraphX+m_iOriginX) <G_TICKLENGTH/2)? dataarea.right-(m_iGraphX+m_iOriginX):G_TICKLENGTH/2;
	//check for the special case where the y-axis has been forced to the left
	if(m_bYLineAtLeft)
	{
		ylefttick=0;
	}

	int x,y;

	if(m_bYLineAtLeft)
	{
		x=dataarea.left;
		y=dataarea.bottom;
		//y=centerY;
	}
	else
	{
		x=m_iGraphX+m_iOriginX;
		y=dataarea.bottom;
		//y=centerY;
	}


/*
	for(n=1;n<G_NUMTICKSANDGRID;n++)
	{
		//Loop and do the y axis ticks

		//pdc->MoveTo(x,y-(n*GridSpacing));

		pdc->MoveTo(centerX,y-(n*GridSpacing));

		//do left side tick
		//pdc->LineTo(x-ylefttick,y-(n*GridSpacing));

		pdc->LineTo(centerX-ylefttick,y-(n*GridSpacing));

		//do rightside tick
		//pdc->LineTo(x+yrighttick,y-(n*GridSpacing));
		pdc->LineTo(centerX+yrighttick,y-(n*GridSpacing));
	}
*/

	for(n=1;n<G_NUMTICKSANDGRID*2;n++)
	{
		//Loop and do the y axis ticks

		//pdc->MoveTo(x,y-(n*GridSpacing));

		pdc->MoveTo(centerX,y-(n*GridSpacing/2));

		//do left side tick
		//pdc->LineTo(x-ylefttick,y-(n*GridSpacing));

		pdc->LineTo(centerX-ylefttick,y-(n*GridSpacing/2));

		//do rightside tick
		//pdc->LineTo(x+yrighttick,y-(n*GridSpacing));
		pdc->LineTo(centerX+yrighttick,y-(n*GridSpacing/2));
	}

	//cleanup
	pdc->SelectObject(poldpen);
    TickPen.DeleteObject();// updated 12/9/2009 - fixed memory leak 
	m_pWnd->ReleaseDC(pdc);
}


///////////// CalcDataArea ////////////////////////////////////////////////////
/*
	This function calculates the actual plotting area of the graph
	this is the graph area minus the top,bottom,left & right margins
	Returns:
	CRect with the plotting area (in client area co-ords)
*/
///////////////////////////////////////////////////////////////////////////////
CRect CGraph::CalcDataArea()
{
	CRect dataarea;

	dataarea.left=m_iGraphX+CalcLeftMargin();
	dataarea.right=m_iGraphX+m_iGraphWidth-CalcRightMargin();

	dataarea.top=m_iGraphY+CalcTopMargin();
	dataarea.bottom=m_iGraphY+m_iGraphHeight-CalcBottomMargin();

	return dataarea;
}

///////////////////// CalcXAxisGridAndTicks ////////////////////////////////
/*
	This calculates the positions of the vertical gridlines of the plot area.
	This is also used for x-axis ticks as the ticks line up with
	the gridlines.
	How many they are is determined by the G_NUMTICKSANDGRID  define
	in the header file
	Return:
	a double denoting the x-asis grid spacing
*/
///////////////////////////////////////////////////////////////////////////
double CGraph::CalcXAxisGridAndTicks()
{
	//the placing of the ticks co-incide with gridlines
	CRect rect=CalcDataArea();
	return ((double)rect.right-(double)rect.left)/(double)G_NUMTICKSANDGRID;

}

///////////////////  CalcYAxisGridAndTicks  //////////////////////////////////
/*
	This calculates the positions of the horizontal gridlines of the plot area.
	This is also used for y-axis ticks as the ticks line up with
	the gridlines.
	How many they are is determined by the G_NUMTICKSANDGRID  define
	in the header file
	Return:
	A double denoting the howizontal gridline spacing
*/
//////////////////////////////////////////////////////////////////////////////
double CGraph::CalcYAxisGridAndTicks()
{
	//the placing of the ticks co-incide with gridlines
	CRect rect=CalcDataArea();
	return ((double)rect.bottom-(double)rect.top)/(double)G_NUMTICKSANDGRID;
}


///////////////////// DrawGraphTitle ////////////////////////////////////
/*
	This draws the Graph title string in the color m_crGraphTitleColor
	This title is centred abobe the plotting area.
*/
////////////////////////////////////////////////////////////////////////
void CGraph::DrawGraphTitle()
{
	//The graph title is drawn one character line down
	//centered left right between the left and right margins
	if (m_pWnd==NULL)
	{
		return;
	}

	UINT lmargin=CalcLeftMargin();
	UINT rmargin=CalcRightMargin();

	CRect rect;

	rect.left=m_iGraphX+lmargin+5;

	rect.top=m_iGraphY+m_iCharHeight;
	rect.right=m_iGraphX+m_iGraphWidth-rmargin;
	rect.bottom=rect.top+m_iCharHeight;

	//draw the title using the specified colorscheme
	//using the graph font
	CDC *pdc=m_pWnd->GetDC();
	CFont *poldfont;

	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//or bottom of a given rect
	pdc->FillRect(&rect,&brush);

	pdc->SetBkMode(TRANSPARENT);

	pdc->SetTextColor(m_crGraphTitleColor);

	poldfont=pdc->SelectObject(&m_GraphFont);

	//pdc->DrawText(m_szGraphTitle,&rect,DT_CENTER|DT_END_ELLIPSIS);

	CString sscale("");
	//sscale.Format(L"%d                      0                       %d",m_TARGETSCALE*(-1),m_TARGETSCALE);

	//pdc->DrawText(L"-300                      0                       300",&rect,DT_NOCLIP|DT_LEFT);
	//pdc->DrawText(sscale,&rect,DT_NOCLIP|DT_LEFT);


	sscale.Format(L"%d",m_TARGETSCALE*(-1));
	pdc->DrawText(sscale,&rect,DT_NOCLIP|DT_LEFT);
	sscale.Format(L"%d",0);
	pdc->DrawText(sscale,&rect,DT_NOCLIP|DT_CENTER);
	sscale.Format(L"%d",m_TARGETSCALE*(1));
	pdc->DrawText(sscale,&rect,DT_NOCLIP|DT_RIGHT);


	


/*
	rect.top+=20;
	pdc->DrawText(L"-300",&rect,DT_NOCLIP|DT_LEFT);
	pdc->DrawText(L"0",&rect,DT_NOCLIP|DT_CENTER);
	pdc->DrawText(L"300",&rect,DT_NOCLIP|DT_RIGHT);
*/	

	//cleanup
	pdc->SelectObject(poldfont);
    brush.DeleteObject();// updated 12/9/2009 - fixed memory leak 
	m_pWnd->ReleaseDC(pdc);
}

///////////////////// DrawXLegend //////////////////////////////////////////////
/*
	The Xaxis legend is drawn below the plotting area
	below the x-axis scale numbers
*/
///////////////////////////////////////////////////////////////////////////////
void CGraph::DrawXLegend()
{
	//The x legend is drawn  below the data area
	if (m_pWnd==NULL)
	{
		return;
	}
	UINT lmargin=CalcLeftMargin();
	UINT rmargin=CalcRightMargin();
	CRect rect;
	rect.left=m_iGraphX+lmargin;
	rect.right=m_iGraphX+m_iGraphWidth-rmargin;
	rect.top=m_iGraphY+m_iGraphHeight-2*m_iCharHeight+1;
	rect.bottom=rect.top+m_iCharHeight;
	//just 2 b safe move the rect down a bit so we don't
	//interfere with the yaxis numbers
	rect.top+=1;
	rect.bottom+=1;

	//draw the title using the specified colorscheme
	//using the graph font
	CDC *pdc=m_pWnd->GetDC();
	CFont *poldfont;

	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	rect.InflateRect(0,0,1,1);//bcause fillrect does not go right to the right
	//or bottom of a given rect
	pdc->FillRect(&rect,&brush);
	rect.DeflateRect(0,0,1,1);

	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(m_crXLegendTextColor);
	poldfont=pdc->SelectObject(&m_GraphFont);
	pdc->DrawText(m_szXLegendText,&rect,DT_CENTER|DT_END_ELLIPSIS);
	
	//cleanup
	pdc->SelectObject(poldfont);
    brush.DeleteObject();// updated 12/9/2009 - fixed memory leak 
	m_pWnd->ReleaseDC(pdc);
}

//////////////////////// DrawYlegend //////////////////////////////////////////
/*
	The Y legend is drawn at the LHS of the plot area between the yaxis max 
	and yaxis min scale
*/
///////////////////////////////////////////////////////////////////////////////
void CGraph::DrawYLegend()
{
	//The Y legend is drawn down the left hand side just
	//outside the data area
	if (m_pWnd==NULL)
	{
		return;
	}
	UINT tmargin=CalcTopMargin();
	UINT bmargin=CalcBottomMargin();
	CRect rect;

	int x_offset=8;

	rect.left=m_iGraphX+m_iCharWidth+x_offset;
	rect.right=rect.left+11*m_iCharWidth+x_offset;

	int y_offset=50;

	rect.top=m_iGraphY+tmargin+2*m_iCharHeight-y_offset;
	rect.bottom=m_iGraphY+m_iGraphHeight-bmargin-2*m_iCharHeight-y_offset-90;

	//draw the title using the specified colorscheme
	//using the graph font
	CDC *pdc=m_pWnd->GetDC();
	CFont *poldfont;

	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//or bottom of a given rect
	pdc->FillRect(&rect,&brush);


	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(m_crYLegendTextColor);
	poldfont=pdc->SelectObject(&m_GraphFont);
	pdc->DrawText(m_szYLegendText,&rect,DT_RIGHT|DT_WORDBREAK|DT_END_ELLIPSIS);
	
	//cleanup
	pdc->SelectObject(poldfont);
	m_pWnd->ReleaseDC(pdc);
}

///////////////////// DrawXAxisNumbers //////////////////////////////////////
/*
	The X axis scale numbers are drawm directly below the plot area.
	Three numbers are drawn, min, middle and max scaling
*/
////////////////////////////////////////////////////////////////////////////
void CGraph::DrawXAxisNumbers()
{
	//we draw three sets of numbers xmin, xmiddle and xmax
	//we will limit them to 7 digits
	//
	if (!m_pWnd)
	{
		return;
	}
	CString astring;
	CRect rect,dataarea;
	CFont *poldfont;
	CDC *pdc;
	
	pdc=m_pWnd->GetDC();
	poldfont=pdc->SelectObject(&m_GraphFont);
	pdc->SetTextColor(m_crXLegendTextColor);
	pdc->SetBkMode(TRANSPARENT);

	dataarea=CalcDataArea();
	//do the left side (min) left aligned
	rect.left=dataarea.left;
	rect.top=m_iGraphY+m_iGraphHeight-3*m_iCharHeight;
	rect.right=rect.left+8*m_iCharWidth;//allow for 7 characters
	rect.bottom=rect.top+m_iCharHeight;
	rect.top+=1;
	rect.bottom+=1;
	//clear any old text
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	pdc->FillRect(&rect,&brush);
	//format and print the number
	astring.Format(L"%.4g",m_dXAxisMin);
	pdc->DrawText(astring,&rect,DT_NOCLIP|DT_LEFT);
	//do the half way point - centre aligned
	rect.left=dataarea.left+((dataarea.right-dataarea.left)/2);
	rect.left=rect.left-4*m_iCharWidth;
	rect.right=rect.left+8*m_iCharWidth;
	rect.top+=1;
	rect.bottom+=1;
	pdc->FillRect(&rect,&brush);	
	astring.Format(L"%.4g",(m_dXAxisMin+m_dXAxisMax)/2);
	pdc->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
	//now do the righthand side (max); right aligned
	rect.left=dataarea.right-8*m_iCharWidth;
	rect.right=dataarea.right;
	rect.top+=1;
	rect.bottom+=1;;
	pdc->FillRect(&rect,&brush);	
	astring.Format(L"%.4g",m_dXAxisMax);// note the lack of - in the format
	pdc->DrawText(astring,&rect,DT_NOCLIP|DT_RIGHT);

	//cleanup
	pdc->SelectObject(poldfont);
    brush.DeleteObject();
	m_pWnd->ReleaseDC(pdc);

	
}

///////////////////// DrawYAxisNumbers //////////////////////////////////////
/*
	The Y axis scale is drawn on the LHS of the plot area.
	Only two numbers are drawn - min and max to allow for the Y-axis
	legend.
*/
/////////////////////////////////////////////////////////////////////////////
void CGraph::DrawYAxisNumbers()
{
	//we will only do two sets of numbers min and max because
	//if we print the halfway point it will cross the yaxis label
		if (!m_pWnd)
	{
		return;
	}
	CString astring;
	CRect rect,dataarea;
	CFont *poldfont;
	CDC *pdc;
	
	pdc=m_pWnd->GetDC();
	poldfont=pdc->SelectObject(&m_GraphFont);
	pdc->SetTextColor(m_crYLegendTextColor);
	pdc->SetBkMode(TRANSPARENT);

	dataarea=CalcDataArea();

	//do the max value
	rect.left=m_iGraphX+m_iCharWidth;
	rect.top=dataarea.top;
	rect.right=rect.left+12*m_iCharWidth;//allow for  characters
	rect.bottom=rect.top+m_iCharHeight;
	rect.DeflateRect(0,0,1,0);//otherwise we interfere with the dataarea bottom line
	//clear any old text
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	pdc->FillRect(&rect,&brush);
	//format and print the number
	astring.Format(L"%.4g",m_dYAxisMax);
	pdc->DrawText(astring,&rect,DT_RIGHT);
	//do the bottom - (min)
	rect.top=dataarea.bottom-m_iCharHeight;
	rect.bottom=rect.top+m_iCharHeight;
	rect.DeflateRect(0,0,1,0);
	pdc->FillRect(&rect,&brush);	
	astring.Format(L"%.4g",(m_dYAxisMin));
	pdc->DrawText(astring,&rect,DT_RIGHT);

	//cleanup
	pdc->SelectObject(poldfont);
    brush.DeleteObject();// updated 12/9/2009 - fixed memory leak 
	m_pWnd->ReleaseDC(pdc);
}

////////////////// DrawFunctionName /////////////////////////////////////
/*
	The function Name is drawn just above the plot area and below the Graph Title;
	Usually the Function Name is autoset - but it's just another text string
	which can be change programitcally.
*/
////////////////////////////////////////////////////////////////////////////
void CGraph::DrawFunctionName()
{
	//The function name is drawn just above the dataarea
	//centered, using it's own color
	if (!m_pWnd)
	{
		return;
	}
	CRect dataarea=CalcDataArea();
	CRect rect;
	rect.top=dataarea.top-m_iCharHeight;
	rect.bottom=dataarea.top;
	rect.left=dataarea.left;
	rect.right=dataarea.right;

	//move up a pixel so as not to get too close to the 
	//dataarea top
	rect.bottom-=2;
	rect.top-=2;

	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	CFont *poldfont;

	CDC *pdc=m_pWnd->GetDC();
	pdc->FillRect(&rect,&brush);

	poldfont=pdc->SelectObject(&m_GraphFont);
	pdc->SetTextColor(m_crFunctionNameColor);
	pdc->SetBkMode(TRANSPARENT);
	pdc->DrawText(m_szFunctionNameText,&rect,DT_CENTER|DT_END_ELLIPSIS);

	//cleanup
	pdc->SelectObject(poldfont);
    brush.DeleteObject();
	m_pWnd->ReleaseDC(pdc);
}

////////////////// SetdefaultColorScheme  //////////////////////////////////
/*
	sets the colours of various bits back to the default colours
*/
///////////////////////////////////////////////////////////////////////////
void CGraph::SetDefaultColorScheme()
{
	SetColorScheme(G_DEFAULTSCHEME);
}


//////////////////////  SetDefaultGraphTitle //////////////////////////////
/*
	Sets the default graph title
*/
///////////////////////////////////////////////////////////////////////////
void CGraph::SetDefaultGraphTitle()
{
	m_szGraphTitle=L"Graph Title goes here";
}


//////////// SetDefaultXLegend/////////////////////////////////////////////
/*

*/
///////////////////////////////////////////////////////////////////////////
void CGraph::SetDefaultXLegend()
{
	m_szXLegendText=L"X Axis Legend Text here";
}

///////////////// SetDefaultYLegend ///////////////////////////////////////
/*

*/
///////////////////////////////////////////////////////////////////////////
void CGraph::SetDefaultYLegend()
{
	m_szYLegendText=L"Y Axis Legend text  here";
}



////////////////////// SetDefaultFunctionname  //////////////////////////////
/*
*/
/////////////////////////////////////////////////////////////////////////////
void CGraph::SetDefaultFunctionName()
{
	m_szFunctionNameText="Function Name (or extra text)";
}

///////////////////////// SetGraphTitle /////////////////////////////////////
/*
	This sets the GraphTitle
	Takes:
	CString
*/
/////////////////////////////////////////////////////////////////////////////
void CGraph::SetGraphTitle(CString GraphTitle)
{
	m_szGraphTitle=GraphTitle;
	DrawGraphTitle();
	
}


//////////////////////// SetXLegendText  ///////////////////////////////////
/*
	Sets the X axis legend text
*/
////////////////////////////////////////////////////////////////////////////
void CGraph::SetXLegendText(CString XText)
{
	m_szXLegendText=XText;
	DrawXLegend();
}


////////////////////// SetYLegendText /////////////////////////////////////
/*
	Sets the Y axis legend text
*/
///////////////////////////////////////////////////////////////////////////
void CGraph::SetYLegendText(CString YText)
{
	m_szYLegendText=YText;
	DrawYLegend();
}


////////////////////////////// SetFunctionName /////////////////////////////
/*
	Sets the Function name text string
*/
////////////////////////////////////////////////////////////////////////////
void CGraph::SetFunctionName(CString FunctionName)
{
	m_szFunctionNameText=FunctionName;
	DrawFunctionName();
}

//////////////////// SetYLineAtLeft ////////////////////////////////////////
/*
	The Y axis line can be forced to the LHS of the 
	plot area using this function
*/
/////////////////////////////////////////////////////////////////////////////

void CGraph::SetYLineAtLeft(BOOL AtLeft)
{
	BOOL bprevious=m_bYLineAtLeft;
	m_bYLineAtLeft=AtLeft;
	//if there is a change in the Y line position then we will have to
	//redraw the graph
	if (m_bYLineAtLeft != bprevious)
	{
		PaintGraph();
	}
}

////////////////////////// ShowGrid /////////////////////////////////////////
/*
	This switches the grid on or off as set by the bShow parameter
	The graph is repainted to match
*/
/////////////////////////////////////////////////////////////////////////////
void CGraph::ShowGrid(BOOL bShow)
{
	//this is public function
	//show the graph grid if bShow==TRUE
	//or vice-versa
	BOOL bprevious=m_bShowGrid;
	m_bShowGrid=bShow;
	//if there is a change then repaint
	if (m_bShowGrid !=bprevious)
	{
		PaintGraph();
	}
}

/////////////////// ShowTicks ////////////////////////////////////////////
/*
	This switches the x and y axis 'ticks' on or off as set by the
	bShow parameter
*/
///////////////////////////////////////////////////////////////////////////
void CGraph::ShowTicks(BOOL bShow)
{
	//this is public function
	//show the graph grid if bShow==TRUE
	//or vice-versa
	BOOL bprevious=m_bShowTicks;
	m_bShowTicks=bShow;
	//if there is a change then repaint
	if (m_bShowTicks!=bprevious)
	{
		PaintGraph();
	}

}

////////////////////// ConvertToGraphCoords ///////////////////////////////////
/*																			///
ConvertToGraphCoords (double x, double y) will take the						///
result of some calculation as given by x and y and return where				///			
they should be plotted on the graph.										///
As we are can only plot a whole pixel the return value is of				///
type LONG (as apposed to double).											///
The Y-pixel is in the HIWORD and x-pixel in the LOWORD of the				///
return.																		///
Note that depending on the scale of the graph, the return coords			///
of a single pixel could be one of many.										///
For example: if the x axis is 400 pixels wide, but is scaled to				///
represent 1000, then each pixel represent 2.5 in the real world.			///
So to pixel 398 represent the real world values of 995 to 996.5 inc.		///
																			///
*/																			///
///////////////////////////////////////////////////////////////////////////////
LONG CGraph::ConvertToGraphCoords(double x, double y)
{
	
	LONG result =-1;
	//to be plottable on the graph the given x-value must be between
	//x-min and x-max
	if(x < m_dXAxisMin || x > m_dXAxisMax)
	{
		return result;
	}
	if( y <m_dYAxisMin || y > m_dYAxisMax)
	{
		return result;
	}
	//double x1 = x*m_dPixelsPerX;
	//double y1 = y*m_dPixelsPerY;

	//calc the abs difference between Xmin and x;
	double xdif = abs(m_dXAxisMin - x);
	//calc the abs difference between Ymin and y;
	double ydif = abs(m_dYAxisMin - y);

	//find the dataarea
	CRect rect=CalcDataArea();

	 int xpos=rect.left+(xdif*m_dPixelsPerX); //from left
	int ypos=rect.bottom-(ydif*m_dPixelsPerY); //from bottom

	result=MAKELONG(xpos,ypos);	

	return result;
}


///////////////////////////// DoFunction //////////////////////////////////////
/*																			///
	The user fills in a G_FUNCTIONSTRUCT relevant to a function and passes	///
	a pointer to it to this function.										///
	The pointer to this G_FUNCTIONSTRUCT is saved in a member variable		///
	This function does some preliminary stuff  and if there are no			///
	obvious problems it then calls the PaintGraph function.					///
																			///
	Returns:																///
	FALSE if there are no problems											///
*/																			///
///////////////////////////////////////////////////////////////////////////////
BOOL CGraph::DoFunction(G_FUNCTIONSTRUCT *pFunctionParams)
{
	//do some checks first to see if good data has been passed
	if(pFunctionParams->FuncType > NUMFUNCTIONS)
	{
		return FALSE;
	}
	if(pFunctionParams->ChartType > NUMCHARTTYPES)
	{
		return FALSE;
	}
	if( (pFunctionParams->pPlotXYItems==NULL) && (pFunctionParams->FuncType==G_PLOTXY))
	{
		return FALSE;
	}
	if( (pFunctionParams->pPlotXYItems==NULL) && (pFunctionParams->FuncType==G_PLOTDEV_PERCENT))
	{
		return FALSE;
	}
	if( (pFunctionParams->pPlotXYItems==NULL) && (pFunctionParams->FuncType==G_PLOTDEV_ABS))
	{
		return FALSE;
	}


	m_pFunctionParams=pFunctionParams;
	//Set chart title and other text items
	SetGraphTitle((CString)m_pFunctionParams->szGraphTitle);
	SetXLegendText((CString)m_pFunctionParams->szXLegend);
	SetYLegendText((CString)m_pFunctionParams->szYLegend);

	switch(m_pFunctionParams->FuncType)
	{
		case G_SINX:
		SetFunctionName((CString)"function: y=sin(x)");
		break;
		case G_XSQUARED:
		SetFunctionName((CString)"function: y= X^2");
		break;
		case G_XCUBED:
		SetFunctionName((CString)"function: y=x^3");
		break;
		case G_MXPLUSC:
		SetFunctionName((CString)"function: y=mx+c");
		break;
		case G_COSX:
		SetFunctionName((CString)"function: y=cos(x)");
		break;
		case G_LOGEX:
		SetFunctionName((CString)"function: y=Loge(x)");
		break;
		case G_PLOTXY:
		SetFunctionName((CString)"Plotting x,y");
		break;
		case G_PLOTDEV_PERCENT:
		SetFunctionName((CString)"Deviation percent");
		break;
		case G_PLOTDEV_ABS:
		SetFunctionName((CString)"Deviation absolute");
		break;
		case G_HISTOGRAM_PERCENT:
		SetFunctionName(L"Histogram (%)");
		break;
		case G_HISTOGRAM_ABS:
		SetFunctionName((CString)"Histogram (absolute)");
		break;
		case G_TSM_SENSOR:
		SetFunctionName((CString)L"");
		break;

		default:
		SetFunctionName((CString)"");
		break;
	}

	////////////////////AutoScaling X axis
	/*
	Precautions:
	For the plotdeviationpercent and plotdeviationabsolute
	the axis min should be 0 (having any other values makes no sense)
	*/
	double xminimum;
	if( (pFunctionParams->FuncType==G_PLOTDEV_PERCENT)
		|| (pFunctionParams->FuncType==G_PLOTDEV_ABS))
	{
		xminimum=0;
	}
	else
	{
		xminimum=pFunctionParams->xMin;
	}

	SetXAxisScale(xminimum,pFunctionParams->xMax);
	//set the Y axis scale
	SetYAxisScale(pFunctionParams->yMin,pFunctionParams->yMax);

	PaintGraph(); 

	return TRUE;
}


///////////////////////// ClearFunction ////////////////////////////////
/*																	 ///
	This resets the G_FUNCTIONSTRUCT member pointer					 ///
	The graph is repainteed (cleared)								 ///
*/																	 ///
////////////////////////////////////////////////////////////////////////
void CGraph::ClearFunction()
{
	m_pFunctionParams=NULL; //reset the pointer
	//Clear the graph
	PaintGraph();
}


/////////////////////////////// DrawFunction  /////////////////////////////////
/*																			///
//	This is called from within the PaintGraph routine to draw				///
//	the actual function onto the graph.										///
//	This functions just switches the FunctionType member specified in the	///
//	G_FUNCTIONSTRUCT and calls the appropriate routine.						///
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CGraph::DrawFunction()
{
	if(!m_pWnd)
	{
		return;
	}
	
	if(!m_pFunctionParams)
	{
		return;
	}
	
	switch (m_pFunctionParams->FuncType)
	{
		case G_SINX:
			DoSineX();
			break;
		case G_COSX:
			DoCosX();
			break;
		case G_XSQUARED:
			DoXSquared();
			break;
		case G_XCUBED:
			DoXCubed();
			break;
		case G_MXPLUSC:
			DoMXPlusC();
			break;
		case G_LOGEX:
			DoLogeX();
			break;
		case G_PLOTXY:
			DoPlotXY();
			break;
		case G_PLOTDEV_PERCENT:
		case G_PLOTDEV_ABS:
			DoDeviationPercentAbs();
			break;
		case G_MULTIPLOTXY:
			DoMultiPlotXY();
			break;
		case G_HISTOGRAM_PERCENT:
		case G_HISTOGRAM_ABS:
			DoHistogram();
		
		case G_TSM_SENSOR:
			DoCustomFun();
		break;

		default:
		break;
	}
}

////////////////////// DoSineX ////////////////////////////////////////////////
/*																			///
	This draws the function ASinX											///
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CGraph::DoSineX()
{
	//we loop round from the given xstart to xend
	//plotting y=sinx
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//the left hand side of the graph plot area on screen

	//how many steps (pixels) in the x-direction
	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;

	UINT step;
	if(m_pFunctionParams->ChartType==G_BARCHART)
	{
		step=G_BARCHARTWIDTH;
	}
	else
	{
		step=1;
	}
	
	for (UINT count =0; count < xspan; count+=step)
	{
		//do the function
		double y=m_pFunctionParams->Const_1*sin(m_dXAxisMin+count*xperpixel);
		
		UINT pixelx=xstart+count;

		y=ConstrainY(y);
		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
		
		
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}

}

//////////////////// DoCosX ////////////////////////////////////////////////////
/*																			////
	Draws the function CosX													////
*/																			////
////////////////////////////////////////////////////////////////////////////////
void CGraph::DoCosX()
{
	//we loop round from the given xstart to xend
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//the left hand side of the graph plot area on screen

	//how many steps (pixels) in the x-direction
	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;

	UINT step;
	if(m_pFunctionParams->ChartType==G_BARCHART)
	{
		step=G_BARCHARTWIDTH;
	}
	else
	{
		step=1;
	}
	
	for (UINT count =0; count < xspan; count+=step)
	{
		//do the function
		double y=m_pFunctionParams->Const_1*cos(m_dXAxisMin+count*xperpixel);
		
		UINT pixelx=xstart+count;

		y=ConstrainY(y);
		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
				
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;
	}
}

/////////////////// DoXSquared  ///////////////////////////////////////////////////
/*																				///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
void CGraph::DoXSquared()
{
	//we loop round from the given xstart to xend
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//the left hand side of the graph plot area on screen

	//how many steps (pixels) in the x-direction
	UINT xspan=(m_dXAxisMax -m_dXAxisMin)*m_dPixelsPerX;

	UINT step;
	if(m_pFunctionParams->ChartType==G_BARCHART)
	{
		step=G_BARCHARTWIDTH;
	}
	else
	{
		step=1;
	}
	
	for (UINT count =0; count < xspan; count+=step)
	{
		//do the function
		double y=pow( (m_dXAxisMin+count*xperpixel),2);
		
		UINT pixelx=xstart+count;
		y=ConstrainY(y);//keep y within the plottable y scale

		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}
}

/////////////////// DoXCubed   /////////////////////////////////////////////////
/*																			////
*/																			////
////////////////////////////////////////////////////////////////////////////////
void CGraph::DoXCubed()
{
	//we loop round from the given xstart to xend
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	//how many steps (pixels) in the x-direction
	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;

	UINT step;
	if(m_pFunctionParams->ChartType==G_BARCHART)
	{
		step=G_BARCHARTWIDTH;
	}
	else
	{
		step=1;
	}
	
	for (UINT count =0; count < xspan; count+=step)
	{
		//do the function
		double y=pow( (m_dXAxisMin+count*xperpixel),3.00);
		
		UINT pixelx=xstart+count;

		y=ConstrainY(y);	
		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}
}

//////////////////// DoMXPlusC ////////////////////////////////////////////////
/*																			///	
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CGraph::DoMXPlusC()
{
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	//how many steps are they (pixels) in the x-direction
	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;

	UINT step;
	if(m_pFunctionParams->ChartType==G_BARCHART)
	{
		step=G_BARCHARTWIDTH;
	}
	else
	{
		step=1;
	}
	
	for (UINT count =0; count < xspan; count+=step)
	{
		//do the function
		double y=m_pFunctionParams->Const_1*( m_dXAxisMin+(count*xperpixel))+m_pFunctionParams->Const_2;
		
		UINT pixelx=xstart+count;

		y=ConstrainY(y);	
		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}
}


////////////////////////// DoLogeX  ///////////////////////////////////////////////
/*																				///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
void CGraph::DoLogeX()
{
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	//how many steps are they (pixels) in the x-direction
	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;

	UINT step;
	if(m_pFunctionParams->ChartType==G_BARCHART)
	{
		step=G_BARCHARTWIDTH;
	}
	else
	{
		step=1;
	}
	
	for (UINT count =0; count < xspan; count+=step)
	{
		//do the function
		double y=log( m_dXAxisMin+(count*xperpixel));
		
		UINT pixelx=xstart+count;

		y=ConstrainY(y);	
		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}
}


////////////////////////  DoPlotXY ////////////////////////////////////////////////
/*																				///	
	This has many similarities with the other functions - however				///
	there is no need to calculate Y  in the sane way as in the other function	///
	as it is given.																///
	a particular x point on the graph is also given - we place the				/// 
	given y value at the given x point											///
	Other:																		///
	a. We will not bother plotting  if x < the graph scale XMin or > XMax		///
	b. Only dot, bar and line will be acceptable for the chart type. Any other	///
	will default to bar. However be aware that line is only suitable if the		///
	x-values are in sequence.													///
	c. The usual y constraints apply											///
*/																				///																			////
///////////////////////////////////////////////////////////////////////////////////
void CGraph::DoPlotXY()
{
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
	
	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems*2; count+=2)
	{
		
		double x=m_pFunctionParams->pPlotXYItems[count];
		double y=m_pFunctionParams->pPlotXYItems[count+1];
		
		//if x is off scale - don't bother
			if ( (x < m_dXAxisMin) || (x > m_dXAxisMax))
			{
				continue; //NEXT !!!!!!
			}
		
		UINT pixelx= xstart+(x-m_dXAxisMin)/xperpixel;

		y=ConstrainY(y);	
		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}
}


/////////////////////// DoDeviationPercentAbs /////////////////////////////////////
/*																				///
	There are two types of deviation graphs, namely percent and absolute		///
	but hey both end up here to be sorted out									///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
 void CGraph::DoDeviationPercentAbs()
 {
 	/* 
 	No X valuess are supplied. The x-axis represents
 	samples really
 	a. We will not bother plotting  if x < the graph scale XMin or > XMax
 	c. The usual y constraints apply
 	*/
 	UINT prevx=0;
 	UINT prevy=0;
 	BOOL firstpoint=TRUE;
 	
 	LONG result;
 	UINT xstart;
 	UINT ystart;
 	double yperpixel=1/m_dPixelsPerY;
 
 	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
 	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
 	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
 
 	/*
 	We need to know when we will run off the right hand side
 	*/
 	CRect dataarea=CalcDataArea();
 
 	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems*2; count+=2)
 	{
 		
 		double yval;//will hold percent or absolute devaition
 		double yset=m_pFunctionParams->pPlotXYItems[count];
 		double yactual=m_pFunctionParams->pPlotXYItems[count+1];
 		if( m_pFunctionParams->FuncType==G_PLOTDEV_PERCENT)
 		{
 			yval=(yactual-yset)/yset*100; //percentage of setpoint
 		}
 		else
 		{
 			yval=(yactual-yset); //difference
 		}
 			
 		
 		//if we reach the right hand side then we can stop
 		
 		UINT pixelx= xstart+( (count/2)*m_dPixelsPerX);
 		if (pixelx > dataarea.right )
 		{
 			return; //no need to go any further
 		}
 
 
 		yval=ConstrainY(yval);	
 		UINT pixely=ystart-(yval-m_dYAxisMin)/yperpixel;
 		
 		if(firstpoint)
 		{
 			prevx=pixelx;
 			prevy=pixely;
 			firstpoint=FALSE;
 		}
 		PlotPoints(pixelx,pixely,prevx,prevy);
 		//current point becomes previous point
 		prevx=pixelx;
 		prevy=pixely;
 
 	}
 }

 
///////////////////// DoMultiPlotXY  //////////////////////////////////////////////
/*	This has many similarities with the PlotXY functions - it plots several		///
	graphs on the																///
	same scale and axis.														///
	It loops (number of graphs) times - drawing each graph in it's				///
	entirety each time.															///
	Other:																		///
	a. We will not bother plotting  if x < the graph scale XMin or > XMax		///
	b. Dot, bar, and line will be acceptable for the chart type.				///
	c. The usual y constraints apply											///
																				///
	The number of graphs required are givenn in the								///
	Const_1 fiend of the G_FUNCTIONSTRUCT										///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
void CGraph::DoMultiPlotXY()
{
	UINT prevx=0;
	UINT prevy=0;
	
	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	//how many graphs?
	UINT num_graphs=m_pFunctionParams->Const_1;
	//how may plot items per graph
	UINT num_plotitems=m_pFunctionParams->num_PlotXYItems;
	//save the default pen color
	COLORREF savedpencolor=m_crGraphPenColor;
	UINT PenCount=0;
	
	for( long countgraphs=0;countgraphs < 
					num_graphs*num_plotitems*2;countgraphs+=(long)num_plotitems*2)
	{
	
		//set the pencolor for the current graph
		m_crGraphPenColor=GetMultiPlotPenColor(PenCount);
		
		BOOL firstpoint=TRUE;//first time round each graph loop

		for (UINT count =0; count < num_plotitems*2; count+=2)
		{
		
		double x=(m_pFunctionParams->pPlotXYItems+countgraphs)[count];
		double y=(m_pFunctionParams->pPlotXYItems+countgraphs)[count+1];
		
		//if x is off scale - don't bother
			if ( (x < m_dXAxisMin) || (x > m_dXAxisMax))
			{
				continue; //NEXT !!!!!!
			}
		
		UINT pixelx= xstart+(x-m_dXAxisMin)/xperpixel;

		y=ConstrainY(y);	
		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}//end this graph

	PenCount++; //next pen
	}//all graphs

	//restore the graph pencolor
	m_crGraphPenColor=savedpencolor;
}

///////////////    DoHistogram ////////////////////////////////////////////////////
/*																				///
	The user can request two types of histogram, namely							///
	one where the frequency of occurrence is displayed as anumber value,		///
	the other where the frequency of occurrence is displayed as a percentage	///
	of the of the quantity of dataitems											///
	Either way they both end up here to be plotted.								///
	The usersupplies the usual data pairs.										///
	However, the first values of the pair(usually the x value)					///
	is not used by this function. The user can use this as a sequence			///
	number if they wish.														///
	We use the send value of each pair -usually the y value.					///
	//For the histogram - we need to calulate									///
	//a. The mean value															///
	//b. The Root of the Mean of the Squares;									///
																				///
	To disply the data we, we divide the graph(dataarea) by the width			///
	of a bar to see how many we can fit in. We then divide the min-max			///
	span of the data into the same amount of slices.							///
	We then check each data item to see which slice it falls into.				///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
void CGraph::DoHistogram()
{
	
	UINT prevx=0;
	UINT prevy=0;
	BOOL firstpoint=TRUE;
	
	LONG result;
	UINT xstart;
	UINT ystart;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	//we will autoscale the x-axis to match the min and max value in the data;
	UINT count;
	double minval,maxval;
	minval=maxval=m_pFunctionParams->pPlotXYItems[1];
	for (count =0; count < m_pFunctionParams->num_PlotXYItems*2; count+=2)
	{
		if(m_pFunctionParams->pPlotXYItems[count+1] < minval)
		{
			minval=m_pFunctionParams->pPlotXYItems[count+1];
		}
		else if (m_pFunctionParams->pPlotXYItems[count+1] > maxval)
		{
			maxval=m_pFunctionParams->pPlotXYItems[count+1];
		}
	}
	//we should have just found the min and max values of the data
	//scale graph accordingly
	SetXAxisScale(minval,maxval);
	DrawXAxisNumbers();

	//find how many bars can fit into the praph plot width
	CRect dataarearect=CalcDataArea();
	UINT num_strips= (dataarearect.right-dataarearect.left)/G_BARCHARTWIDTH;
	
	//find out how much a strip is 'worth'
	double strip_val= (maxval-minval)/num_strips;
	//count the frequency of the data
	double *pCounts=new double[num_strips];
	memset(pCounts,0,num_strips*sizeof(double));
	for(count=0;count < m_pFunctionParams->num_PlotXYItems*2;count +=2)
	{
		double value=m_pFunctionParams->pPlotXYItems[count+1];
		//see which strip the current value falls into
		for(UINT stripcount=0; stripcount<num_strips;stripcount++)
		{
			
			if( (value >=minval+(stripcount*strip_val)) && 
				(value <= minval+(stripcount*strip_val)+strip_val))	
			{
				pCounts[stripcount]++;
				break;
			}
		}

	}
	/*
		Chack if we are showing the histogram frequencies as an absolute
		occurrence figure or as a percentage occurrence
	*/
	if(m_pFunctionParams->FuncType==G_HISTOGRAM_ABS)
	{

		//find the maximum frequency of occurence and scale the 
		//y axis
		int max_freqcurrent=pCounts[0]; //preset a start point
		for(count=1;count < num_strips; count++)
		{
			if (pCounts[count] > max_freqcurrent)
			{
				max_freqcurrent=pCounts[count];
			}
		}
	
		SetYAxisScale(0,max_freqcurrent+1);
		
	}
	else
	{
		//we are showing as a percentage
		//change the ocurrences from an absolute occurence figure to % occurrence
		
		for(count=0;count < num_strips; count++)
		{
			pCounts[count] =( (pCounts[count]/ ((double)m_pFunctionParams->num_PlotXYItems))*100);
		}
	
		//find the maximum frequency of occurence and scale the 
		//y axis
		int max_freqcurrent=pCounts[0]; //preset a start point
		for(count=1;count < num_strips; count++)
		{
			if (pCounts[count] > max_freqcurrent)
			{
				max_freqcurrent=pCounts[count];
			}
		}
		
		
		SetYAxisScale(0,max_freqcurrent+1);
	}

	//in both cases
	DrawYAxisNumbers();

	//we have changed the x & y axis scaling so recalculate
	//the yperpixel and/or xperpixel as required
	double yperpixel=1/m_dPixelsPerY;	
	
	for (count =0; count < num_strips; count++)
	{		
		UINT pixelx= xstart+(count*G_BARCHARTWIDTH);;

		pCounts[count]=ConstrainY(pCounts[count]);	
		UINT pixely=ystart-(pCounts[count]/yperpixel);
		
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;

	}
	//cleanup
	delete []pCounts;
}


///////////////// PlotPoints  /////////////////////////////////////////////
/*																		///
	Each Function e.g. DoSineX,DoPlotXY, etc, calls this function as	///
	they calculate each point so that each point can be drawn on the	///
	Takes:																///
	UINT x, UNIT y - the graph co-ord of the point just calculeted		///
	(current point).													///
	UINT prevx, UINT prevy - the co-ords of the previous point			///
	This routine checks what type of plot (line, dot, or bar) is		///
	required and calls the appropriate routine							///
*/																		///
///////////////////////////////////////////////////////////////////////////
void CGraph::PlotPoints(UINT x, UINT y, UINT prevx, UINT prevy)
{
	//here we check the chart type and plot the points accordingly
	//we need to constarin the Y values to keep them within the
	//plot area;
	
	switch(m_pFunctionParams->ChartType)
	{
	case G_BARCHART:
		{
			DrawBar(x,y);
			break;
		}
	case G_LINECHART:
		{
			DrawConnectLine(prevx,prevy,x,y);
			break;
		}
	case G_DOTCHART:
	default:
		{
			DrawDot(x,y);
			break;
		}
	}//SWITCH

		return;
}


///////////////////// DrawDot /////////////////////////////////////////////
/*																		///
	For the Dot chart type - this routine plots the pixel at the given	///
	point																///
*/																		///
///////////////////////////////////////////////////////////////////////////
void CGraph::DrawDot(UINT X, UINT Y)
{
	
	if (!m_pWnd)
	{
		return;
	}
	CDC *pdc =m_pWnd->GetDC();
	pdc->SetPixel(X,Y,m_crGraphPenColor);
	m_pWnd->ReleaseDC(pdc);

}


//////////////////////////// DrawConnectLine //////////////////////////////////
/*																			///
	For the line chat type, this routine draws a line between previous		///
	point (FROM) and current point (TO)										///
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CGraph::DrawConnectLine(UINT FromX, UINT FromY, UINT ToX, UINT ToY)
{
	//draws a connecting line between to pixels
	//using the graphpen color

	if(!m_pWnd)
	{
		return;
	}
	//for the PLOTXY graphtype the user is not 

	CPen pen, *poldpen;
	pen.CreatePen(PS_SOLID,1,m_crGraphPenColor);
	CDC *pdc=m_pWnd->GetDC();
	poldpen=pdc->SelectObject(&pen);

	pdc->MoveTo(FromX,FromY);
	pdc->LineTo(ToX,ToY);

	pdc->SelectObject(poldpen);
    pen.DeleteObject();// updated 12/9/2009 - fixed memory leak 

	m_pWnd->ReleaseDC(pdc);

}

////////////////// DrawBar  ///////////////////////////////////////////////////
/*																			///
	This draws the bar of a bar chart										///
	The lefthand side of the bar is at point xy.							///
	It also calcultes whether it is a 'positive' bar or a 'negative 'bar'	///
	Some manipulation of the barwidths maybe required depending on what		///
	function is being drawn													///
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CGraph::DrawBar(UINT x, UINT y)
{
	//here we create a vertical rect
	//and fill it with the current pen color
	 //we only use the current point - the width
	//of the barchart is added to make rect

	if(!m_pWnd)
	{
		return;
	}
	
	//for plotxy, dev% an dev (abs) barcharts, we have to adjust the width
	//because for these funtions the user provides a set of xpoint for the
	//plot. These points may be unevenly spread and may cause overlapping
	//bars.
	//we do however know howmuch one unit of x is worth in pixels so we
	//use that as our barwidth

	UINT barwidth;
	if	(		(m_pFunctionParams->FuncType==G_PLOTXY)
			|| (m_pFunctionParams->FuncType==G_PLOTDEV_PERCENT)
			|| (m_pFunctionParams->FuncType==G_PLOTDEV_ABS)
		)
	{
		if( m_dPixelsPerX >1)
		{
			barwidth=m_dPixelsPerX;
		}
		else
		{
			barwidth=1;
		}//
	}
	else //Standard bars for everybody else
	{
		barwidth=G_BARCHARTWIDTH;
	}
	
	CRect rect,dataarea;
	dataarea=CalcDataArea();
	//remember when we create the bar we must not step over the graph RHS
	rect.left=x;
	rect.right= ((x+barwidth)<dataarea.right)? x+barwidth :dataarea.right;
	//the tricky bit
	//the top/bottom of the bar depends on whether the bar
	//is a negative value or positive value
	
	//note we have need of the Y origin here
	//the Yorigin is an off-set from the bottom of the entire graph bottom
	//and therefore includes the bottom margin -
	// - not from the bottom of the plot area as is our  pixel co-ords
	UINT bmargin =CalcBottomMargin();
	UINT Yorg=m_iOriginY-bmargin;
	
	
	if(y < dataarea.bottom-Yorg)
	{
		//it's a 'positive' value
		rect.top=y;
		rect.bottom=dataarea.bottom-Yorg;
	
	}
	else
	{
		rect.top=dataarea.bottom-Yorg;
		rect.bottom=y;
		// it's a negative value
	}

	CPen pen, *poldpen;
	CBrush brush;	
	
	CDC *pdc=m_pWnd->GetDC();
	
	brush.CreateSolidBrush(m_crGraphPenColor);
	pen.CreatePen(PS_SOLID,1,m_crGraphPenColor);
	poldpen=pdc->SelectObject(&pen);
	
	pdc->Rectangle(&rect);

	pdc->SelectObject(poldpen);
    pen.DeleteObject();
    brush.DeleteObject();
	m_pWnd->ReleaseDC(pdc);
}


//////////////// ConstrainY  //////////////////////////////////////////////
/*																		///
	This function makes sure that a value of Y plot valuestays within	///
	stays within the plot area.											///
	Input:																///
	double Y value														///
	Returns:															///
	double y constrained to the plotting area							///
*/																		///
///////////////////////////////////////////////////////////////////////////
double CGraph::ConstrainY(double y)
{

	if ( (y < m_dYAxisMax) && (y > m_dYAxisMin))
	{
		return y;
	}

	else if (y <= m_dYAxisMin)
	{
		return m_dYAxisMin;
	}
	else if (y >=m_dYAxisMax)
	{
		return m_dYAxisMax;
	}
	return y; //should never get here???
}

//////////////  GetMultiPlotPenColor //////////////////////////////////////////
/*																			///
	For multiply xy graphs we cycle through a series of pens for each graph	///
	Input:																	///
	UINT PenNumber (synonymous with GraphNumber)							///
	Returns:																///
	COLORREF of next pen in the cycle										///
*/																			///
///////////////////////////////////////////////////////////////////////////////
COLORREF CGraph::GetMultiPlotPenColor(UINT PenNumber)
{
	//keep the pen within range
	PenNumber=PenNumber%G_NUM_MULTIPLOT_COLORS;
	switch(PenNumber)
	{
	case 0:
		return G_MULTIPLOT_COLOR_ONE;
		break;
	case 1:
		return G_MULTIPLOT_COLOR_TWO;
		break;
	case 2:
		return G_MULTIPLOT_COLOR_THREE;
		break;
	case 3:
		return G_MULTIPLOT_COLOR_FOUR;
		break;
	case 4:
		return G_MULTIPLOT_COLOR_FIVE;
		break;
	case 5:
		return G_MULTIPLOT_COLOR_SIX;
		break;
	case 6:
		return G_MULTIPLOT_COLOR_SEVEN;
		break;
	case 7:
		return G_MULTIPLOT_COLOR_EIGHT;
		break;
	default:
		return G_MULTIPLOT_COLOR_ONE;
	}

}


void CGraph::DoCustomFun(void)
{
	//we loop round from the given xstart to xend
	BOOL firstpoint=TRUE;

	UINT prevx=0;
	UINT prevy=0;

	LONG result;
	UINT xstart;
	UINT ystart;
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//the left hand side of the graph plot area on screen

	//how many steps (pixels) in the x-direction
	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
	UINT step;

	UINT pstep=0;
	UINT gstep=1;//(UINT)((float)xspan/m_PullLen)+1;
	double y=0;
	int idx=0;
	UINT pixelx=0;
	UINT pixely=0;

	if(m_pFunctionParams->ChartType==G_BARCHART)
	{
		step=G_BARCHARTWIDTH;
	}
	else
	{
		step=1;
	}


	if (m_SetPoint!=0)
	{

			CRect rect;
			rect.left=m_iGraphX+20;
			rect.top=m_iGraphY+123;
			rect.right=m_iGraphX+100;
			rect.bottom=rect.top+20;

			CDC *pdc=m_pWnd->GetDC();
			CFont *poldfont;
			poldfont=pdc->SelectObject(&m_GraphFont);

			pdc->SetTextColor(RGB(255,0,0));
			pdc->SetBkColor(RGB(0,0,0));

			CString ss("");
			ss.Format(L"%.2f %s",m_SetPoint,m_Units);
			pdc->DrawTextW(ss,&rect,DT_CENTER|DT_END_ELLIPSIS);


		//y=m_SetPoint;

		y=m_HPoint;
		//setpoint drawing
		m_crGraphPenColor=RGB(255,0,0);
		for (UINT count =0; count < xspan; count+=step)
		{
			//do the function

			//make real value to y here////////////////////	
			/*
			pstep++;

				if (pstep>=gstep)
				{
					pstep=0;
					y=m_SetPoint;
				}
			*/

			pixelx=xstart+count;

			y=ConstrainY(y);

			pixely=ystart-((y-m_dYAxisMin)/yperpixel);
				
			if(firstpoint)
			{
				prevx=pixelx;
				prevy=pixely;
				firstpoint=FALSE;
			}

			//!!!PlotPoints(pixelx,pixely,prevx,prevy);

			DrawDot(pixelx,pixely);

			//current point becomes previous point
			prevx=pixelx;
			prevy=pixely;
		}

		y=m_LPoint;
		//setpoint drawing
		m_crGraphPenColor=RGB(255,0,0);
		for (UINT count =0; count < xspan; count+=step)
		{
			//do the function

			//make real value to y here////////////////////	
			/*
			pstep++;

				if (pstep>=gstep)
				{
					pstep=0;
					y=m_SetPoint;
				}
			*/

			pixelx=xstart+count;

			y=ConstrainY(y);

			pixely=ystart-((y-m_dYAxisMin)/yperpixel);
				
			if(firstpoint)
			{
				prevx=pixelx;
				prevy=pixely;
				firstpoint=FALSE;
			}

			//!!!PlotPoints(pixelx,pixely,prevx,prevy);

			DrawDot(pixelx,pixely);

			//current point becomes previous point
			prevx=pixelx;
			prevy=pixely;
		}


	}	
	//main graph drawing

	m_crGraphPenColor=RGB(0,255,0);
	prevx=0;
	prevy=0;
	pstep=0;
	gstep=1;//(UINT)((float)xspan/(float)m_PullLen);
	y=0;
	idx=0;
	firstpoint=1;
  	
	for (UINT count =0; count < xspan; count+=step)
	{
		//do the function

		//make real value to y here////////////////////	

		y=m_PullData[idx];

		pstep++;

			if (pstep>=gstep)
			{
				pstep=0;
				//y=m_PullData[idx++];
				idx++;
			}

		pixelx=xstart+count;

		y=ConstrainY(y);

		pixely=ystart-((y-m_dYAxisMin)/yperpixel);
				
		if(firstpoint)
		{
			prevx=pixelx;
			prevy=pixely;
			firstpoint=FALSE;
		}

		//!!!DrawDot(pixelx,pixely);
		PlotPoints(pixelx,pixely,prevx,prevy);
		//current point becomes previous point
		prevx=pixelx;
		prevy=pixely;
	}

	firstpoint=TRUE;
	prevx=0;
	prevy=0;
}


void CGraph::DrawGap(int x, int y, int n)
{

/*
	if (n==200)
	{
		if ((x!=0)||(y==0)) return;
//Координаты контрольной точки приводим к десятым миллиметра
		x*=10;
		y*=10;
	}


	if (n==100)
	{
		if ((this->mdlX==0)&&(this->mdlY==0)) return;
	}
	else
	{
		//x=mdlX;
		//y=mdlY;
	}

*/

/*
	if (n!=200)
	{
		//y/=m_TARGSCALEFACTOR;
		//x/=m_TARGSCALEFACTOR;
	}
*/

	y*=-1;	


//m_iGraphX верхний левый угол X области отображжения
//m_iGraphY верхний левый угол Y области отображжения 

//CalcTopMargin()			верхнее поле
//CalcBottomMargin()		нижнее поле
//CalcLeftMargin()			левое поле
//CalcRightMargin()			правое поле

//	this->m_iGraphHeight	высота области отображения
//	this->m_iGraphHeight	ширина области отображения

/*

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

*/

//Приводим координаты к метрике мишени

	//700
	//GRAPH_WIDE/2

	
	int M=m_TARGETSCALE*10;
	int mW=GRAPH_WIDE/2;
	int mH=GRAPH_HEIGHT/2;

	x*=mW;
	x/=M;

	y*=mH;
	y/=M;

	int MM=(GRAPH_WIDE/2);

	if (x>MM) x=MM+10;
	if (x<-MM) x=-MM-10;

	if (y>MM) y=MM+10;
	if (y<-MM) y=-MM-10;

	CPen pen, *poldpen;

//создаем действующий пен
//pen.CreatePen(PS_SOLID,1,m_crGridColor);
	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));

//Берем канву родительского окна
	CDC* pdc=m_pWnd->GetDC();

//Запоминаем прежний пен
	poldpen=pdc->SelectObject(&pen);

//Выделение области текста
	CRect rect=CalcDataArea();

//	rect.top+=50;
//	rect.left+=50;

int DY=13;
//int DY=10;

int DX=9;

//if (y>)

	rect.top=GRAPH_CENTER_Y-DY+y;
	rect.left=GRAPH_CENTER_X-DX+x;

	//if (rect.top<GRAPH_ZERO_Y)  rect.top=GRAPH_ZERO_Y;
	//if (rect.top>GRAPH_DOWN_Y)  rect.top=GRAPH_DOWN_Y;

	//if (rect.left<GRAPH_ZERO_X) rect.left=GRAPH_ZERO_X;
	//if (rect.left>GRAPH_RIGHT_X) rect.left=GRAPH_RIGHT_X;

	//rect.top=GRAPH_DOWN_Y-13+y;
	//rect.left=GRAPH_RIGHT_X-9+x;

	rect.right=rect.left+1;
	rect.bottom=rect.top+1;

//Создаем метрику текста
// 	TEXTMETRIC textmetrics;

	

//Создаем фонт
	CFont font;

	//detach old font if any

	//create the new one

	//font.CreatePointFont(8*10,L"Courier",pdc);

	font.Detach();
	font.CreatePointFont(30*10,L"Arial",pdc);
	
// 	pdc->GetTextMetrics(&textmetrics);

	CFont *poldfont;
	//note we must clear of any old stuff crap fom this area
	CBrush brush;

	brush.CreateSolidBrush(m_crGraphBkColor);

	rect.InflateRect(0,0,1,1);//bcause fillrect does not go right to the right
	//or bottom of a given rect
	pdc->FillRect(&rect,&brush);
	rect.DeflateRect(0,0,1,1);

	pdc->SetBkMode(TRANSPARENT);

	//pdc->SetTextColor(m_crXLegendTextColor);

	switch (n)
	{
		case 100:
			pdc->SetTextColor(RGB(255,0,0));
		break;

		case 200:
			pdc->SetTextColor(RGB(255,255,0));
		break;

		default:
			pdc->SetTextColor(RGB(255,255,255));
	}
	//poldfont=pdc->SelectObject(&m_GraphFont);
	poldfont=pdc->SelectObject(&font);



	//pdc->MoveTo(GRAPH_ZERO_X,GRAPH_ZERO_Y);

	//pdc->LineTo(GRAPH_RIGHT_X,GRAPH_ZERO_Y);
	//pdc->LineTo(GRAPH_RIGHT_X,GRAPH_DOWN_Y);
	//pdc->LineTo(GRAPH_ZERO_X,GRAPH_DOWN_Y);
	//pdc->LineTo(GRAPH_ZERO_X,GRAPH_ZERO_Y);

	//pdc->LineTo(GRAPH_CENTER_X,GRAPH_ZERO_Y);
	//pdc->LineTo(GRAPH_CENTER_X,GRAPH_CENTER_Y);

	CString s;
	s.Format(L"%d",n);

//Если показываем среднюю точку
	if (n==100) s.Format(L"СТП");

//Если показываем контрольную точку
	if (n==200) s.Format(L"KT");

	pdc->DrawText(L"*",&rect,DT_NOCLIP|DT_LEFT);


	rect.top-=3;
	rect.left+=10;

	CFont font1;
	font1.Detach();
	//font1.CreatePointFont(7*10,L"Arial Black",pdc);
	font1.CreatePointFont(8*10,L"Arial",pdc);
	pdc->SelectObject(&font1);

	switch (n)
	{
		case 100:
			pdc->SetTextColor(RGB(255,0,0));
		break;

		case 200:
			pdc->SetTextColor(RGB(255,255,0));
		break;

		default:
			pdc->SetTextColor(RGB(255,255,0));
	}

	pdc->DrawText(s,&rect,DT_NOCLIP|DT_LEFT);

	pdc->SelectObject(poldfont);
    brush.DeleteObject();// updated 12/9/2009 - fixed memory leak 

	pdc->SelectObject(poldpen);
    pen.DeleteObject();
	m_pWnd->ReleaseDC(pdc);

}

int CGraph::DrawMdlGap(long x, long y)
{
	
	//this->DrawGap(this->mdlX,this->mdlY,100);

	this->DrawGap(x,y,100);

	return 0;
}

/*

Рисование круга относительно заданного центра с заданным радиусом

*/

int CGraph::DrawTestCircle(long x, long y, long r)
{

	if (r==0) return 0;

//Переводим в десятые миллиметра
	r*=10;

//Преобразуем координаты в относительные на макете мишени

//Приводим к масштабу макета мишени
	//y/=m_TARGSCALEFACTOR;
	//x/=m_TARGSCALEFACTOR;

//!!! Радиус не прводим он всегда в миллиметрах
	//r/=m_TARGSCALEFACTOR;

//Инвертируем координату по Y
	y*=-1;	

//Масштабируем

	int M=m_TARGETSCALE*10;
	int mW=GRAPH_WIDE/2;
	int mH=GRAPH_HEIGHT/2;

//По ширине и высоте масштаб может быть разным
	x*=mW;
	x/=M;

	y*=mH;
	y/=M;

//Радиус по ширрине и высоте
	long rw=r*mW;
	rw/=M;

	long rh=r*mH;
	rh/=M;

//Это если за пределами мишени такого быть не должно
	int MM=(GRAPH_WIDE/2);

	if (x>MM) x=MM+10;
	if (x<-MM) x=-MM-10;

	if (y>MM) y=MM+10;
	if (y<-MM) y=-MM-10;


//Квадрат в который будет далее вписан круг
	CRect rect=CalcDataArea();


//Тут не текст а графика поправки нулевые
int DY=0;
int DX=0;


	rect.top=GRAPH_CENTER_Y-DY+y;
	rect.left=GRAPH_CENTER_X-DX+x;

	rect.right=GRAPH_CENTER_Y-DY+y;
	rect.bottom=GRAPH_CENTER_X-DX+x;

//Берем канву родительского окна
	CDC* pdc=m_pWnd->GetDC();
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	//pen=new CPen(PS_INSIDEFRAME,1,RGB(255,0,0));

//Запоминаем прежний пен
	poldpen=pdc->SelectObject(&pen);

	CBrush brush, *oldbrush;

	//brush.CreateSolidBrush(RGB(255,255,0));	
	//brush.CreateHatchBrush(0,RGB(255,255,0));
	//brush.CreateSysColorBrush(NULL_BRUSH);
//Такое объявление кисти для незакрашенных фигур
	brush.Attach(GetStockObject(NULL_BRUSH) );

	oldbrush=pdc->SelectObject(&brush);


//Правый нижний угол	
	long x1=GRAPH_CENTER_X+x-rw;
	long y1=GRAPH_CENTER_Y+y-rh;

//Левый верхний угол
	long x2=GRAPH_CENTER_X+x+rw;
	long y2=GRAPH_CENTER_Y+y+rh;

//Диагональ прямоугольника
	//pdc->MoveTo(x1,y1);
	//pdc->LineTo(x2,y2);

//Рисуем круг
	pdc->Ellipse(x1,y1,x2,y2);
	
//Прямоугольник в который вписан круг
	//pdc->Rectangle(x1,y1,x2,y2);

	pdc->SelectObject(poldpen);
	pdc->SelectObject(oldbrush);

    pen.DeleteObject();
	brush.DeleteObject();
	m_pWnd->ReleaseDC(pdc);

	return 1;
}


/*
Рисуем прямоугольник
*/

int CGraph::DrawTestRect(long x, long y, long h, long w )
{
//Преобразуем координаты в относительные на макете мишени

	if ((w==0)&&(h==0)) return 0; 

//Приводим к масштабу макета мишени
	w*=10;
	h*=10;

//!!! Высота и ширина не приводится она всегда в миллиметрах
	//w=m_TARGSCALEFACTOR;
	//h=m_TARGSCALEFACTOR;

//Инвертируем координату по Y
	y*=-1;	

//Масштабируем

	int M=m_TARGETSCALE*10;

	int mW=GRAPH_WIDE/2;
	int mH=GRAPH_HEIGHT/2;

//По ширине и высоте масштаб может быть разным
	x*=mW;
	x/=M;

	y*=mH;
	y/=M;

//Радиус по ширрине и высоте
	long rw=(w/2)*mW;
	rw/=M;

	long rh=(h/2)*mH;
	rh/=M;

//Это если за пределами мишени такого быть не должно
	int MM=(GRAPH_WIDE/2);

	if (x>MM) x=MM+10;
	if (x<-MM) x=-MM-10;

	if (y>MM) y=MM+10;
	if (y<-MM) y=-MM-10;


//Квадрат в который будет далее вписан круг
	CRect rect=CalcDataArea();


//Тут не текст а графика поправки нулевые
int DY=0;
int DX=0;


	rect.top=GRAPH_CENTER_Y-DY+y;
	rect.left=GRAPH_CENTER_X-DX+x;

	rect.right=GRAPH_CENTER_Y-DY+y;
	rect.bottom=GRAPH_CENTER_X-DX+x;

//Берем канву родительского окна
	CDC* pdc=m_pWnd->GetDC();
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	//pen=new CPen(PS_INSIDEFRAME,1,RGB(255,0,0));

//Запоминаем прежний пен
	poldpen=pdc->SelectObject(&pen);

	CBrush brush, *oldbrush;

	//brush.CreateSolidBrush(RGB(255,255,0));	
	//brush.CreateHatchBrush(0,RGB(255,255,0));
	//brush.CreateSysColorBrush(NULL_BRUSH);
//Такое объявление кисти для незакрашенных фигур
	brush.Attach(GetStockObject(NULL_BRUSH) );

	oldbrush=pdc->SelectObject(&brush);

//Левый верхний угол
	long x1=GRAPH_CENTER_X+x-rw;
	long y1=GRAPH_CENTER_Y+y-rh;

//Правый верхний угол	

	long x2=GRAPH_CENTER_X+x+rw;
	long y2=GRAPH_CENTER_Y+y-rh;


//Правый нижний угол	
	long x3=GRAPH_CENTER_X+x+rw;
	long y3=GRAPH_CENTER_Y+y+rh;

//Левый нижний угол	

	long x4=GRAPH_CENTER_X+x-rw;
	long y4=GRAPH_CENTER_Y+y+rh;

	pdc->MoveTo(x1,y1);
	pdc->LineTo(x2,y2);
	pdc->LineTo(x3,y3);
	pdc->LineTo(x4,y4);
	pdc->LineTo(x1,y1);


//Диагональ прямоугольника
	//pdc->MoveTo(x1,y1);
	//pdc->LineTo(x2,y2);

//Рисуем круг
	//pdc->Ellipse(x1,y1,x2,y2);
	
//Прямоугольник в который вписан круг
	//!!!pdc->Rectangle(x1,y1,x3,y3);

	//pdc->MoveTo(GRAPH_CENTER_X+10,GRAPH_CENTER_Y+10);
	//pdc->LineTo(GRAPH_CENTER_X+10,GRAPH_CENTER_Y+10);

	pdc->SelectObject(poldpen);
	pdc->SelectObject(oldbrush);

    pen.DeleteObject();
	brush.DeleteObject();
	m_pWnd->ReleaseDC(pdc);


	return 1;
}


int CGraph::DrawCtrlGap(long x, long y)
{
	this->DrawGap(x,y,200);
	return 0;
}


/*
Отрисовка прямоугольника с учетом наклона
Определение вхождения в прямоугольник точек
*/

int CGraph::DrawTestRectEx		 (int rejectidx, //номер оторвавшейся точки если -1 считаем все точки
								  long x[4],	   //координаты 4 точек	
								  long y[4],	   //	
								  long stpx,	   //координаты СТП 	
								  long stpy,	   //	
								  long h,		   //высота прямоугольника	
								  long w		   //ширина прямоугольника	
								  )
{

int res=0;

//Преобразуем координаты в относительные на макете мишени

//Приводим к масштабу макета мишени размеры прямоугольника
	w*=10;
	h*=10;


//Инвертируем координату по Y

	for (int i=0; i<4; i++)
	{
		y[i]*=-1;
	}

	stpy*=-1;

//Масштабируем

	int M=m_TARGETSCALE*10;
	int mW=GRAPH_WIDE/2;
	int mH=GRAPH_HEIGHT/2;

//По ширине и высоте масштаб может быть разным

	for (int i=0; i<4; i++)
	{
		x[i]*=mW;
		x[i]/=M;

		y[i]*=mH;
		y[i]/=M;
	}

	stpx*=mW;
	stpx/=M;

	stpy*=mH;
	stpy/=M;

//Радиус по ширрине и высоте
	long rw=(w/2)*mW;
	rw/=M;

	long rh=(h/2)*mH;
	rh/=M;

//Это если за пределами мишени такого быть не должно
	int MM=(GRAPH_WIDE/2);

	for (int i=0; i<4; i++)
	{
		if (x[i]>MM) x[i]=MM+10;
		if (x[i]<-MM) x[i]=-MM-10;

		if (y[i]>MM) y[i]=MM+10;
		if (y[i]<-MM) y[i]=-MM-10;
	}

		if (stpx>MM) stpx=MM+10;
		if (stpx<-MM) stpx=-MM-10;

		if (stpy>MM) stpy=MM+10;
		if (stpy<-MM) stpy=-MM-10;


//Берем канву родительского окна
	CDC* pdc=m_pWnd->GetDC();
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	//pen=new CPen(PS_INSIDEFRAME,1,RGB(255,0,0));

//Запоминаем прежний пен
	poldpen=pdc->SelectObject(&pen);
	CBrush brush, *oldbrush;

//Такое объявление кисти для незакрашенных фигур
	brush.Attach(GetStockObject(NULL_BRUSH) );
	oldbrush=pdc->SelectObject(&brush);


//Ищем самую левую отчку
	long left1,left2,left3,left4;
	int l1,l2,l3,l4=-1;

	CPoint p1,p2,p3,p4; 

	p1.x=x[0];
	p1.y=y[0];

	p2.x=x[0];
	p2.y=y[0];

	p3.x=x[0];
	p3.y=y[0];

	p4.x=x[0];
	p4.y=y[0];


//Находим крайние точки по оси x

	long xmax=x[0];
	for (int i=0; i<4; i++)
	{		
		if (x[i]>xmax)
		{
			xmax=x[i];			
		}
	}


//1
	left1=xmax;
	for (int i=0; i<4; i++)
	{		
		if (x[i]<left1)
		{
			left1=x[i];
			p1.x=x[i];
			p1.y=y[i];
			l1=i;
		}
	}

//2
	left2=xmax;
	for (int i=0; i<4; i++)
	{		
		if ((x[i]<left2)&&(i!=l1))
		{
			left2=x[i];
			p2.x=x[i];
			p2.y=y[i];
			l2=i;
		}
	}

//3
	left3=xmax;
	for (int i=0; i<4; i++)
	{		
		if ((x[i]<left3)&&(i!=l2)&&(i!=l1))
		{
			left3=x[i];
			p3.x=x[i];
			p3.y=y[i];
			l3=i;
		}
	}

//4
	left4=xmax;
	for (int i=0; i<4; i++)
	{		
		if ((x[i]==xmax))
		{
			left4=x[i];
			p4.x=x[i];
			p4.y=y[i];
			l4=i;
		}
	}


//левая точка
	p1.x+=GRAPH_CENTER_X;
	p1.y+=GRAPH_CENTER_Y;

//верхняя точка	
	p2.x+=GRAPH_CENTER_X;
	p2.y+=GRAPH_CENTER_Y;

//правая точка	
	p3.x+=GRAPH_CENTER_X;
	p3.y+=GRAPH_CENTER_Y;

//нижяя точка
	p4.x+=GRAPH_CENTER_X;
	p4.y+=GRAPH_CENTER_Y;

//Соединяем две крайние точки по Х
	pdc->MoveTo(p1);
	pdc->LineTo(p4);

//Считаем расстояние между двумя крайними по Х точками

	double dx=(double)(p4.x)-(double)(p1.x);
	double dy=(double)(p4.y)-(double)(p1.y);

	dy*=-1;

	double rx=sqrt((dx*dx)+(dy*dy));

//габариты будующего прямоугольника
	double dh=0;
	double dw=0;

	if (rx<=(double)w)
	{
		dh=double(rh);
		dw=double(rw);
	}
	else
	{
		dh=double(rw);
		dw=double(rh);
	}

//Рисуем прямоугольник с выясненными координатами параллельно оси точек 1 - 4

//Считаем угол наклона оси 1-4 к оси Х

#define M_PI           3.14159265358979323846  /* pi */ 

	double alpha=atan((dy/dx))*180/M_PI;

	double ss=sin(alpha);
	double cs=cos(alpha);

	double xr1=dh*cs+stpx+GRAPH_CENTER_X;
	double yr1=dh*ss+stpy+GRAPH_CENTER_Y;
	
	pdc->MoveTo(stpx+GRAPH_CENTER_X,stpy+GRAPH_CENTER_Y);
	pdc->LineTo(xr1,yr1);



//Рисуем полуось

	//pdc->LineTo(p3);
	//pdc->LineTo(p4);
	//pdc->LineTo(p1);
	//pdc->LineTo(p3);

	//pdc->MoveTo(x1,y1);
	//pdc->LineTo(x2,y2);
	//pdc->LineTo(x3,y3);
	//pdc->LineTo(x4,y4);
	//pdc->LineTo(x1,y1);

	pdc->SelectObject(poldpen);
	pdc->SelectObject(oldbrush);

    pen.DeleteObject();
	brush.DeleteObject();
	m_pWnd->ReleaseDC(pdc);

	return res;
}


int CGraph::DrawTestRectCalculated(CPoint p1,CPoint p2,CPoint p3,CPoint p4)
{

//Приводим координаты к масштабу мишени

//Масштабируем

	long M=m_TARGETSCALE*10;
	long mW=GRAPH_WIDE/2;
	long mH=(GRAPH_HEIGHT/2)*(-1);

	p1.x*=mW;
	p1.x/=M;

	p1.y*=mH;
	p1.y/=M;

	p2.x*=mW;
	p2.x/=M;
	p2.y*=mH;
	p2.y/=M;

	p3.x*=mW;
	p3.x/=M;
	p3.y*=mH;
	p3.y/=M;

	p4.x*=mW;
	p4.x/=M;
	p4.y*=mH;
	p4.y/=M;

	long MM=(GRAPH_WIDE/2);

	if (p1.x>MM) p1.x=MM+10;
	if (p1.x<-MM) p1.x=-MM-10;
	if (p1.y>MM) p1.y=MM+10;
	if (p1.y<-MM) p1.y=-MM-10;

	if (p2.x>MM) p2.x=MM+10;
	if (p2.x<-MM) p2.x=-MM-10;
	if (p2.y>MM) p2.y=MM+10;
	if (p2.y<-MM) p2.y=-MM-10;

	if (p3.x>MM) p3.x=MM+10;
	if (p3.x<-MM) p3.x=-MM-10;
	if (p3.y>MM) p3.y=MM+10;
	if (p3.y<-MM) p3.y=-MM-10;

	if (p4.x>MM) p4.x=MM+10;
	if (p4.x<-MM) p4.x=-MM-10;
	if (p4.y>MM) p4.y=MM+10;
	if (p4.y<-MM) p4.y=-MM-10;


	p1.x+=GRAPH_CENTER_X;
	p2.x+=GRAPH_CENTER_X;
	p3.x+=GRAPH_CENTER_X;
	p4.x+=GRAPH_CENTER_X;

	p1.y+=GRAPH_CENTER_Y;
	p2.y+=GRAPH_CENTER_Y;
	p3.y+=GRAPH_CENTER_Y;
	p4.y+=GRAPH_CENTER_Y;



//Берем канву родительского окна
	CDC* pdc=m_pWnd->GetDC();
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	//pen=new CPen(PS_INSIDEFRAME,1,RGB(255,0,0));

//Запоминаем прежний пен
	poldpen=pdc->SelectObject(&pen);
	CBrush brush, *oldbrush;

//Такое объявление кисти для незакрашенных фигур
	brush.Attach(GetStockObject(NULL_BRUSH) );
	oldbrush=pdc->SelectObject(&brush);

	pdc->MoveTo(p1);
	pdc->LineTo(p2);
	pdc->LineTo(p3);
	pdc->LineTo(p4);
	pdc->LineTo(p1);

	pdc->SelectObject(poldpen);
	pdc->SelectObject(oldbrush);

    pen.DeleteObject();
	brush.DeleteObject();
	m_pWnd->ReleaseDC(pdc);


	return 0;
}
