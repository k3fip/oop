#pragma once

#include "Point.h"
#include <string>

class IShape 
{

public:
    virtual ~IShape() = default;

    virtual double Area() const = 0;
    virtual double Perimeter() const = 0;
};