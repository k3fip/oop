#include "Rectangle.h"
#include "Config.h"
#include <algorithm>
#include <cmath>

Rectangle::Rectangle(sf::Vector2f p1, sf::Vector2f p2) {
    float left = std::min(p1.x, p2.x);
    float top = std::min(p1.y, p2.y);
    float width = std::abs(p2.x - p1.x);
    float height = std::abs(p2.y - p1.y);

    shape.setPosition({ left, top });
    shape.setSize({ width, height });
    shape.setFillColor(Config::RECTANGLE_FILL_COLOR);
    shape.setOutlineThickness(Config::RECTANGLE_OUTLINE_THICKNESS);
    shape.setOutlineColor(Config::RECTANGLE_OUTLINE_COLOR);
}

void Rectangle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

double Rectangle::area() const {
    return shape.getSize().x * shape.getSize().y;
}

double Rectangle::perimeter() const {
    return 2 * (shape.getSize().x + shape.getSize().y);
}