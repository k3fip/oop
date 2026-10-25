#include "ShapeAdapter.h"
#include <SFML/Graphics.hpp>

std::vector<float> ShapeAdapter::ExtractNumbers(const std::string& line) {
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

std::vector<std::unique_ptr<IShape>> ShapeAdapter::ReadShapes(const std::string& filename) {
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
            std::vector<float> nums = ExtractNumbers(line);

            if (nums.size() >= 6) {
                shapes.push_back(std::make_unique<Triangle>(
                    Point(nums[0], nums[1]),
                    Point(nums[2], nums[3]),
                    Point(nums[4], nums[5])
                ));
            }
        }
        else if (line.find(Config::KW_RECTANGLE) == 0) {
            std::vector<float> nums = ExtractNumbers(line);

            if (nums.size() >= 4) {
                shapes.push_back(std::make_unique<Rectangle>(
                    Point(nums[0], nums[1]),
                    Point(nums[2], nums[3])
                ));
            }
        }
        else if (line.find(Config::KW_CIRCLE) == 0) {
            std::vector<float> nums = ExtractNumbers(line);

            if (nums.size() >= 3) {
                shapes.push_back(std::make_unique<Circle>(
                    Point(nums[0], nums[1]),
                    nums[2]
                ));
            }
        }
    }
    return shapes;
}

void ShapeAdapter::WriteResults(const std::string& filename, const std::vector<std::unique_ptr<IShape>>& shapes) {
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

        file << type << ": P=" << shape->Perimeter() << "; S=" << shape->Area() << "\n";
    }
}

void ShapeAdapter::DrawTriangle(Triangle* shape, sf::RenderWindow& window)
{
    sf::ConvexShape shapeSF;
    shapeSF.setPointCount(3);
    shapeSF.setPoint(0, { shape->GetP1().x, shape->GetP1().y });
    shapeSF.setPoint(1, { shape->GetP2().x, shape->GetP2().y });
    shapeSF.setPoint(2, { shape->GetP3().x, shape->GetP3().y });
    shapeSF.setFillColor(Config::TRIANGLE_FILL_COLOR);
    shapeSF.setOutlineThickness(Config::TRIANGLE_OUTLINE_THICKNESS);
    shapeSF.setOutlineColor(Config::TRIANGLE_OUTLINE_COLOR);

    window.draw(shapeSF);
}

void ShapeAdapter::DrawCircle(Circle* shape, sf::RenderWindow& window)
{
    sf::CircleShape shapeSF;

    shapeSF.setRadius(shape->GetRadius());
    shapeSF.setPosition({ shape->GetCenter().x - shape->GetRadius(), shape->GetCenter().y - shape->GetRadius() });
    shapeSF.setFillColor(Config::CIRCLE_FILL_COLOR);
    shapeSF.setOutlineThickness(Config::CIRCLE_OUTLINE_THICKNESS);
    shapeSF.setOutlineColor(Config::CIRCLE_OUTLINE_COLOR);
    
    window.draw(shapeSF);
}

void ShapeAdapter::DrawRectangle(Rectangle* shape, sf::RenderWindow& window)
{
    sf::RectangleShape shapeSF;

    float left = std::min(shape->GetP1().x, shape->GetP2().x);
    float top = std::min(shape->GetP1().y, shape->GetP2().y);

    shapeSF.setPosition({ left, top });
    shapeSF.setSize({ shape->GetWidth(), shape->GetHeight()});
    shapeSF.setFillColor(Config::RECTANGLE_FILL_COLOR);
    shapeSF.setOutlineThickness(Config::RECTANGLE_OUTLINE_THICKNESS);
    shapeSF.setOutlineColor(Config::RECTANGLE_OUTLINE_COLOR); 

    window.draw(shapeSF);
}

void ShapeAdapter::RenderShapes()
{
    auto shapes = ShapeAdapter::ReadShapes(Config::INPUT_FILE);
    if (shapes.empty()) 
    {
        std::cerr << "Error: No shapes " << std::endl;
        return;
    }

    ShapeAdapter::WriteResults(Config::OUTPUT_FILE, shapes);

    sf::RenderWindow window(
        sf::VideoMode({ Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT }),
        Config::WINDOW_TITLE
    );

    window.setFramerateLimit(Config::FRAMERATE_LIMIT);

    while (window.isOpen()) 
    {
        while (const std::optional event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
        }

        window.clear(Config::BACKGROUND_COLOR);

        for (const auto& shape : shapes) 
        {
            if (auto* tri = dynamic_cast<Triangle*>(shape.get()))
            {
                ShapeAdapter::DrawTriangle(tri, window);
            }
            else if (auto* rect = dynamic_cast<Rectangle*>(shape.get()))
            {
                ShapeAdapter::DrawRectangle(rect, window);
            }
            else if (auto* circle = dynamic_cast<Circle*>(shape.get()))
            {
                ShapeAdapter::DrawCircle(circle, window);
            };
        }

        window.display();
    }
}