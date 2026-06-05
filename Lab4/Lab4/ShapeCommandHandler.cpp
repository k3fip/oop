#include "ShapeCommandHandler.h"

void ShapeCommandHandler::ProcessShapes()
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

void ShapeCommandHandler::ProcessCommand(std::vector<std::unique_ptr<IShape>>& shapes, const std::string& commandLine)
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

ParsedCommand ShapeCommandHandler::ParseCommand(const std::string& input)
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

uint32_t ShapeCommandHandler::ParseColor(const std::string& colorStr)
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

const IShape* ShapeCommandHandler::FindShapeWithMaxArea(const std::vector<std::unique_ptr<IShape>>& shapes)
{
    if (shapes.empty()) return nullptr;

    auto maxAreaShape = std::max_element(shapes.begin(), shapes.end(), 
        [](const std::unique_ptr<IShape>& first, const std::unique_ptr<IShape>& second)
        {
            return first->GetArea() < second->GetArea();
        });

    return maxAreaShape->get();
}

const IShape* ShapeCommandHandler::FindShapeWithMinPerimeter(const std::vector<std::unique_ptr<IShape>>& shapes)
{
    if (shapes.empty()) return nullptr;

    auto minPerimeterShape = std::min_element(shapes.begin(), shapes.end(), 
        [](const std::unique_ptr<IShape>& first, const std::unique_ptr<IShape>& second)
        {
            return first->GetPerimeter() < second->GetPerimeter();
        });

    return minPerimeterShape->get();
}

/*const IShape* maxShape = shapes[0].get();
    double maxArea = shapes[0]->GetArea();

    for (size_t i = 1; i < shapes.size(); ++i)
    {
        double area = shapes[i]->GetArea();
        if (area > maxArea)
        {
            maxArea = area;
            maxShape = shapes[i].get();
        }
    }*/

/*const IShape* minShape = shapes[0].get();
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

    return minShape;*/

void ShapeCommandHandler::HandleLineSegment(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
{
    if (args.size() != LINE_SEGMENT_ARGS_COUNT)
    {
        std::cout << INVALID_ARGUMENT_COUNT_LINE_SEGMENT << args.size() << std::endl;
        return;
    }

    try
    {
        double startX = std::stod(args[LINE_SEGMENT_ARGS_COUNT - 5]);
        double startY = std::stod(args[LINE_SEGMENT_ARGS_COUNT - 4]);
        double endX = std::stod(args[LINE_SEGMENT_ARGS_COUNT - 3]);
        double endY = std::stod(args[LINE_SEGMENT_ARGS_COUNT - 2]);
        uint32_t outlineColor = ParseColor(args[LINE_SEGMENT_ARGS_COUNT - 1]);

        shapes.push_back(std::make_unique<CLineSegment>(
            startX,
            startY,
            endX,
            endY,
            outlineColor
        ));
    }
    catch (const std::exception&)
    {
        std::cout << INVALID_COORDINATES << std::endl;
    }
}

void ShapeCommandHandler::HandleTriangle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
{
    if (args.size() != TRIANGLE_ARGS_COUNT)
    {
        std::cout << INVALID_ARGUMENT_COUNT_TRIANGLE << args.size() << std::endl;
        return;
    }

    try
    {
        double v1x = std::stod(args[TRIANGLE_ARGS_COUNT - 8]);
        double v1y = std::stod(args[TRIANGLE_ARGS_COUNT - 7]);
        double v2x = std::stod(args[TRIANGLE_ARGS_COUNT - 6]);
        double v2y = std::stod(args[TRIANGLE_ARGS_COUNT - 5]);
        double v3x = std::stod(args[TRIANGLE_ARGS_COUNT - 4]);
        double v3y = std::stod(args[TRIANGLE_ARGS_COUNT - 3]);
        uint32_t outlineColor = ParseColor(args[TRIANGLE_ARGS_COUNT - 2]);
        uint32_t fillColor = ParseColor(args[TRIANGLE_ARGS_COUNT - 1]);

        shapes.push_back(std::make_unique<CTriangle>(
            CPoint(v1x, v1y),
            CPoint(v2x, v2y),
            CPoint(v3x, v3y),
            outlineColor,
            fillColor
        ));
    }
    catch (const std::exception&)
    {
        std::cout << INVALID_COORDINATES << std::endl;
    }
}

void ShapeCommandHandler::HandleRectangle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
{
    if (args.size() != RECTANGLE_ARGS_COUNT)
    {
        std::cout << INVALID_ARGUMENT_COUNT_RECTANGLE << args.size() << std::endl;
        return;
    }

    try
    {
        double leftX = std::stod(args[RECTANGLE_ARGS_COUNT - 6]);
        double topY = std::stod(args[RECTANGLE_ARGS_COUNT - 5]);
        double width = std::stod(args[RECTANGLE_ARGS_COUNT - 4]);
        double height = std::stod(args[RECTANGLE_ARGS_COUNT - 3]);
        uint32_t outlineColor = ParseColor(args[RECTANGLE_ARGS_COUNT - 2]);
        uint32_t fillColor = ParseColor(args[RECTANGLE_ARGS_COUNT - 1]);

        shapes.push_back(std::make_unique<CRectangle>(
            leftX, topY, width, height,
            outlineColor,
            fillColor
        ));
    }
    catch (const std::exception&)
    {
        std::cout << INVALID_DIMENSIONS << std::endl;
    }
}

void ShapeCommandHandler::HandleCircle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args)
{
    if (args.size() != CIRCLE_ARGS_COUNT)
    {
        std::cout << INVALID_ARGUMENT_COUNT_CIRCLE << args.size() << std::endl;
        return;
    }

    try
    {
        double centerX = std::stod(args[CIRCLE_ARGS_COUNT - 5]);
        double centerY = std::stod(args[CIRCLE_ARGS_COUNT - 4]);
        double radius = std::stod(args[CIRCLE_ARGS_COUNT - 3]);
        uint32_t outlineColor = ParseColor(args[CIRCLE_ARGS_COUNT - 2]);
        uint32_t fillColor = ParseColor(args[CIRCLE_ARGS_COUNT - 1]);

        shapes.push_back(std::make_unique<CCircle>(
            centerX, centerY, radius,
            outlineColor,
            fillColor
        ));
    }
    catch (const std::exception&)
    {
        std::cout << INVALID_COORDINATES_AND_RADIUS << std::endl;
    }
}

void ShapeCommandHandler::PrintResults(const std::vector<std::unique_ptr<IShape>>& shapes)
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

void ShapeCommandHandler::PrintShapeDetails(const IShape* shape)
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