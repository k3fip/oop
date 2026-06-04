#pragma once

#include "ISolidShape.h"
#include "CPoint.h"

class CRectangle : public ISolidShape
{
public:
	static constexpr const char* SHAPE_NAME = "Rectangle: ";
	static constexpr const char* LEFT_TOP_LABEL = "leftTop=";
	static constexpr const char* WIDTH_LABEL = "width=";
	static constexpr const char* HEIGHT_LABEL = "height=";

	CRectangle(const double x, const double y, const double width, const double height,
		const uint32_t outlineColor, const uint32_t fillColor);

	double GetWidth() const;
	double GetHeight() const;
	double GetArea() const override;
	double GetPerimeter() const override;
	CPoint GetLeftTop() const;
	CPoint GetRightBottom() const;
	std::string ToString() const override;
	uint32_t GetOutlineColor() const override;
	uint32_t GetFillColor() const override;

private:
	CPoint m_topLeftCP;
	double m_width;
	double m_height;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
};