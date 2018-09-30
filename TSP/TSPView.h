
// TSPView.h : CTSPView ��Ľӿ�
//

#pragma once


class CTSPView : public CView
{
protected: // �������л�����
	CTSPView();
	DECLARE_DYNCREATE(CTSPView)

// ����
public:
	CTSPDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CTSPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TSPView.cpp �еĵ��԰汾
inline CTSPDoc* CTSPView::GetDocument() const
   { return reinterpret_cast<CTSPDoc*>(m_pDocument); }
#endif

