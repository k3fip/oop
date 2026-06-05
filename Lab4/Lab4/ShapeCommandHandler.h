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

#include "IShape.h"
#include "ISolidShape.h"
#include "CLineSegment.h"
#include "CCircle.h"
#include "CTriangle.h"
#include "CRectangle.h"
#include "CPoint.h"

static constexpr char HASH_CHAR = '#';
static constexpr char MINUS_CHAR = '-';
static constexpr char DECIMAL_POINT = '.';
static constexpr char ZERO_CHAR = '0';

static constexpr size_t HEX_BASE = 16;

static constexpr const char* PROMPT_MESSAGE = "Enter shape commands (LineSegment, Triangle, Rectangle, Circle). Press Ctrl+Z to finish:";
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

static constexpr size_t COLOR_WIDTH = 6;
static constexpr size_t DOUBLE_PRECISION = 2;

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

inline static const std::map<std::string, CommandType> COMMAND_MAP = {
{LINE_SEGMENT_CMD, CommandType::LINE_SEGMENT},
{TRIANGLE_CMD, CommandType::TRIANGLE},
{RECTANGLE_CMD, CommandType::RECTANGLE},
{CIRCLE_CMD, CommandType::CIRCLE}
};

class ShapeCommandHandler
{
public:

    static void ProcessShapes();
    static void ProcessCommand(std::vector<std::unique_ptr<IShape>>& shapes, const std::string& commandLine);
    static ParsedCommand ParseCommand(const std::string& input);
    static uint32_t ParseColor(const std::string& colorStr);
    static const IShape* FindShapeWithMaxArea(const std::vector<std::unique_ptr<IShape>>& shapes);
    static const IShape* FindShapeWithMinPerimeter(const std::vector<std::unique_ptr<IShape>>& shapes);
    static void HandleLineSegment(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args);
    static void HandleTriangle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args);
    static void HandleRectangle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args);
    static void HandleCircle(std::vector<std::unique_ptr<IShape>>& shapes, const std::vector<std::string>& args);
    static void PrintResults(const std::vector<std::unique_ptr<IShape>>& shapes);
    static void PrintShapeDetails(const IShape* shape);
};