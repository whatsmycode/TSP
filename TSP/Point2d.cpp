#include "stdafx.h"
#include "Point2d.h"


CPoint CPoint2d::ToCPoint()
{
	return CPoint(static_cast<int>(x*m_scale + offsetX), static_cast<int>(y*m_scale+offsetY));
}

double CPoint2d::m_scale = 0.5;
int CPoint2d::offsetX = 0;
int CPoint2d::offsetY = 0;

double Distance(CPoint2d & p1, CPoint2d & p2)
{
	double xx = (p1.x - p2.x) * (p1.x - p2.x);
	double yy = (p1.y - p2.y)*(p1.y - p2.y);
	return sqrt(xx + yy);
}
