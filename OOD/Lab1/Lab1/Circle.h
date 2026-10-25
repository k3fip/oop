#pragma once

#pragma once
#include "IShape.h"


class Circle : public IShape 
{

private:
    float m_radius;
    Point m_center;

public:
    Circle(Point center, float radius);

    float GetRadius() const;
    Point GetCenter() const;

    double Area() const override;
    double Perimeter() const override;
};