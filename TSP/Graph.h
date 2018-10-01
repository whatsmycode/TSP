#pragma once
#include "afx.h"
#include "Point2d.h"
#include <vector>
#include <queue>
#include <memory>
#include <map>


class CPoint2d;
class CGraph :
	public CObject
{
public:
	CGraph();
	void AddVertex(CPoint2d);
	void AddVertex(double x, double y);
	void SetVisited(CPoint2d&);
	bool FindCloestUnvisited(CPoint2d&);
private:
	struct Dist {
		size_t target;
		double dist;
		Dist(size_t tar, double d) :target(tar), dist(d) {}
		bool operator<(Dist d) { return this->dist < d.dist; }
	};

	std::map<size_t, std::vector<Dist>>row;
	std::vector<CPoint2d>nodes;
};