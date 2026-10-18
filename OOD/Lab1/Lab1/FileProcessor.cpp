#include "FileProcessor.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static std::vector<float> extractNumbers(const std::string& line) {
    std::vector<float> numbers;
    std::stringstream ss(line);
    std::string token;

    while (ss >> token) {

        size_t eqPos = token.find('=');
        if (eqPos != std::string::npos) {
            token = token.substr(eqPos + 1);
        }

        if (!token.empty() && token.back() == ';') {
            token.pop_back();
        }

        std::replace(token.begin(), token.end(), ',', ' ');

        std::stringstream tokenStream(token);
        float num;
        while (tokenStream >> num) {
            numbers.push_back(num);
        }
    }
    return numbers;
}

std::vector<std::unique_ptr<IShape>> FileProcessor::readShapes(const std::string& filename) {
    std::vector<std::unique_ptr<IShape>> shapes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return shapes;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line.find(Config::KW_TRIANGLE) == 0) {
            std::vector<float> nums = extractNumbers(line);

            if (nums.size() >= 6) {
                shapes.push_back(std::make_unique<Triangle>(
                    sf::Vector2f(nums[0], nums[1]),
                    sf::Vector2f(nums[2], nums[3]),
                    sf::Vector2f(nums[4], nums[5])
                ));
            }
        }
        else if (line.find(Config::KW_RECTANGLE) == 0) {
            std::vector<float> nums = extractNumbers(line);

            if (nums.size() >= 4) {
                shapes.push_back(std::make_unique<Rectangle>(
                    sf::Vector2f(nums[0], nums[1]),
                    sf::Vector2f(nums[2], nums[3])
                ));
            }
        }
        else if (line.find(Config::KW_CIRCLE) == 0) {
            std::vector<float> nums = extractNumbers(line);

            if (nums.size() >= 3) {
                shapes.push_back(std::make_unique<Circle>(
                    sf::Vector2f(nums[0], nums[1]),
                    nums[2]
                ));
            }
        }
    }
    return shapes;
}

void FileProcessor::writeResults(const std::string& filename, const std::vector<std::unique_ptr<IShape>>& shapes) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file " << filename << std::endl;
        return;
    }

    for (const auto& shape : shapes) {
        std::string type = Config::KW_UNKNOWN;

        if (dynamic_cast<Triangle*>(shape.get())) type = Config::KW_TRIANGLE;
        else if (dynamic_cast<Rectangle*>(shape.get())) type = Config::KW_RECTANGLE;
        else if (dynamic_cast<Circle*>(shape.get())) type = Config::KW_CIRCLE;

        file << type << ": P=" << shape->perimeter() << "; S=" << shape->area() << "\n";
    }
}