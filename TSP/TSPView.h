
// TSPView.h : CTSPView 类的接口
//

#pragma once
#include "atltypes.h"


class CTSPView : public CView
{
protected: // 仅从序列化创建
	CTSPView();
	DECLARE_DYNCREATE(CTSPView)

// 特性
public:
	CTSPDoc* GetDocument() const;

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
	virtual ~CTSPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
//自定义函数
public:
	void TextToStatusBar(LPCTSTR,int);
	afx_msg void OnAlgGreedy();
private:
	bool firstRButtonDown;
	CPoint firstRButtonPoint;
public:
//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMinCostTree();
};

#ifndef _DEBUG  // TSPView.cpp 中的调试版本
inline CTSPDoc* CTSPView::GetDocument() const
   { return reinterpret_cast<CTSPDoc*>(m_pDocument); }
#endif

