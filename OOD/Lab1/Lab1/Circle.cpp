#define _USE_MATH_DEFINES

#include "Circle.h"
#include "Config.h"

Circle::Circle(sf::Vector2f center, float radius) {
    shape.setRadius(radius);
    shape.setPosition({ center.x - radius, center.y - radius });
    shape.setFillColor(Config::CIRCLE_FILL_COLOR);
    shape.setOutlineThickness(Config::CIRCLE_OUTLINE_THICKNESS);
    shape.setOutlineColor(Config::CIRCLE_OUTLINE_COLOR);
}

void Circle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

double Circle::area() const {
    return M_PI * shape.getRadius() * shape.getRadius();
}

double Circle::perimeter() const {
    return 2 * M_PI * shape.getRadius();
}