#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Circle.h"
#include "ShapeAdapter.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class App
{

public:
	static void RenderShapes();

private:
	static std::vector<float> ExtractNumbers(const std::string& line);
	static std::vector<std::unique_ptr<IShape>> ReadShapes(const std::string& filename);
	static std::unique_ptr<IDrawable> MakeDrawable(const IShape& shape);
	static void WriteResults(const std::string& filename, const std::vector<std::unique_ptr<IShape>>& shapes);
};