#include "CCircle.h"
#include <cmath>

CCircle::CCircle(const double& x, const double& y, const double radius,
	const uint32_t outlineColor, const uint32_t fillColor)
	: m_center(x, y), m_radius(radius),
	m_outlineColor(outlineColor), m_fillColor(fillColor)
{
	if (m_radius < 0) m_radius = 0;
}

double CCircle::GetArea() const
{
	return M_PI * m_radius * m_radius;
}

double CCircle::GetPerimeter() const
{
	return 2 * M_PI * m_radius;
}

std::string CCircle::ToString() const
{
	std::ostringstream oss;
	oss << SHAPE_NAME
		<< CENTER_LABEL << m_center.ToString()
		<< FIELD_SEPARATOR << RADIUS_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << m_radius
		<< FIELD_SEPARATOR << AREA_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetArea()
		<< FIELD_SEPARATOR << PERIMETER_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetPerimeter()
		<< FIELD_SEPARATOR << OUTLINE_COLOR_LABEL << m_outlineColor
		<< FIELD_SEPARATOR << FILL_COLOR_LABEL << m_fillColor;
	return oss.str();
}

uint32_t CCircle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CCircle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CCircle::GetCenter() const
{
	return m_center;
}

double CCircle::GetRadius() const
{
	return m_radius;
}