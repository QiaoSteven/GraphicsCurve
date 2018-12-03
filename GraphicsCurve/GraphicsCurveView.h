
// GraphicsCurveView.h: CGraphicsCurveView 类的接口
//

#pragma once
#include "math.h"
#include "stdafx.h"
#include "MapObj.h"

class CGraphicsCurveView : public CView
{
protected: // 仅从序列化创建
	CGraphicsCurveView() noexcept;
	DECLARE_DYNCREATE(CGraphicsCurveView)
/*************************************************************************************/
//属性
public:
	CPoint startPoint;//画一段折线的起始点
	CPoint endPoint;//画一段折线的终止点
	bool boolLButtonDown;//标记鼠标左键是否按下
	int CurveType;//哪种类型画线
	int N;									//次数
	int K;									//阶数
	int nPoints;							//划分点
	CArray<CPoint, CPoint> pointList;       //顶点列表
	CArray<CPoint, CPoint> pointPrintList;	//画点数组
	CObArray objList;//每画一个图形，一个Obj,链成ObjList
//方法
public:
	void HermiteToPoints();					//计算hermite曲线上各点坐标值
	CPoint Hermite(CPoint V[], double u);	//计算hermite曲线在参数为U的坐标值
	void BezierToPoints();
	CPoint Bezier(double u);				//几何作图法
	void BSplineToPoints();
	CPoint BSpline(double knot[], int num, double u);
	double Base(int i, int k, double knot[], int num, double u);//计算B样条基函数
/*************************************************************************************/
// 特性
public:
	CGraphicsCurveDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CGraphicsCurveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void DDALine(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF color);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnBezier();
	afx_msg void OnBspline();
	afx_msg void OnHermite();
};

#ifndef _DEBUG  // GraphicsCurveView.cpp 中的调试版本
inline CGraphicsCurveDoc* CGraphicsCurveView::GetDocument() const
   { return reinterpret_cast<CGraphicsCurveDoc*>(m_pDocument); }
#endif

