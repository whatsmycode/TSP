
// TSPView.cpp : CTSPView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTSPView ����/����

CTSPView::CTSPView()
{
	// TODO: �ڴ˴���ӹ������

}

CTSPView::~CTSPView()
{
}

BOOL CTSPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTSPView ����

void CTSPView::OnDraw(CDC* /*pDC*/)
{
	CTSPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CTSPView ��ӡ

BOOL CTSPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTSPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTSPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CTSPView ���

#ifdef _DEBUG
void CTSPView::AssertValid() const
{
	CView::AssertValid();
}

void CTSPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTSPDoc* CTSPView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTSPDoc)));
	return (CTSPDoc*)m_pDocument;
}
#endif //_DEBUG


// CTSPView ��Ϣ�������
