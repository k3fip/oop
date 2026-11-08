#pragma once
#include <SFML/Graphics.hpp>
#include <cstddef>
#include <string>

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

    constexpr char ASSIGN_CHAR = '=';
    constexpr char TERMINATOR = ';';
    constexpr char DECIMAL_COMMA = ',';

    constexpr std::size_t TRIANGLE_COORDS = 6;
    constexpr std::size_t RECTANGLE_COORDS = 4;
    constexpr std::size_t CIRCLE_COORDS = 3;

    constexpr std::size_t TRIANGLE_POINTS = 3;

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

    const std::string MSG_FILE_OPEN_FAILED = "Error: Could not open file ";
    const std::string MSG_FILE_CREATE_FAILED = "Error: Could not create file ";
    const std::string MSG_NO_SHAPES = "Error: No shapes ";
    const std::string MSG_WARNING_PREFIX = "Warning: ";
    const std::string MSG_SHAPE_SKIPPED = " (shape skipped)";

    const std::string EX_UNSUPPORTED_SHAPE = "MakeDrawable: unsupported IShape type";

    const std::string OUT_PERIMETER_PREFIX = ": P=";
    const std::string OUT_AREA_SEPARATOR = "; S=";
    const std::string OUT_LINE_END = "\n";
}