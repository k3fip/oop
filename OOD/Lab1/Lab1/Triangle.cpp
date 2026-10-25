#include "Triangle.h"
#include "Config.h"
#include <cmath>

Triangle::Triangle(Point p1, Point p2, Point p3)
    : m_p1(p1), m_p2(p2), m_p3(p3) { };

Point Triangle::GetP1() const
{
    return m_p1;
}
Point Triangle::GetP2() const
{
    return m_p2;
}
Point Triangle::GetP3() const
{
    return m_p3;
}

double Triangle::Area() const {
    return std::abs((m_p1.x * (m_p2.y - m_p3.y) + m_p2.x * (m_p3.y - m_p1.y) + m_p3.x * (m_p1.y - m_p2.y)) / 2.0);
}

double Triangle::Perimeter() const {
    return m_p1.DistanceTo(m_p2) + m_p2.DistanceTo(m_p3) + m_p3.DistanceTo(m_p1);
}