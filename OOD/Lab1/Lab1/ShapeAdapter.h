#pragma once

#include "IShape.h"
#include "IDrawable.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Config.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <type_traits>

template <typename TSFMLShape, typename TModel>
struct IsValidShapePair : std::false_type {};

template <> struct IsValidShapePair<sf::RectangleShape, Rectangle> : std::true_type {};
template <> struct IsValidShapePair<sf::CircleShape, Circle> : std::true_type {};
template <> struct IsValidShapePair<sf::ConvexShape, Triangle> : std::true_type {};

template <typename TSFMLShape, typename TModel>
inline constexpr bool IsValidShapePairV =
IsValidShapePair<TSFMLShape, TModel>::value;



template <typename TSFMLShape, typename TModel>
class ShapeAdapter : public IDrawable 
{
    static_assert(IsValidShapePairV<TSFMLShape, TModel>,
        "ShapeAdapter: not find exact TSFMLShape/TModel. ");

public:
    explicit ShapeAdapter(const TModel& shape);

    void Draw(sf::RenderWindow& window) const override;

private:
    const TModel& m_model;
    TSFMLShape m_sfmlShape;

    static void Configure(sf::RectangleShape& target, const Rectangle& rect);
    static void Configure(sf::CircleShape& target, const Circle& circle);
    static void Configure(sf::ConvexShape& target, const Triangle& tri);
};

template <typename TSFMLShape, typename TModel>
ShapeAdapter<TSFMLShape, TModel>::ShapeAdapter(const TModel& shape)
    : m_model(shape)
{
    Configure(m_sfmlShape, m_model);
}

template <typename TSFMLShape, typename TModel>
void ShapeAdapter<TSFMLShape, TModel>::Draw(sf::RenderWindow& window) const {
    window.draw(m_sfmlShape);
}

template <typename TSFMLShape, typename TModel>
void ShapeAdapter<TSFMLShape, TModel>::Configure(sf::RectangleShape& target,
    const Rectangle& rect) {
    float left = std::min(rect.GetP1().x, rect.GetP2().x);
    float top = std::min(rect.GetP1().y, rect.GetP2().y);

    target.setPosition({ left, top });
    target.setSize({ rect.GetWidth(), rect.GetHeight() });
    target.setFillColor(Config::RECTANGLE_FILL_COLOR);
    target.setOutlineThickness(Config::RECTANGLE_OUTLINE_THICKNESS);
    target.setOutlineColor(Config::RECTANGLE_OUTLINE_COLOR);
}

template <typename TSFMLShape, typename TModel>
void ShapeAdapter<TSFMLShape, TModel>::Configure(sf::CircleShape& target,
    const Circle& circle) {
    float r = circle.GetRadius();
    Point c = circle.GetCenter();

    target.setRadius(r);
    target.setPosition({ c.x - r, c.y - r });
    target.setFillColor(Config::CIRCLE_FILL_COLOR);
    target.setOutlineThickness(Config::CIRCLE_OUTLINE_THICKNESS);
    target.setOutlineColor(Config::CIRCLE_OUTLINE_COLOR);
}

template <typename TSFMLShape, typename TModel>
void ShapeAdapter<TSFMLShape, TModel>::Configure(sf::ConvexShape& target,
    const Triangle& tri) {
    target.setPointCount(3);
    target.setPoint(0, { tri.GetP1().x, tri.GetP1().y });
    target.setPoint(1, { tri.GetP2().x, tri.GetP2().y });
    target.setPoint(2, { tri.GetP3().x, tri.GetP3().y });
    target.setFillColor(Config::TRIANGLE_FILL_COLOR);
    target.setOutlineThickness(Config::TRIANGLE_OUTLINE_THICKNESS);
    target.setOutlineColor(Config::TRIANGLE_OUTLINE_COLOR);
}