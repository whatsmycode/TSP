
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
ON_COMMAND(ID_MIN_COST_TREE, &CTSPDoc::OnMinCostTree)
ON_COMMAND(ID_2OPT, &CTSPDoc::On2opt)
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
		CFile* file = ar.GetFile();//��ȡ��ǰ�ļ�
		char* buffer = new char[file->GetLength()];//������ʱ����
		file->Read(buffer, file->GetLength());
		std::stringstream ss(buffer);
		delete[] buffer;

		std::string line;
		bool validData = false;//�ж������Ƿ���Ч
		while (std::getline(ss, line))//��ȡһ������
		{
			std::stringstream localSS(line);
			std::string first;
			localSS >> first;//�õ���һ���ַ���
			
			if (!isdigit(first[0]))//��һ���ַ�������ĸ��ͷ
			{
				if (first == "NODE_COORD_SECTION")
				{
					validData = true;//������Ч
					m_pNodes->clear();//�����һ���ļ����������
					m_pResult->clear();//���֮ǰ��·�����
				}
				if (first == "EOF")
					break;//�����ļ�β������
				continue;
			}
			else if (validData)//��ʼ��ȡ����
			{
				size_t id = stoi(first);
				double xx, yy;//�õ�x�����y����
				localSS >> xx >> yy;
				m_pNodes->push_back(CPoint2d(id,xx, yy));
			}
			else
			{
				MessageBox(*AfxGetMainWnd(), _T("��֧�ֵ����ݸ�ʽ"), _T("����"), WM_CLOSE);
				ExitProcess(1);//������֧�ֵ��ļ���ʽֱ�ӽ�������
				return;
			}
		}
		if (!m_pNodes->empty())
		{
			graph = CGraph(*m_pNodes);
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


	double totalMinDist = INT_MAX;
	size_t minStartP = 0;


	for (size_t k = 0; k < m_pNodes->size(); ++k)
	{
		size_t i = k;
		size_t j = 0;
		double curTotalDist = 0.0;
		while (graph.IsVisited(i)==false)
		{
			graph.SetVisited(i);
			size_t minV = 0;

			double minDist = INT_MAX;

			for (; j < graph.size(); ++j)
			{
				if (graph.IsVisited(j) == false && graph.GetElement(i,j) < minDist)
				{
					minDist = graph.GetElement(i, j);
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
		graph.Reset();
	}

	size_t i = minStartP;
	size_t j = 0;
	while (graph.IsVisited(i) == false)
	{
		graph.SetVisited(i);
		size_t minV = 0;

		double minDist = INT_MAX;
		m_pResult->push_back(i);
		for (; j < graph.size(); ++j)
		{
			if (graph.IsVisited(j) == false && graph.GetElement(i, j) < minDist)
			{
				minDist = graph.GetElement(i, j);
				minV = j;
			}
		}
		
		i = minV;
		j = 0;
	}
	return totalMinDist;
}


void CTSPDoc::OnMinCostTree()
{
	// TODO: �ڴ���������������
	if (!m_pResult->empty())
		m_pResult->clear();

	std::vector<double>lowcost(m_pNodes->size(), INFINITE);
	std::vector<size_t>adjvex(m_pNodes->size(), 0);
	lowcost[0] = 0;
	m_pResult->push_back(0);
	for (size_t i = 1; i < lowcost.size(); ++i)
	{
		lowcost[i] = graph.GetElement(0, i);
	}

	for (size_t i = 1; i < m_pNodes->size(); ++i)
	{
		double min = INFINITY;
		size_t j = 1; size_t k = 0;
		while (j < m_pNodes->size())
		{
			if (lowcost[j] != 0 && lowcost[j] < min)
			{
				min = lowcost[j];
				k = j;
			}
			j++;
		}
		m_pResult->push_back(k);
		lowcost[k] = 0;

		for (j = 1; j < m_pNodes->size(); ++j)
		{
			if (lowcost[j] != 0 && graph.GetElement(k, j) < lowcost[j])
			{
				lowcost[j] = graph.GetElement(k, j);
				adjvex[j] = k;
			}
		}
	}
}


void CTSPDoc::On2opt()
{
	// TODO: �ڴ���������������
	if (!m_pResult->empty())
		m_pResult->clear();
	
}
