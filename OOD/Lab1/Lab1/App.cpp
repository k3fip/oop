#include "App.h"

#include <SFML/Graphics.hpp>


std::vector<float> App::ExtractNumbers(const std::string& line)
{
    std::vector<float> numbers;
    std::stringstream ss(line);
    std::string token;

    while (ss >> token)
    {
        size_t eqPos = token.find(Config::ASSIGN_CHAR);
        if (eqPos != std::string::npos)
        {
            token = token.substr(eqPos + 1);
        }

        if (!token.empty() && token.back() == Config::TERMINATOR)
        {
            token.pop_back();
        }

        std::replace(token.begin(), token.end(), Config::DECIMAL_COMMA, ' ');

        std::stringstream tokenStream(token);
        float num;
        while (tokenStream >> num)
        {
            numbers.push_back(num);
        }
    }
    return numbers;
}

std::vector<std::unique_ptr<IShape>> App::ReadShapes(const std::string& filename)
{
    std::vector<std::unique_ptr<IShape>> shapes;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << Config::MSG_FILE_OPEN_FAILED << filename << std::endl;
        return shapes;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        if (line.find(Config::KW_TRIANGLE) == 0)
        {
            std::vector<float> nums = ExtractNumbers(line);

            if (nums.size() >= Config::TRIANGLE_COORDS)
            {
                shapes.push_back(std::make_unique<Triangle>(
                    Point(nums[0], nums[1]),
                    Point(nums[2], nums[3]),
                    Point(nums[4], nums[5])
                ));
            }
        }
        else if (line.find(Config::KW_RECTANGLE) == 0)
        {
            std::vector<float> nums = ExtractNumbers(line);

            if (nums.size() >= Config::RECTANGLE_COORDS)
            {
                shapes.push_back(std::make_unique<Rectangle>(
                    Point(nums[0], nums[1]),
                    Point(nums[2], nums[3])
                ));
            }
        }
        else if (line.find(Config::KW_CIRCLE) == 0)
        {
            std::vector<float> nums = ExtractNumbers(line);

            if (nums.size() >= Config::CIRCLE_COORDS)
            {
                shapes.push_back(std::make_unique<Circle>(
                    Point(nums[0], nums[1]),
                    nums[2]
                ));
            }
        }
    }
    return shapes;
}

std::unique_ptr<IDrawable> App::MakeDrawable(const IShape& shape)
{
    if (auto* tri = dynamic_cast<const Triangle*>(&shape))
    {
        return std::make_unique<ShapeAdapter<sf::ConvexShape, Triangle>>(*tri);
    }

    if (auto* rect = dynamic_cast<const Rectangle*>(&shape))
    {
        return std::make_unique<ShapeAdapter<sf::RectangleShape, Rectangle>>(*rect);
    }

    if (auto* circle = dynamic_cast<const Circle*>(&shape))
    {
        return std::make_unique<ShapeAdapter<sf::CircleShape, Circle>>(*circle);
    }

    throw std::invalid_argument(std::string(Config::EX_UNSUPPORTED_SHAPE));
}

void App::WriteResults(const std::string& filename,
    const std::vector<std::unique_ptr<IShape>>& shapes)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << Config::MSG_FILE_CREATE_FAILED << filename << std::endl;
        return;
    }

    for (const auto& shape : shapes)
    {
        const IShape* raw = shape.get();
        std::string type(Config::KW_UNKNOWN);

        if (dynamic_cast<const Triangle*>(raw))       type = Config::KW_TRIANGLE;
        else if (dynamic_cast<const Rectangle*>(raw)) type = Config::KW_RECTANGLE;
        else if (dynamic_cast<const Circle*>(raw))    type = Config::KW_CIRCLE;

        file << type
            << Config::OUT_PERIMETER_PREFIX << shape->Perimeter()
            << Config::OUT_AREA_SEPARATOR << shape->Area()
            << Config::OUT_LINE_END;
    }
}

void App::RenderShapes()
{
    auto shapes = App::ReadShapes(std::string(Config::INPUT_FILE));
    if (shapes.empty())
    {
        std::cerr << Config::MSG_NO_SHAPES << std::endl;
        return;
    }

    App::WriteResults(std::string(Config::OUTPUT_FILE), shapes);

    std::vector<std::unique_ptr<IDrawable>> drawables;
    drawables.reserve(shapes.size());
    for (const auto& shape : shapes)
    {
        try
        {
            drawables.push_back(App::MakeDrawable(*shape));
        }
        catch (const std::invalid_argument& ex)
        {
            std::cerr << Config::MSG_WARNING_PREFIX << ex.what()
                << Config::MSG_SHAPE_SKIPPED << std::endl;
        }
    }

    sf::RenderWindow window(
        sf::VideoMode({ Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT }),
        std::string(Config::WINDOW_TITLE)
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

        for (const auto& drawable : drawables)
        {
            drawable->Draw(window);
        }

        window.display();
    }
}