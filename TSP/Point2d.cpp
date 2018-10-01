#include "stdafx.h"
#include "Point2d.h"


double CPoint2d::DistanceTo(const CPoint2d& pt)
{
	double xx = (x - pt.x)*(x - pt.x);
	double yy = (y - pt.y)*(y - pt.y);
	return sqrt(xx + yy);
}
CPoint CPoint2d::ToCPoint()
{
	return CPoint(static_cast<int>(x*m_scale), static_cast<int>(y*m_scale));
}
size_t CPoint2d::all_id = 0;
double CPoint2d::m_scale = 1.0;
