#pragma once

#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

struct CPoint
{
	static constexpr const char* POINT_SEPARATOR = ", ";
	static constexpr const char* POINT_OPEN_BRACKET = "(";
	static constexpr const char* POINT_CLOSE_BRACKET = ")";
	static constexpr int POINT_PRECISION = 2;

	CPoint(const double& dx, const double& dy);

	double DistanceTo(const CPoint other) const;
	std::string ToString() const;

	double x = 0;
	double y = 0;
};
