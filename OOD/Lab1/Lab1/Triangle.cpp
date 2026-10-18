#include "Triangle.h"
#include "Config.h"
#include <cmath>

Triangle::Triangle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
    : p1(a), p2(b), p3(c)
{
    shape.setPointCount(3);
    shape.setPoint(0, p1);
    shape.setPoint(1, p2);
    shape.setPoint(2, p3);
    shape.setFillColor(Config::TRIANGLE_FILL_COLOR);
    shape.setOutlineThickness(Config::TRIANGLE_OUTLINE_THICKNESS);
    shape.setOutlineColor(Config::TRIANGLE_OUTLINE_COLOR);
}

void Triangle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

double Triangle::area() const {
    return std::abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0);
}

double Triangle::perimeter() const {
    auto dist = [](sf::Vector2f a, sf::Vector2f b) {
        return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
        };
    return dist(p1, p2) + dist(p2, p3) + dist(p3, p1);
}