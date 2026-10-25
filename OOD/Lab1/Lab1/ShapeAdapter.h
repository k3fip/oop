#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Circle.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ShapeAdapter
{

public:
	static void RenderShapes();

private:
	static std::vector<float> ExtractNumbers(const std::string& line);
	static std::vector<std::unique_ptr<IShape>> ReadShapes(const std::string& filename);
	static void WriteResults(const std::string& filename, const std::vector<std::unique_ptr<IShape>>& shapes);

	static void DrawTriangle(Triangle* shape, sf::RenderWindow& window);
	static void DrawCircle(Circle* shape, sf::RenderWindow& window);
	static void DrawRectangle(Rectangle* shape, sf::RenderWindow& window);
};