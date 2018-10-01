#include "stdafx.h"
#include "Graph.h"

void CGraph::AddVertex(CPoint2d pt)
{
	nodes.push_back(pt);
	std::vector<CPoint2d>::iterator beg = nodes.begin();

	std::vector<Dist>& curVector = row[pt.GetID()];
	while (beg != nodes.end() - 1)
	{
		curVector.push_back(Dist((*beg).GetID(), (*beg).DistanceTo(pt)));
	}
}

void CGraph::AddVertex(double x, double y)
{
	CPoint2d pt(x, y);
	AddVertex(pt);
}

void CGraph::SetVisited(CPoint2d& pt)
{
	pt.visited = true;
}

bool CGraph::FindCloestUnvisited(CPoint2d & pt)
{
	std::vector<Dist>& curVector = row[pt.GetID()];
	bool found = false;
	double minDist = INFINITE;
	size_t minID = INFINITE;
	for (size_t i = 0; i < curVector.size(); ++i)
	{
		if (curVector[i].dist < minDist)
		{
			minDist = curVector[i].dist;
			minID = i;
		}
	}

	for (size_t i = pt.GetID() + 1; i < nodes.size(); ++i)
	{
		std::vector<Dist>& curVector = row[i];
		if (curVector[pt.GetID()].dist < minDist)
		{
			minDist = curVector[pt.GetID()].dist;
			minID = i;
		}
	}

	if (minID == INFINITE)
	{
		pt = CPoint2d();
		return false;
	}
	else
	{
		pt = nodes[minID];
		return true;
	}
}

