#pragma once

#include "IShape.h"
#include <vector>


class Triangle : public IShape 
{

private:
    Point m_p1, m_p2, m_p3;

public:
    Triangle(Point p1, Point p2, Point p3);

    Point GetP1() const;
    Point GetP2() const;
    Point GetP3() const;
    
    double Area() const override;
    double Perimeter() const override;
};