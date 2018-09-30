#pragma once
#include "afx.h"
class CPoint2d :
	public CObject
{
public:
	CPoint2d(double x, double y) :m_scale(1.0) { this->x = x; this->y = y; }
	CPoint2d() :x(0.0), y(0.0), m_scale(1.0) {}
	~CPoint2d();
	double DistTo(CPoint2d);
	void SetScale(double scale) { m_scale = scale; }
	double x;
	double y;
private:
	double m_scale;
};

