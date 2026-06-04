#pragma once

#include "IShape.h"
#include "CPoint.h"

class CLineSegment : public IShape
{
public:
	static constexpr const char* SHAPE_NAME = "LineSegment: ";
	static constexpr const char* START_POINT_LABEL = "start=";
	static constexpr const char* END_POINT_LABEL = "end=";
	static constexpr const char* LENGTH_LABEL = "length=";

	CLineSegment(const CPoint& startPoint, const CPoint& endPoint, const uint32_t outlineColor);
	CLineSegment(const double startX, const double startY, const double endX, const double endY, const uint32_t outlineColor);

	double GetArea() const override;
	double GetPerimeter() const override;
	uint32_t GetOutlineColor() const override;
	std::string ToString() const override;

private:
	CPoint m_startPoint;
	CPoint m_endPoint;
	uint32_t m_outlineColor;
};