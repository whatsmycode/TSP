#pragma once
#include "afx.h"

class CGraph;

class CPoint2d:
	public CObject
{
public:
	friend class CGraph;
	friend double Distance(CPoint2d& p1, CPoint2d& p2);
public:
	CPoint2d(size_t idd,double x, double y) :id(idd) { this->x = x; this->y = y;  }
	CPoint2d() :x(0.0), y(0.0), id(0) {}
	
	static void SetScale(double scale) { m_scale = scale; }
	CPoint2d(const CPoint2d& pt) :x(pt.x), y(pt.y), id(pt.id) {}
	CPoint2d& operator=(const CPoint2d& pt)
	{
		if (this == &pt)
			return *this;
		this->x = pt.x;
		this->y = pt.y;
		this->id = pt.id;
		return *this;
	}
	CPoint ToCPoint();
	double x;
	double y;
	size_t GetID() { return id; }


	bool operator == (CPoint2d& p) { return this->id == p.GetID(); }
	static double GetScale() { return m_scale; }
	static void ResetOffset() { offsetX = 0; offsetY = 0; }
	static void SetOffset(int dx, int dy) { offsetX = dx; offsetY = dy; }
private:
	static double m_scale;
	size_t id;
	static int offsetX;
	static int offsetY;
};
double Distance(CPoint2d& p1, CPoint2d& p2);


