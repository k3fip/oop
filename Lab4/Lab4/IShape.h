#pragma once

#include <string>

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