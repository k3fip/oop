#include "CRectangle.h"

CRectangle::CRectangle(const double x, const double y, const double width, const double height,
	const uint32_t outlineColor, const uint32_t fillColor)
	: m_topLeftCP(x, y), m_width(width), m_height(height)
	, m_outlineColor(outlineColor), m_fillColor(fillColor)
{
	if (m_width < 0) m_width = 0;
	if (m_height < 0) m_height = 0;
}

double CRectangle::GetWidth() const
{
	return m_width;
}

double CRectangle::GetHeight() const
{
	return m_height;
}

double CRectangle::GetArea() const
{
	return m_width * m_height;
}

double CRectangle::GetPerimeter() const
{
	return 2 * (m_width + m_height);
}

CPoint CRectangle::GetLeftTop() const
{
	return m_topLeftCP;
}

CPoint CRectangle::GetRightBottom() const
{
	return CPoint(m_topLeftCP.x + m_width, m_topLeftCP.y + m_height);
}

std::string CRectangle::ToString() const
{
	std::ostringstream oss;
	oss << SHAPE_NAME
		<< LEFT_TOP_LABEL << m_topLeftCP.ToString()
		<< FIELD_SEPARATOR << WIDTH_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << m_width
		<< FIELD_SEPARATOR << HEIGHT_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << m_height
		<< FIELD_SEPARATOR << AREA_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetArea()
		<< FIELD_SEPARATOR << PERIMETER_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetPerimeter()
		<< FIELD_SEPARATOR << OUTLINE_COLOR_LABEL << m_outlineColor
		<< FIELD_SEPARATOR << FILL_COLOR_LABEL << m_fillColor;
	return oss.str();
}

uint32_t CRectangle::GetOutlineColor() const
{
	return m_outlineColor;
}

uint32_t CRectangle::GetFillColor() const
{
	return m_fillColor;
}