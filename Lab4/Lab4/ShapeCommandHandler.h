#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <map>
#include <vector>
#include <memory>
#include <iomanip>
#include <limits>
#include "Shapes.h"

class ShapeCommandHandler
{
public:
    enum class CommandType
    {
        LINE_SEGMENT,
        TRIANGLE,
        RECTANGLE,
        CIRCLE,
        UNKNOWN
    };

    struct ParsedCommand
    {
        CommandType type = CommandType::UNKNOWN;
        std::vector<std::string> arguments;
    };

    static constexpr char HASH_CHAR = '#';
    static constexpr char MINUS_CHAR = '-';
    static constexpr char DECIMAL_POINT = '.';
    static constexpr char ZERO_CHAR = '0';

    static constexpr int HEX_BASE = 16;

    static constexpr const char* PROMPT_MESSAGE = "Enter shape commands (LineSegment, Triangle, Rectangle, Circle). Press Ctrl+Z (Windows) or Ctrl+D (Unix) to finish:";
    static constexpr const char* UNKNOWN_COMMAND = "Unknown command";

    static constexpr const char* INVALID_ARGUMENT_COUNT_LINE_SEGMENT = "Invalid number of arguments for LineSegment: expected 5 (startX startY endX endY outlineColor), got ";
    static constexpr const char* INVALID_ARGUMENT_COUNT_TRIANGLE = "Invalid number of arguments for Triangle: expected 8 (v1x v1y v2x v2y v3x v3y outlineColor fillColor), got ";
    static constexpr const char* INVALID_ARGUMENT_COUNT_RECTANGLE = "Invalid number of arguments for Rectangle: expected 6 (leftX topY width height outlineColor fillColor), got ";
    static constexpr const char* INVALID_ARGUMENT_COUNT_CIRCLE = "Invalid number of arguments for Circle: expected 5 (centerX centerY radius outlineColor fillColor), got ";

    static constexpr const char* INVALID_COORDINATES = "Invalid argument: coordinates must be numbers";
    static constexpr const char* INVALID_DIMENSIONS = "Invalid argument: dimensions must be numbers";
    static constexpr const char* INVALID_COORDINATES_AND_RADIUS = "Invalid argument: coordinates and radius must be numbers";

    static constexpr const char* NO_SHAPES_MESSAGE = "No shapes were entered.";
    static constexpr const char* ALL_SHAPES_HEADER = "All Shapes";
    static constexpr const char* MAX_AREA_HEADER = "Shape with Maximum Area";
    static constexpr const char* MIN_PERIMETER_HEADER = "Shape with Minimum Perimeter";

    static constexpr const char* AREA_LABEL = "Area: ";
    static constexpr const char* PERIMETER_LABEL = "Perimeter: ";
    static constexpr const char* OUTLINE_COLOR_LABEL = "Outline Color: #";
    static constexpr const char* FILL_COLOR_LABEL = "Fill Color: #";
    static constexpr const char* SHAPE_INDEX_SEPARATOR = ". ";

    static constexpr const char* LINE_SEGMENT_CMD = "linesegment";
    static constexpr const char* TRIANGLE_CMD = "triangle";
    static constexpr const char* RECTANGLE_CMD = "rectangle";
    static constexpr const char* CIRCLE_CMD = "circle";

    static constexpr size_t LINE_SEGMENT_ARGS_COUNT = 5;
    static constexpr size_t TRIANGLE_ARGS_COUNT = 8;
    static constexpr size_t RECTANGLE_ARGS_COUNT = 6;
    static constexpr size_t CIRCLE_ARGS_COUNT = 5;

    static constexpr int COLOR_WIDTH = 6;
    static constexpr int DOUBLE_PRECISION = 2;

    inline static const std::map<std::string, CommandType> COMMAND_MAP = {
        {LINE_SEGMENT_CMD, CommandType::LINE_SEGMENT},
        {TRIANGLE_CMD, CommandType::TRIANGLE},
        {RECTANGLE_CMD, CommandType::RECTANGLE},
        {CIRCLE_CMD, CommandType::CIRCLE}
    };

    static void ProcessShapes()
    {
        std::vector<std::unique_ptr<IShape>> shapes;
        std::string commandLine;

        std::cout << PROMPT_MESSAGE << std::endl;

        while (std::getline(std::cin, commandLine))
        {
            if (commandLine.empty()) continue;
            ProcessCommand(shapes, commandLine);
        }

        PrintResults(shapes);
    }

    static void ProcessCommand(std::vector<std::unique_ptr<IShape>>& shapes, const std::string& commandLine)
    {
        ParsedCommand cmd = ParseCommand(commandLine);

        switch (cmd.type)
        {
        case CommandType::LINE_SEGMENT:
            HandleLineSegment(shapes, cmd.arguments);
            break;

        case CommandType::TRIANGLE:
            HandleTriangle(shapes, cmd.arguments);
            break;

        case CommandType::RECTANGLE:
            HandleRectangle(shapes, cmd.arguments);
            break;

        case CommandType::CIRCLE:
            HandleCircle(shapes, cmd.arguments);
            break;

        case CommandType::UNKNOWN:
        default:
            std::cout << UNKNOWN_COMMAND << std::endl;
            break;
        }
    }

    static ParsedCommand ParseCommand(const std::string& input)
    {
        std::istringstream iss(input);
        std::string action;
        iss >> action;

        if (action.empty())
        {
            return { CommandType::UNKNOWN, {} };
        }

        std::string actionLower = action;
        std::transform(actionLower.begin(), actionLower.end(), actionLower.begin(), ::tolower);

        auto it = COMMAND_MAP.find(actionLower);
        if (it != COMMAND_MAP.end())
        {
            ParsedCommand result;
            result.type = it->second;

            std::string arg;
            while (iss >> arg)
            {
                result.arguments.push_back(arg);
            }

            return result;
        }

        return { CommandType::UNKNOWN, {} };
    }

    static uint32_t ParseColor(const std::string& colorStr)
    {
        if (colorStr.length() >= 1 && colorStr[0] == HASH_CHAR)
        {
            std::string hexStr = colorStr.substr(1);
            try
            {
                return static_cast<uint32_t>(std::stoul(hexStr, nullptr, HEX_BASE));
            }
            catch (...)
            {
                return 0;
            }
        }
        else
        {
            try
            {
                return static_cast<uint32_t>(std::stoul(colorStr, nullptr, HEX_BASE));
            }
            catch (...)
            {
                return 0;
            }
        }
    }

    static bool IsDouble(const std::string& str)
    {
        if (str.empty()) return false;

        size_t start = 0;
        if (str[0] == MINUS_CHAR)
        {
            if (str.length() == 1) return false;
            start = 1;
        }

        bool hasDecimalPoint = false;
        for (size_t i = start; i < str.length(); ++i)
        {
            if (str[i] == DECIMAL_POINT)
            {
                if (hasDecimalPoint) return false;
                hasDecimalPoint = true;
            }
            else if (!isdigit(str[i]))
            {
                return false;
            }
        }
        return true;
    }

    static const IShape* FindShapeWithMaxArea(const std::vector<std::unique_ptr<IShape>>& shapes)
    {
        if (shapes.empty()) return nullptr;

        const IShape* maxShape = shapes[0].get();
        double maxArea = shapes[0]->GetArea();

        for (size_t i = 1; i < shapes.size(); ++i)
        {
            double area = shapes[i]->GetArea();
            if (area > maxArea)
            {
                maxArea = area;
                maxShape = shapes[i].get();
            }
        }

        return maxShape;
    }

    static const IShape* FindShapeWithMinPerimeter(const std::vector<std::unique_ptr<IShape>>& shapes)
    {
        if (shapes.empty()) return nullptr;

        const IShape* minShape = shapes[0].get();
        double minPerimeter = shapes[0]->GetPerimeter();

        for (size_t i = 1; i < shapes.size(); ++i)
        {
            double perimeter = shapes[i]->GetPerimeter();
            if (perimeter < minPerimeter)
            {
                minPerimeter = perimeter;
                minShape = shapes[i].get();
            }
        }

        return minShape;
    }

    static void HandleLineSegment(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
    {
        if (args.size() != LINE_SEGMENT_ARGS_COUNT)
        {
            std::cout << INVALID_ARGUMENT_COUNT_LINE_SEGMENT << args.size() << std::endl;
            return;
        }

        if (!IsDouble(args[0]) || !IsDouble(args[1]) || !IsDouble(args[2]) || !IsDouble(args[3]))
        {
            std::cout << INVALID_COORDINATES << std::endl;
            return;
        }

        double startX = std::stod(args[0]);
        double startY = std::stod(args[1]);
        double endX = std::stod(args[2]);
        double endY = std::stod(args[3]);
        uint32_t outlineColor = ParseColor(args[4]);

        shapes.push_back(std::make_unique<CLineSegment>(
            startX,
            startY,
            endX,
            endY,
            outlineColor
        ));
    }

    static void HandleTriangle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
    {
        if (args.size() != TRIANGLE_ARGS_COUNT)
        {
            std::cout << INVALID_ARGUMENT_COUNT_TRIANGLE << args.size() << std::endl;
            return;
        }

        for (int i = 0; i < 6; ++i)
        {
            if (!IsDouble(args[i]))
            {
                std::cout << INVALID_COORDINATES << std::endl;
                return;
            }
        }

        double v1x = std::stod(args[0]);
        double v1y = std::stod(args[1]);
        double v2x = std::stod(args[2]);
        double v2y = std::stod(args[3]);
        double v3x = std::stod(args[4]);
        double v3y = std::stod(args[5]);
        uint32_t outlineColor = ParseColor(args[6]);
        uint32_t fillColor = ParseColor(args[7]);

        shapes.push_back(std::make_unique<CTriangle>(
            CPoint(v1x, v1y),
            CPoint(v2x, v2y),
            CPoint(v3x, v3y),
            outlineColor,
            fillColor
        ));
    }

    static void HandleRectangle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
    {
        if (args.size() != RECTANGLE_ARGS_COUNT)
        {
            std::cout << INVALID_ARGUMENT_COUNT_RECTANGLE << args.size() << std::endl;
            return;
        }

        for (int i = 0; i < 4; ++i)
        {
            if (!IsDouble(args[i]))
            {
                std::cout << INVALID_DIMENSIONS << std::endl;
                return;
            }
        }

        double leftX = std::stod(args[0]);
        double topY = std::stod(args[1]);
        double width = std::stod(args[2]);
        double height = std::stod(args[3]);
        uint32_t outlineColor = ParseColor(args[4]);
        uint32_t fillColor = ParseColor(args[5]);

        shapes.push_back(std::make_unique<CRectangle>(
            leftX, topY, width, height,
            outlineColor,
            fillColor
        ));
    }

    static void HandleCircle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
    {
        if (args.size() != CIRCLE_ARGS_COUNT)
        {
            std::cout << INVALID_ARGUMENT_COUNT_CIRCLE << args.size() << std::endl;
            return;
        }

        for (int i = 0; i < 3; ++i)
        {
            if (!IsDouble(args[i]))
            {
                std::cout << INVALID_COORDINATES_AND_RADIUS << std::endl;
                return;
            }
        }

        double centerX = std::stod(args[0]);
        double centerY = std::stod(args[1]);
        double radius = std::stod(args[2]);
        uint32_t outlineColor = ParseColor(args[3]);
        uint32_t fillColor = ParseColor(args[4]);

        shapes.push_back(std::make_unique<CCircle>(
            centerX, centerY, radius,
            outlineColor,
            fillColor
        ));
    }

    static void PrintResults(const std::vector<std::unique_ptr<IShape>>& shapes)
    {
        if (shapes.empty())
        {
            std::cout << NO_SHAPES_MESSAGE << std::endl;
            return;
        }

        std::cout << ALL_SHAPES_HEADER << std::endl;
        for (size_t i = 0; i < shapes.size(); ++i)
        {
            std::cout << (i + 1) << SHAPE_INDEX_SEPARATOR << shapes[i]->ToString() << std::endl;
        }

        const IShape* maxAreaShape = FindShapeWithMaxArea(shapes);
        const IShape* minPerimeterShape = FindShapeWithMinPerimeter(shapes);

        std::cout << MAX_AREA_HEADER << std::endl;
        if (maxAreaShape)
        {
            PrintShapeDetails(maxAreaShape);
        }

        std::cout << MIN_PERIMETER_HEADER << std::endl;
        if (minPerimeterShape)
        {
            PrintShapeDetails(minPerimeterShape);
        }
    }

    static void PrintShapeDetails(const IShape* shape)
    {
        std::cout << shape->ToString() << std::endl;
        std::cout << AREA_LABEL << std::fixed << std::setprecision(DOUBLE_PRECISION) << shape->GetArea() << std::endl;
        std::cout << PERIMETER_LABEL << std::fixed << std::setprecision(DOUBLE_PRECISION) << shape->GetPerimeter() << std::endl;
        std::cout << OUTLINE_COLOR_LABEL << std::hex << std::setfill(ZERO_CHAR) << std::setw(COLOR_WIDTH) << shape->GetOutlineColor() << std::dec << std::endl;

        const ISolidShape* solidShape = dynamic_cast<const ISolidShape*>(shape);
        if (solidShape)
        {
            std::cout << FILL_COLOR_LABEL << std::hex << std::setfill(ZERO_CHAR) << std::setw(COLOR_WIDTH) << solidShape->GetFillColor() << std::dec << std::endl;
        }
    }
};