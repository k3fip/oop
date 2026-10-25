#include "Point.h"

Point::Point(const float dx, const float dy)
	: x(dx), y(dy) { };

float Point::DistanceTo(const Point other) const
{
	double dx = x - other.x;
	double dy = y - other.y;
	return std::sqrt(dx * dx + dy * dy);
};