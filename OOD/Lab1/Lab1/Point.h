#pragma once
#include <cmath>

struct Point
{
	Point(const float dx, const float dy);
	float DistanceTo(const Point other) const;

	float x, y;
};