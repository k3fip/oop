#include "CLineSegment.h"

CLineSegment::CLineSegment(const CPoint& startPoint, const CPoint& endPoint, const uint32_t outlineColor)
	: m_startPoint(startPoint), m_endPoint(endPoint), m_outlineColor(outlineColor)
{
}

CLineSegment::CLineSegment(const double startX, const double startY, const double endX, const double endY, const uint32_t outlineColor)
	: CLineSegment({ startX , startY }, { endX , endY }, outlineColor)
{
}

double CLineSegment::GetArea() const
{
	return 0;
}

double CLineSegment::GetPerimeter() const
{
	return m_startPoint.DistanceTo(m_endPoint);
}

uint32_t CLineSegment::GetOutlineColor() const
{
	return m_outlineColor;
}

std::string CLineSegment::ToString() const
{
	std::ostringstream oss;
	oss << SHAPE_NAME
		<< START_POINT_LABEL << m_startPoint.ToString()
		<< FIELD_SEPARATOR << END_POINT_LABEL << m_endPoint.ToString()
		<< FIELD_SEPARATOR << LENGTH_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetPerimeter()
		<< FIELD_SEPARATOR << OUTLINE_COLOR_LABEL << m_outlineColor;
	return oss.str();
}