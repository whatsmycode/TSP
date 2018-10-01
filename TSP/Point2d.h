#pragma once
#include "afx.h"
#include "Graph.h"

class CGraph;
class CPoint2d:
	public CObject
{
public:
	friend class CGraph;
public:
	CPoint2d(double x, double y) :this_id(all_id++) { this->x = x; this->y = y; visited = false; }
	CPoint2d() :x(0.0), y(0.0), this_id(all_id++), visited(false) {}
	double DistanceTo(const CPoint2d&);
	inline void SetScale(double scale) { m_scale = scale; }
	CPoint2d(const CPoint2d& pt) :x(pt.x), y(pt.y), this_id(pt.this_id), visited(pt.visited) {}
	CPoint2d& operator=(CPoint2d pt)
	{
		return CPoint2d(pt);
	}
	CPoint ToCPoint();
	double x;
	double y;
	size_t GetID() { return this_id; }
private:
	static double m_scale;
	static size_t all_id;
	size_t this_id;
	bool visited;
};


