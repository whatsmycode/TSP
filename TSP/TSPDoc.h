
// TSPDoc.h : CTSPDoc ��Ľӿ�
//
#include <memory>
#include <vector>
#include "Point2d.h"

#pragma once


class CTSPDoc : public CDocument
{
protected: // �������л�����
	CTSPDoc();
	DECLARE_DYNCREATE(CTSPDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CTSPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// �洢���
	std::shared_ptr<std::vector<CPoint2d>> m_pNodes;
	double OnAlgGreedy();
	std::shared_ptr<std::vector<size_t>> m_pResult;
};
