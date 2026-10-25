#define _USE_MATH_DEFINES

#include "Circle.h"
#include "Config.h"

Circle::Circle(Point center, float radius)
    : m_center(center), m_radius(radius) { };

float Circle::GetRadius() const
{
    return m_radius;
}

Point Circle::GetCenter() const
{
    return m_center;
}

double Circle::Area() const {
    return M_PI * m_radius * m_radius;
}

double Circle::Perimeter() const {
    return 2 * M_PI * m_radius;
}
