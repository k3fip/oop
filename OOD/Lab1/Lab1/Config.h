#pragma once
#include <SFML/Graphics.hpp>

namespace Config {

    constexpr unsigned int WINDOW_WIDTH = 800;
    constexpr unsigned int WINDOW_HEIGHT = 600;
    const std::string WINDOW_TITLE = "Geometry Shapes - Adapter Pattern";
    constexpr unsigned int FRAMERATE_LIMIT = 60;

    const std::string INPUT_FILE = "input.txt";
    const std::string OUTPUT_FILE = "output.txt";

    const std::string KW_TRIANGLE = "TRIANGLE";
    const std::string KW_RECTANGLE = "RECTANGLE";
    const std::string KW_CIRCLE = "CIRCLE";
    const std::string KW_UNKNOWN = "UNKNOWN";

    const sf::Color TRIANGLE_FILL_COLOR = sf::Color::Red;
    const sf::Color TRIANGLE_OUTLINE_COLOR = sf::Color::Red;
    constexpr float TRIANGLE_OUTLINE_THICKNESS = 2.0f;

    const sf::Color RECTANGLE_FILL_COLOR = sf::Color::Green;
    const sf::Color RECTANGLE_OUTLINE_COLOR = sf::Color::Green;
    constexpr float RECTANGLE_OUTLINE_THICKNESS = 2.0f;

    const sf::Color CIRCLE_FILL_COLOR = sf::Color::Blue;
    const sf::Color CIRCLE_OUTLINE_COLOR = sf::Color::Blue;
    constexpr float CIRCLE_OUTLINE_THICKNESS = 2.0f;

    const sf::Color BACKGROUND_COLOR = sf::Color::White;
}