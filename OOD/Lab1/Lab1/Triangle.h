#pragma once

#include "IShape.h"
#include <SFML/Graphics.hpp>

class Triangle : public IShape {
private:
    sf::ConvexShape shape;
    sf::Vector2f p1, p2, p3;

public:
    Triangle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);

    void draw(sf::RenderWindow& window) const override;
    double area() const override;
    double perimeter() const override;
};