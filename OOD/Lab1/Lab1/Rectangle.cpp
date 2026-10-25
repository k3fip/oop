#include "Rectangle.h"
#include "Config.h"
#include <algorithm>
#include <cmath>

Rectangle::Rectangle(Point p1, Point p2)
    :m_p1(p1), m_p2(p2),
     m_width(p1.DistanceTo({ p1.x, p2.y })),
     m_height(p2.DistanceTo({ p2.x, p1.y })) { };

Point Rectangle::GetP1() const
{
    return m_p1;
}
Point Rectangle::GetP2() const
{
    return m_p2;
}
float Rectangle::GetWidth() const
{
    return m_width;
}
float Rectangle::GetHeight() const
{
    return m_height;
}

double Rectangle::Area() const 
{
    return m_width * m_height;
}

double Rectangle::Perimeter() const 
{
    return 2 * (m_width + m_height);
}