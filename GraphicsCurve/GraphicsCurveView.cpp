
// GraphicsCurveView.cpp: CGraphicsCurveView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GraphicsCurve.h"
#endif

#include "GraphicsCurveDoc.h"
#include "GraphicsCurveView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphicsCurveView

IMPLEMENT_DYNCREATE(CGraphicsCurveView, CView)

BEGIN_MESSAGE_MAP(CGraphicsCurveView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGraphicsCurveView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_Bezier, &CGraphicsCurveView::OnBezier)
	ON_COMMAND(ID_BSpline, &CGraphicsCurveView::OnBspline)
	ON_COMMAND(ID_Hermite, &CGraphicsCurveView::OnHermite)
END_MESSAGE_MAP()

// CGraphicsCurveView 构造/析构

CGraphicsCurveView::CGraphicsCurveView() noexcept
{
	// TODO: 在此处添加构造代码
	CurveType = 0;
	boolLButtonDown = false;
	N = 3;
	K = 4;
	nPoints = 50;

}

CGraphicsCurveView::~CGraphicsCurveView()
{
}

BOOL CGraphicsCurveView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGraphicsCurveView 绘图

void CGraphicsCurveView::OnDraw(CDC* pDC)
{
	CGraphicsCurveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
	boolLButtonDown = false;

	for (int i = 0; i < objList.GetSize(); i++)
	{
		MapObj* obj = (MapObj*)objList.GetAt(i);
		pointPrintList.RemoveAll();
		pointList.RemoveAll();//清空当前顶点列表，从objList中取出一个顶点集，转移到pointList中，准备重画
		int pointsize = obj->points.GetSize();
		for (int j = 0; j < pointsize; j++)
		{
			pointList.Add(obj->points.GetAt(j));
		}

		//画出原有的限制线段
		for (int j = 0; j < pointsize; j++)
		{
			CPoint p1 = pointList.GetAt(j);
			CPoint p2 = pointList.GetAt((j + 1) % pointsize);
			if((j + 1)%pointsize!= 0)
			{
				DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(10, 10, 10));
			}
		}

		/************************************************/
		CPen pen, *oldpen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldpen = pDC->SelectObject(&pen);

		if (obj->type == 1)
		{
			HermiteToPoints();
		}
		else if (obj->type == 2)
		{
			BezierToPoints();
		}
		else if (obj->type == 3)
		{
			BSplineToPoints();
		}

		pDC->MoveTo(pointPrintList.GetAt(0));
		for (int m = 1; m < pointPrintList.GetSize(); m++)
		{
			pDC->LineTo(pointPrintList.GetAt(m));
		}
		pDC->SelectObject(oldpen);
		/************************************************/

		pointPrintList.RemoveAll();
		pointList.RemoveAll();//清空当前顶点列表，从objList中取出一个顶点集，转移到pointList中，准备重画

	}
	

}


// CGraphicsCurveView 打印


void CGraphicsCurveView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGraphicsCurveView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGraphicsCurveView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGraphicsCurveView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CGraphicsCurveView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGraphicsCurveView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGraphicsCurveView 诊断

#ifdef _DEBUG
void CGraphicsCurveView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicsCurveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicsCurveDoc* CGraphicsCurveView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicsCurveDoc)));
	return (CGraphicsCurveDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphicsCurveView 消息处理程序
/*************************************************************************************************************/

// 计算hermite曲线上各点坐标值
void CGraphicsCurveView::HermiteToPoints()
{
	// TODO: 在此处添加实现代码.
	//pointList是组成折线段的点的集合
	//pointPrintList是将要画出的曲线的集合
	double u = 0;
	double delt = 1 / (double)(nPoints);
	CPoint *P = new CPoint[N + 1];
	CPoint *V = new CPoint[N + 1];
	P[0] = pointList.GetAt(0);
	P[1] = pointList.GetAt(2);
	P[2] = pointList.GetAt(1) - pointList.GetAt(0);
	P[3] = pointList.GetAt(3) - pointList.GetAt(2);
	V[0].x = P[0].x * 2 - P[1].x * 2 + P[2].x + P[3].x;
	V[0].y = P[0].y * 2 - P[1].y * 2 + P[2].y + P[3].y;
	V[1].x = P[0].x * (-3) + P[1].x * 3 - P[2].x * 2 - P[3].x;
	V[1].y = P[0].y * (-3) + P[1].y * 3 - P[2].y * 2 - P[3].y;
	V[2] = P[2];
	V[3] = P[0];
	for (int i = 0; i < nPoints + 1; i++)
	{
		pointPrintList.Add(Hermite(V, u));
		u += delt;
	}
	delete P;
	delete V;
}


// 计算hermite曲线在参数为U的坐标值
CPoint CGraphicsCurveView::Hermite(CPoint V[], double u)
{
	// TODO: 在此处添加实现代码.
	CPoint pts;
	double *U = new double[N + 1];
	U[N] = 1;
	pts = V[N];
	for (int i = N - 1; i >= 0; i--)
	{
		U[i] = U[i + 1] * u;
		pts.x += V[i].x * U[i];
		pts.y += V[i].y * U[i];
	}
	delete U;
	return pts;
}


// 计算bezier曲线上各点坐标值
void CGraphicsCurveView::BezierToPoints()
{
	// TODO: 在此处添加实现代码.
	double u = 0;
	double delt = 1 / (double)(nPoints);
	for (int i = 0; i < nPoints + 1; i++)
	{
		pointPrintList.Add(Bezier(u));
		u += delt;
	}
}


// 采用几何作图法，计算bezier曲线在参数为U的坐标值
CPoint CGraphicsCurveView::Bezier(double u)
{
	// TODO: 在此处添加实现代码.
	int m, i;
	CPoint *R = new CPoint[N + 1];
	CPoint *Q = new CPoint[N + 1];
	CPoint P;
	for (int i = 0; i < pointList.GetSize(); i++)
	{
		R[i] = pointList.GetAt(i);
	}
	for (int m = N; m > 0; m--)
	{
		for (int i = 0; i < m; i++)
		{
			Q[i].x = R[i].x + (R[i + 1].x - R[i].x)*u;
			Q[i].y = R[i].y + (R[i + 1].y - R[i].y)*u;
		}
		for (int i = 0; i < m; i++)
		{
			R[i] = Q[i];
		}
	}
	P = R[0];
	delete R;
	delete Q;
	return P;
}


// 计算bspline曲线上各点坐标值
void CGraphicsCurveView::BSplineToPoints()
{
	// TODO: 在此处添加实现代码.
	double *knots = new double[N + K + 1];
	for (int i = 0; i < N + K + 1; i++)
	{
		knots[i] = i;
	}
	double u = knots[K - 1];
	double delt = (knots[N + 1] - knots[K - 1]) / (double)(nPoints);
	for (int i = 0; i < nPoints + 1; i++)
	{
		pointPrintList.Add(BSpline(knots, N + K + 1, u));
		u += delt;
	}
}


// 计算B样条曲线在参数为U的坐标值
CPoint CGraphicsCurveView::BSpline(double knot[], int num, double u)
{
	// TODO: 在此处添加实现代码.
	CPoint ret = CPoint(0, 0);
	double temp;
	for (int i = 0; i < N + 1; i++)
	{
		temp = Base(i, K, knot, num, u);
		ret.x += pointList.GetAt(i).x * temp;
		ret.y += pointList.GetAt(i).y * temp;
	}
	return ret;
}


// 计算B样条基函数
double CGraphicsCurveView::Base(int i, int k, double knot[], int num, double u)
{
	// TODO: 在此处添加实现代码.
	double temp1 = 0;
	double temp2 = 0;
	if (k == 1)
	{
		if (i == (num - 2))
		{
			if ((u >= knot[i]) && (u <= knot[i + 1])) return 1;
			else return 0;
		}
		else
		{
			if ((u >= knot[i]) && (u < knot[i + 1])) return 1;
			else return 0;
		}
	}
	else if (k > 1)
	{
		if (knot[i + k - 1] != knot[i])
		{
			temp1 = Base(i, k - 1, knot, num, u);
			temp1 = (u - knot[i])*temp1 / (knot[i + k - 1] - knot[i]);
		}
		if (knot[i + k] != knot[i + 1])
		{
			temp2 = Base(i + 1, k - 1, knot, num, u);
			temp2 = (knot[i + k] - u) * temp2 / (knot[i + k] - knot[i + 1]);
		}
		return temp1 + temp2;
	}
	return 0;
}



void CGraphicsCurveView::DDALine(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	// TODO: 在此处添加实现代码.
	double dx, dy, e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx /= e;
	dy /= e;
	x = x1;
	y = y1;
	pDC->MoveTo(x1, y1);
	for (int i = 1; i <= e; i++)
	{
		pDC->SetPixel((int)(x + 0.5), (int)(y + 0.5), color);
		x += dx;
		y += dy;
	}
}


void CGraphicsCurveView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//基本操作
	this->SetCapture();	//捕捉鼠标
	boolLButtonDown = true;
	
	startPoint = point;
	endPoint = point;
	pointList.Add(point);

	CView::OnLButtonDown(nFlags, point);
}


void CGraphicsCurveView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pointList.GetSize() == N + 1)
	{
		boolLButtonDown = false;
		ReleaseCapture();	//释放鼠标
		/*************************************************************/
		/*************************************************************/
		//为重画存下目前的顶点
		MapObj* obj = new MapObj();
		obj->type = CurveType;
		for (int i = 0; i < pointList.GetSize(); i++)
		{
			obj->points.Add(pointList.GetAt(i));
		}
		objList.Add(obj);//将此次图形的所有点保存下来
		/*************************************************************/
		/*************************************************************/

		CClientDC pDC(this);
		CPen pen, *oldpen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldpen = pDC.SelectObject(&pen);
		switch (CurveType)
		{
		case 1:
		{
			HermiteToPoints();
		}
		break;
		case 2:
		{
			BezierToPoints();
		}
		break;
		case 3:
		{
			BSplineToPoints();
		}
		break;
		default:
			break;
		}
		pDC.MoveTo(pointPrintList.GetAt(0));
		for (int i = 1; i < pointPrintList.GetSize(); i++)
		{
			pDC.LineTo(pointPrintList.GetAt(i));
		}
		pDC.SelectObject(oldpen);
		pointList.RemoveAll();
		pointPrintList.RemoveAll();
		
	}
	CView::OnLButtonUp(nFlags, point);
}


void CGraphicsCurveView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC* pDC = this->GetDC();	//构造设备环境对象
	pDC->SetROP2(R2_NOT);		//设置绘图模式为R2_NOT

	if (boolLButtonDown)
	{
		pDC->MoveTo(startPoint);
		pDC->LineTo(endPoint);
		pDC->MoveTo(startPoint);
		pDC->LineTo(point);
		endPoint = point;
	}
	CView::OnMouseMove(nFlags, point);
}


void CGraphicsCurveView::OnHermite()
{
	// TODO: 在此添加命令处理程序代码
	CurveType = 1;
}


void CGraphicsCurveView::OnBezier()
{
	// TODO: 在此添加命令处理程序代码
	CurveType = 2;
}


void CGraphicsCurveView::OnBspline()
{
	// TODO: 在此添加命令处理程序代码
	CurveType = 3;
}

