#pragma once
#include "IShape.h"


class Rectangle : public IShape 
{

private:
    Point m_p1, m_p2;
    float m_width, m_height;

public:
    Rectangle(Point p1, Point p2);

    Point GetP1() const;
    Point GetP2() const;
    float GetWidth() const;
    float GetHeight() const;

    double Area() const override;
    double Perimeter() const override;
};