#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class IShape {
public:
    virtual ~IShape() = default;

    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
};