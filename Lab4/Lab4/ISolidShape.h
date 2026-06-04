#pragma once

#include "IShape.h"

class ISolidShape : public IShape
{
public:
	static constexpr const char* FILL_COLOR_LABEL = "fillColor=";

	virtual uint32_t GetFillColor() const = 0;
};