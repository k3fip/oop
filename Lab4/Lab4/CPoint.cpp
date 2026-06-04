#include "CPoint.h"

CPoint::CPoint(const double& dx, const double& dy) : x(dx), y(dy) {}

double CPoint::DistanceTo(const CPoint other) const
{
	double dx = x - other.x;
	double dy = y - other.y;
	return std::sqrt(dx * dx + dy * dy);
}

std::string CPoint::ToString() const
{
	std::ostringstream oss;
	oss << POINT_OPEN_BRACKET << std::fixed << std::setprecision(POINT_PRECISION)
		<< x << POINT_SEPARATOR << y << POINT_CLOSE_BRACKET;
	return oss.str();
}