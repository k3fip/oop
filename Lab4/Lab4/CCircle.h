#pragma once

#include "ISolidShape.h"
#include "CPoint.h"

class CCircle : public ISolidShape
{
public:
	static constexpr const char* SHAPE_NAME = "Circle: ";
	static constexpr const char* CENTER_LABEL = "center=";
	static constexpr const char* RADIUS_LABEL = "radius=";

	CCircle(const double& x, const double& y, const double radius,
		const uint32_t outlineColor, const uint32_t fillColor);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;
	uint32_t GetOutlineColor() const override;
	uint32_t GetFillColor() const override;
	CPoint GetCenter() const;
	double GetRadius() const;

private:
	CPoint m_center;
	double m_radius;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;

	//#define
	static constexpr double M_PI = 3.141592;
};