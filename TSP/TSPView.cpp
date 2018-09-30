
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
END_MESSAGE_MAP()

// CTSPView 构造/析构

CTSPView::CTSPView()
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

void CTSPView::OnDraw(CDC* /*pDC*/)
{
	CTSPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
