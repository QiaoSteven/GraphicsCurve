
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
END_MESSAGE_MAP()

// CGraphicsCurveView 构造/析构

CGraphicsCurveView::CGraphicsCurveView() noexcept
{
	// TODO: 在此处添加构造代码

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

void CGraphicsCurveView::OnDraw(CDC* /*pDC*/)
{
	CGraphicsCurveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
