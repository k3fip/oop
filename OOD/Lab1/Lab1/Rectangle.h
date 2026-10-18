#pragma once
#include "IShape.h"
#include <SFML/Graphics.hpp>

class Rectangle : public IShape {
private:
    sf::RectangleShape shape;

public:
    Rectangle(sf::Vector2f p1, sf::Vector2f p2);

    void draw(sf::RenderWindow& window) const override;
    double area() const override;
    double perimeter() const override;
};