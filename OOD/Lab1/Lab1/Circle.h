#pragma once

#pragma once
#include "IShape.h"
#include <SFML/Graphics.hpp>

class Circle : public IShape {
private:
    sf::CircleShape shape;

public:
    Circle(sf::Vector2f center, float radius);

    void draw(sf::RenderWindow& window) const override;
    double area() const override;
    double perimeter() const override;
};