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

	CPoint(const double& dx, const double& dy) : x(dx), y(dy) {}

	double DistanceTo(const CPoint other) const
	{
		double dx = x - other.x;
		double dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	std::string ToString() const
	{
		std::ostringstream oss;
		oss << POINT_OPEN_BRACKET << std::fixed << std::setprecision(POINT_PRECISION) << x << POINT_SEPARATOR << y << POINT_CLOSE_BRACKET;
		return oss.str();
	}

	double x = 0;
	double y = 0;
};

class IShape
{
public:
	static constexpr const char* FIELD_SEPARATOR = ", ";
	static constexpr const char* AREA_LABEL = "area=";
	static constexpr const char* PERIMETER_LABEL = "perimeter=";
	static constexpr const char* OUTLINE_COLOR_LABEL = "outlineColor=";
	static constexpr int DEFAULT_PRECISION = 2;

	virtual ~IShape() = default;
	virtual double GetArea() const = 0;
	virtual double GetPerimeter() const = 0;
	virtual uint32_t GetOutlineColor() const = 0;

	virtual std::string ToString() const = 0;
};

class ISolidShape : public IShape
{
public:
	static constexpr const char* FILL_COLOR_LABEL = "fillColor=";

	virtual uint32_t GetFillColor() const = 0;
};

class CLineSegment : public IShape
{
public:
	static constexpr const char* SHAPE_NAME = "LineSegment: ";
	static constexpr const char* START_POINT_LABEL = "start=";
	static constexpr const char* END_POINT_LABEL = "end=";
	static constexpr const char* LENGTH_LABEL = "length=";

	CLineSegment(const CPoint& startPoint, const CPoint& endPoint, const uint32_t outlineColor)
		: m_startPoint(startPoint), m_endPoint(endPoint)
		, m_outlineColor(outlineColor)
	{
	}

	CLineSegment(const double startX, const double startY, const double endX, const double endY, const uint32_t outlineColor) 
		: CLineSegment({ startX , startY }, { endX , endY }, outlineColor) {}

	double GetArea() const override
	{
		return 0;
	}

	double GetPerimeter() const override
	{
		return m_startPoint.DistanceTo(m_endPoint);
	}

	uint32_t GetOutlineColor() const override { return m_outlineColor; }

	std::string ToString() const override
	{
		std::ostringstream oss;
		oss << SHAPE_NAME
			<< START_POINT_LABEL << m_startPoint.ToString()
			<< FIELD_SEPARATOR << END_POINT_LABEL << m_endPoint.ToString()
			<< FIELD_SEPARATOR << LENGTH_LABEL << std::fixed << std::setprecision(DEFAULT_PRECISION) << GetPerimeter()
			<< FIELD_SEPARATOR << OUTLINE_COLOR_LABEL << m_outlineColor;
		return oss.str();
	}

private:
	CPoint m_startPoint;
	CPoint m_endPoint;
	uint32_t m_outlineColor;
};

class CTriangle : public ISolidShape
{
public:
	static constexpr const char* SHAPE_NAME = "Triangle: ";
	static constexpr const char* VERTICES_LABEL = "vertices={";
	static constexpr const char* VERTICES_CLOSE_BRACKET = "}";
	static constexpr const char* VERTEX_SEPARATOR = ", ";

	CTriangle(const CPoint& vertex1, const CPoint& vertex2, const CPoint& vertex3,
		const uint32_t outlineColor, const uint32_t fillColor)
		: m_vertex1(vertex1), m_vertex2(vertex2), m_vertex3(vertex3)
		, m_outlineColor(outlineColor), m_fillColor(fillColor)
	{
	}

	double GetArea() const override
	{
		double a = GetSideLength(m_vertex1, m_vertex2);
		double b = GetSideLength(m_vertex2, m_vertex3);
		double c = GetSideLength(m_vertex1, m_vertex3);
		double p = (a + b + c) / 2;
		return std::sqrt(p * (p - a) * (p - b) * (p - c));
	}

	double GetPerimeter() const override
	{
		return GetSideLength(m_vertex1, m_vertex2) +
			GetSideLength(m_vertex2, m_vertex3) +
			GetSideLength(m_vertex1, m_vertex3);
	}

	uint32_t GetOutlineColor() const override { return m_outlineColor; }
	uint32_t GetFillColor() const override { return m_fillColor; }

	CPoint GetVertex1() const { return m_vertex1; }
	CPoint GetVertex2() const { return m_vertex2; }
	CPoint GetVertex3() const { return m_vertex3; }

	std::string ToString() const override
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

private:
	double GetSideLength(const CPoint& vertex1, const CPoint& vertex2) const
	{
		return vertex1.DistanceTo(vertex2);
	}

	CPoint m_vertex1;
	CPoint m_vertex2;
	CPoint m_vertex3;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
};

class CRectangle : public ISolidShape
{
public:
	static constexpr const char* SHAPE_NAME = "Rectangle: ";
	static constexpr const char* LEFT_TOP_LABEL = "leftTop=";
	static constexpr const char* WIDTH_LABEL = "width=";
	static constexpr const char* HEIGHT_LABEL = "height=";

	CRectangle(const double x, const double y, const double width, const double height,
		const uint32_t outlineColor, const uint32_t fillColor)
		: m_topLeftCP(x, y), m_width(width), m_height(height)
		, m_outlineColor(outlineColor), m_fillColor(fillColor)
	{
		if (m_width < 0) m_width = 0;
		if (m_height < 0) m_height = 0;
	}

	double GetWidth() const { return m_width; }
	double GetHeight() const { return m_height; }

	double GetArea() const override {
		return m_width * m_height;
	}

	double GetPerimeter() const override {
		return 2 * (m_width + m_height);
	}

	CPoint GetLeftTop() const { return m_topLeftCP; }
	CPoint GetRightBottom() const {
		return CPoint(m_topLeftCP.x + m_width, m_topLeftCP.y + m_height);
	}

	std::string ToString() const override {
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

	uint32_t GetOutlineColor() const override { return m_outlineColor; }
	uint32_t GetFillColor() const override { return m_fillColor; }

private:
	CPoint m_topLeftCP;
	double m_width;
	double m_height;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;
};

class CCircle : public ISolidShape
{
public:
	static constexpr const char* SHAPE_NAME = "Circle: ";
	static constexpr const char* CENTER_LABEL = "center=";
	static constexpr const char* RADIUS_LABEL = "radius=";

	CCircle(const double& x, const double& y, const double radius,
		const uint32_t outlineColor, const uint32_t fillColor)
		: m_center(x, y), m_radius(radius),
		m_outlineColor(outlineColor), m_fillColor(fillColor)
	{
		if (m_radius < 0) m_radius = 0;
	}

	double GetArea() const override
	{
		return M_PI * m_radius * m_radius;
	}

	double GetPerimeter() const override
	{
		return 2 * M_PI * m_radius;
	}

	std::string ToString() const override
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

	uint32_t GetOutlineColor() const override { return m_outlineColor; }
	uint32_t GetFillColor() const override { return m_fillColor; }

	CPoint GetCenter() const { return m_center; }
	double GetRadius() const { return m_radius; }

private:
	CPoint m_center;
	double m_radius;
	uint32_t m_outlineColor;
	uint32_t m_fillColor;

	static constexpr double M_PI = 3.141592;
};