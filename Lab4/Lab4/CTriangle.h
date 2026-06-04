#pragma once

#include "ISolidShape.h"
#include "CPoint.h"

class CTriangle : public ISolidShape
{
public:
	static constexpr const char* SHAPE_NAME = "Triangle: ";
	static constexpr const char* VERTICES_LABEL = "vertices={";
	static constexpr const char* VERTICES_CLOSE_BRACKET = "}";
	static constexpr const char* VERTEX_SEPARATOR = ", ";

	CTriangle(const CPoint& vertex1, const CPoint& vertex2, const CPoint& vertex3,
		const uint32_t outlineColor, const uint32_t fillColor);

	double GetArea() const override;
	double GetPerimeter() const override;
	uint32_t GetOutlineColor() const override;
	uint32_t GetFillColor() const override;
	CPoint GetVertex1() const;
	CPoint GetVertex2() const;
	CPoint GetVertex3() const;
	std::string ToString() const override;

private:
	double GetSideLength(const CPoint& vertex1, const CPoint& vertex2) const;

	CPoint m_vertex1;
	CPoint m_vertex2;
	CPoint m_vertex3;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
};