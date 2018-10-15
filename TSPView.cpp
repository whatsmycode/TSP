
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
//ON_WM_RBUTTONDOWN()
//ON_WM_RBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_2OPT, &CTSPView::On2opt)
ON_WM_LBUTTONDOWN()
ON_COMMAND(ID_BUTTON_CLEAR, &CTSPView::OnButtonClear)
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
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
		CPoint pt1 = curVec[0].DisplayPoint;
		pDC->Ellipse(pt1.x - radius, pt1.y - radius, pt1.x + radius, pt1.y + radius);
		for (size_t i = 1; i < curVec.size(); ++i)
		{
			CPoint pt2 = curVec[i].DisplayPoint;
			pDC->Ellipse(pt2.x - radius, pt2.y - radius, pt2.x + radius, pt2.y + radius);
			pt1 = pt2;	
		}
	}
	//���ӵ㼯
	if (pDoc->m_pResult->size())
	{
		std::shared_ptr<std::vector<size_t>>pRes = pDoc->m_pResult;
		CPoint first = (*pDoc->m_pNodes)[(*pRes)[0]].DisplayPoint;
		CPoint allF = first;
		int i = 1;
		for (; i < pRes->size(); ++i)
		{
			pDC->MoveTo(first);
			CPoint second = (*pDoc->m_pNodes)[(*pRes)[i]].DisplayPoint;
			pDC->LineTo(second);
			first = second;
		}
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
	std::shared_ptr<std::vector<CPoint2d>>src = pDoc->m_pNodes;
	if (zDelta > 0)
	{
		for (size_t i = 0; i < src->size(); ++i)
		{
			(*src)[i].Zoom(true,pt);
		}
	}
	else
	{
		for (size_t i = 0; i < src->size(); ++i)
		{
			(*src)[i].Zoom(false,pt);
		}
	}
	Invalidate();

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
	pDoc->OnAlgGreedy();
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("̰���㷨  ��ʱ %d ms,��·�� %.1lf"), end - st, pDoc->GetPathLen(pDoc->m_pResult));
	TextToStatusBar(str,0);
}


void CTSPView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (firstRButtonDown)
	{
		CTSPDoc* pDoc = GetDocument();
		HCURSOR hc = LoadCursor(NULL, IDC_SIZEALL);
		::SetCursor(hc);
		std::shared_ptr<std::vector<CPoint2d>>src = pDoc->m_pNodes;
		for (size_t i = 0; i < src->size(); ++i)
		{
			(*src)[i].Offset(point.x - firstRButtonPoint.x, point.y - firstRButtonPoint.y);
		}
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}


void CTSPView::On2opt()
{
	// TODO: �ڴ���������������
	CTSPDoc* pDoc = GetDocument();
	DWORD st = GetTickCount();
	CString cal("������...");
	TextToStatusBar(cal, 0);
	pDoc->On2opt();
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("2opt ��ʱ %d ms ����Ϊ%.1lf"), end - st,pDoc->GetPathLen(pDoc->m_pResult));
	TextToStatusBar(str, 0);
}


void CTSPView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CTSPDoc* pDoc = GetDocument();
	if (pDoc->FileOpened == false)
	{
		CPoint2d pt(0,point.x, point.y);

		pDoc->m_pNodes->push_back(pt);
		pDoc->graph = CGraph(*(pDoc->m_pNodes));
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}


void CTSPView::OnButtonClear()
{
	// TODO: �ڴ���������������
	CTSPDoc* pDoc = GetDocument();
	pDoc->m_pNodes->clear();
	pDoc->m_pResult->clear();
	Invalidate();
}


void CTSPView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HCURSOR hc = LoadCursor(NULL, IDC_SIZEALL);
	::SetCursor(hc);
	firstRButtonDown = true;
	firstRButtonPoint = point;
	CView::OnMButtonDown(nFlags, point);
}


void CTSPView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	firstRButtonDown = false;
	HCURSOR hc = LoadCursor(NULL, IDC_ARROW);
	::SetCursor(hc);
	CTSPDoc* pDoc = GetDocument();
	std::shared_ptr<std::vector<CPoint2d>>src = pDoc->m_pNodes;

	for (size_t i = 0; i < src->size(); ++i)
	{
		(*src)[i].UpdateLastDisplayPoint();
	}
	CView::OnMButtonUp(nFlags, point);
}
