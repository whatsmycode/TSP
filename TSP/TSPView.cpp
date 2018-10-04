
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
#include "MainFrm.h"

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
	ON_WM_MOUSEWHEEL()
//	ON_WM_MBUTTONDOWN()
//	ON_WM_MBUTTONUP()
ON_COMMAND(ID_ALG_Greedy, &CTSPView::OnAlgGreedy)
//ON_WM_MBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CTSPView ����/����

CTSPView::CTSPView()
	: firstRButtonDown(false)
	, firstRButtonPoint(0)
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

void CTSPView::OnDraw(CDC* pDC)
{
	CTSPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	//���Ƶ㼯
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
	//���ӵ㼯
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


BOOL CTSPView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	msg.Format(_T("��ǰ���� %.1lf"), scale);
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
	// TODO: �ڴ���������������
	CTSPDoc* pDoc = GetDocument();
	DWORD st = GetTickCount();
	CString cal("������...");
	TextToStatusBar(cal, 0);
	double dist = pDoc->OnAlgGreedy();
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("̰���㷨  ��ʱ %d ms,��·�� %.1lf"), end - st, dist);
	TextToStatusBar(str,0);
}


void CTSPView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	firstRButtonDown = true;
	firstRButtonPoint = point;
	CView::OnRButtonDown(nFlags, point);
}


void CTSPView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	firstRButtonDown = false;
	CView::OnRButtonUp(nFlags, point);
}


void CTSPView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (firstRButtonDown)
	{
		CPoint2d::SetOffset(point.x - firstRButtonPoint.x, point.y - firstRButtonPoint.y);
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}
