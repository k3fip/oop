#include "CTriangle.h"
#include <cmath>

CTriangle::CTriangle(const CPoint& vertex1, const CPoint& vertex2, const CPoint& vertex3,
	const uint32_t outlineColor, const uint32_t fillColor)
	: m_vertex1(vertex1), m_vertex2(vertex2), m_vertex3(vertex3)
	, m_outlineColor(outlineColor), m_fillColor(fillColor)
{
}

double CTriangle::GetArea() const
{
	double a = GetSideLength(m_vertex1, m_vertex2);
	double b = GetSideLength(m_vertex2, m_vertex3);
	double c = GetSideLength(m_vertex1, m_vertex3);
	double p = (a + b + c) / 2;
	return std::sqrt(p * (p - a) * (p - b) * (p - c));
}

double CTriangle::GetPerimeter() const
{
	return GetSideLength(m_vertex1, m_vertex2) +
		GetSideLength(m_vertex2, m_vertex3) +
		GetSideLength(m_vertex1, m_vertex3);
}

uint32_t CTriangle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CTriangle::GetFillColor() const
{
	return m_fillColor;
}

CPoint CTriangle::GetVertex1() const
{
	return m_vertex1;
}

CPoint CTriangle::GetVertex2() const
{
	return m_vertex2;
}

CPoint CTriangle::GetVertex3() const
{
	return m_vertex3;
}

double CTriangle::GetSideLength(const CPoint& vertex1, const CPoint& vertex2) const
{
	return vertex1.DistanceTo(vertex2);
}

std::string CTriangle::ToString() const
{
	std::ostringstream oss;
	oss << SHAPE_NAME
		<< VERTICES_LABEL << m_vertex1.ToString() << VERTEX_SEPARATOR
		<< m_vertex2.ToString() << VERTEX_SEPARATOR << m_vertex3.ToString() << VERTICES_CLOSE_BRACKET
		<< FIELD_SEPARATOR << AREA_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetArea()
		<< FIELD_SEPARATOR << PERIMETER_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetPerimeter()
		<< FIELD_SEPARATOR << OUTLINE_COLOR_LABEL << m_outlineColor
		<< FIELD_SEPARATOR << FILL_COLOR_LABEL << m_fillColor;
	return oss.str();
}