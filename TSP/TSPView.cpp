
// TSPView.cpp : CTSPView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TSP.h"
#endif

#include "TSPDoc.h"
#include "TSPView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSPView

IMPLEMENT_DYNCREATE(CTSPView, CView)

BEGIN_MESSAGE_MAP(CTSPView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()
//	ON_WM_MBUTTONDOWN()
//	ON_WM_MBUTTONUP()
ON_COMMAND(ID_ALG_Greedy, &CTSPView::OnAlgGreedy)
//ON_WM_MBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CTSPView 构造/析构

CTSPView::CTSPView()
	: firstRButtonDown(false)
	, firstRButtonPoint(0)
{
	// TODO: 在此处添加构造代码

}

CTSPView::~CTSPView()
{
}

BOOL CTSPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTSPView 绘制

void CTSPView::OnDraw(CDC* pDC)
{
	CTSPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//绘制点集
	if (pDoc->m_pNodes->size())
	{
		std::vector<CPoint2d>& curVec = *(pDoc->m_pNodes);
		CBrush brush(RGB(255, 0, 0));
		pDC->SelectObject(brush);
		
		int radius = 5;
		CPoint pt1 = curVec[0].ToCPoint();
		pDC->Ellipse(pt1.x - radius, pt1.y - radius, pt1.x + radius, pt1.y + radius);
		for (size_t i = 1; i < curVec.size(); ++i)
		{
			CPoint pt2 = curVec[i].ToCPoint();
			pDC->Ellipse(pt2.x - radius, pt2.y - radius, pt2.x + radius, pt2.y + radius);
			pt1 = pt2;	
		}
	}
	//连接点集
	if (pDoc->m_pResult->size())
	{
		std::shared_ptr<std::vector<size_t>>pRes = pDoc->m_pResult;
		CPoint first = (*pDoc->m_pNodes)[(*pRes)[0]].ToCPoint();
		CPoint allF = first;
		int i = 1;
		for (; i < pRes->size(); ++i)
		{
			pDC->MoveTo(first);
			CPoint second = (*pDoc->m_pNodes)[(*pRes)[i]].ToCPoint();
			pDC->LineTo(second);
			first = second;
		}
		CPoint last = (*pDoc->m_pNodes)[(*pRes)[i - 1]].ToCPoint();
		pDC->MoveTo(last);
		pDC->LineTo(allF);
		ReleaseDC(pDC);
	}
}


// CTSPView 打印

BOOL CTSPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTSPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTSPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTSPView 诊断

#ifdef _DEBUG
void CTSPView::AssertValid() const
{
	CView::AssertValid();
}

void CTSPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTSPDoc* CTSPView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTSPDoc)));
	return (CTSPDoc*)m_pDocument;
}
#endif //_DEBUG


// CTSPView 消息处理程序


BOOL CTSPView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTSPDoc* pDoc = GetDocument();
	if (pDoc->m_pNodes->empty())
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	double scale = CPoint2d::GetScale();

	if (zDelta > 0)
		scale += 0.1;
	else
		scale -= 0.1;

	if (scale < 0.1)
		scale = 0.11;
	else if (scale > 0.91)
		scale = 0.9;
	else
		CPoint2d::SetScale(scale);
	Invalidate();
	CString msg;
	msg.Format(_T("当前比例 %.1lf"), scale);
	TextToStatusBar(msg,0);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CTSPView::TextToStatusBar(LPCTSTR Str,int index)
{
	CMainFrame* mainFrm = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	mainFrm->m_wndStatusBar.SetPaneText(index, Str);
}


void CTSPView::OnAlgGreedy()
{
	// TODO: 在此添加命令处理程序代码
	CTSPDoc* pDoc = GetDocument();
	DWORD st = GetTickCount();
	CString cal("计算中...");
	TextToStatusBar(cal, 0);
	double dist = pDoc->OnAlgGreedy();
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("贪心算法  耗时 %d ms,总路程 %.1lf"), end - st, dist);
	TextToStatusBar(str,0);
}


void CTSPView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	firstRButtonDown = true;
	firstRButtonPoint = point;
	CView::OnRButtonDown(nFlags, point);
}


void CTSPView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	firstRButtonDown = false;
	CView::OnRButtonUp(nFlags, point);
}


void CTSPView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (firstRButtonDown)
	{
		CPoint2d::SetOffset(point.x - firstRButtonPoint.x, point.y - firstRButtonPoint.y);
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}
