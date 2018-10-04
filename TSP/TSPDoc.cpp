
// TSPDoc.cpp : CTSPDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TSP.h"
#endif

#include "TSPDoc.h"
#include <propkey.h>
#include <sstream>
#include <iostream>
#include <memory>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CTSPDoc

IMPLEMENT_DYNCREATE(CTSPDoc, CDocument)

BEGIN_MESSAGE_MAP(CTSPDoc, CDocument)
//	ON_COMMAND(ID_ALG_Greedy, &CTSPDoc::OnAlgGreedy)
END_MESSAGE_MAP()


// CTSPDoc ����/����

CTSPDoc::CTSPDoc():m_pNodes(new std::vector<CPoint2d>()),m_pResult(new std::vector<size_t>())
{
	// TODO: �ڴ����һ���Թ������
	
}

CTSPDoc::~CTSPDoc()
{
}

BOOL CTSPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CTSPDoc ���л�

void CTSPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		CFile* file = ar.GetFile();
		char* buffer = new char[file->GetLength()];
		file->Read(buffer, file->GetLength());
		std::stringstream ss(buffer);
		delete[] buffer;

		std::string line;
		bool validData = false;
		while (std::getline(ss, line))
		{
			std::stringstream localSS(line);
			std::string first;
			localSS >> first;
			
			if (!isdigit(first[0]))
			{
				if (first == "NODE_COORD_SECTION")
				{
					validData = true;
					m_pNodes->clear();
					m_pResult->clear();
				}
				if (first == "EOF")
					break;
				continue;
			}
			else if (validData)
			{
				size_t id = stoi(first);
				double xx, yy;
				localSS >> xx >> yy;
				m_pNodes->push_back(CPoint2d(id,xx, yy));
			}
			else
			{
				MessageBox(*AfxGetMainWnd(), _T("��֧�ֵ����ݸ�ʽ"), _T("����"), WM_CLOSE);
				ExitProcess(1);
				return;
			}
		}
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CTSPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CTSPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CTSPDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTSPDoc ���

#ifdef _DEBUG
void CTSPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTSPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTSPDoc ����


double CTSPDoc::OnAlgGreedy()
{
	// TODO: �ڴ���������������
	if (m_pNodes->empty())
		return 0.0;
	DWORD startTM = GetTickCount();
	std::vector<std::vector<double>>Graph(m_pNodes->size(), std::vector<double>(m_pNodes->size(),65535.0));
	//��ʼ��ͼ
	const size_t num = m_pNodes->size();
	for (size_t i = 0; i < num; ++i)
	{
		for (size_t j = 0; j < num; ++j)
		{
			if (i != j)
			{
				Graph[i][j] = Distance((*m_pNodes)[i], (*m_pNodes)[j]);
			}
			else
				Graph[i][j] = 0;
		}
	}


	double totalMinDist = INT_MAX;
	size_t minStartP = 0;


	for (size_t k = 0; k < m_pNodes->size(); ++k)
	{
		size_t i = k;
		size_t j = 0;
		double curTotalDist = 0.0;
		while (Graph[i][i] != -1)
		{
			Graph[i][i] = -1;
			size_t minV = 0;

			double minDist = INT_MAX;

			for (; j < Graph[i].size(); ++j)
			{
				if (Graph[j][j] != -1 && Graph[i][j] < minDist)
				{
					minDist = Graph[i][j];
					minV = j;
				}
			}
			if (minDist != INT_MAX)
				curTotalDist += minDist;
			i = minV;
			j = 0;
		}
		if (curTotalDist < totalMinDist)
		{
			totalMinDist = curTotalDist;
			minStartP = i;
		}

		for (size_t i = 0; i < m_pNodes->size(); ++i)
		{
			Graph[i][i] = 0;
		}
	}

	size_t i = minStartP;
	size_t j = 0;
	while (Graph[i][i] != -1)
	{
		Graph[i][i] = -1;
		size_t minV = 0;

		double minDist = INT_MAX;
		m_pResult->push_back(i);
		for (; j < Graph[i].size(); ++j)
		{
			if (Graph[j][j] != -1 && Graph[i][j] < minDist)
			{
				minDist = Graph[i][j];
				minV = j;
			}
		}
		
		i = minV;
		j = 0;
	}
	return totalMinDist;
}
