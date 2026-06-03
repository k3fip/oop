#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "../Lab4/Shapes.h"
#include "../Lab4/ShapeCommandHandler.h"

class CaptureOutput
{
public:
    CaptureOutput() : old(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CaptureOutput() { std::cout.rdbuf(old); }
    std::string getOutput() { return buffer.str(); }
    void clear() { buffer.str(""); buffer.clear(); }

private:
    std::stringstream buffer;
    std::streambuf* old;
};

TEST_CASE("CPoint")
{
    SECTION("Конструктор и координаты")
    {
        CPoint p(1.5, 2.5);
        REQUIRE(p.x == 1.5);
        REQUIRE(p.y == 2.5);
    }

    SECTION("Расстояние до другой точки")
    {
        CPoint p1(0, 0);
        CPoint p2(3, 4);
        REQUIRE(p1.DistanceTo(p2) == 5.0);

        CPoint p3(1, 1);
        CPoint p4(1, 1);
        REQUIRE(p3.DistanceTo(p4) == 0.0);
    }

    SECTION("ToString")
    {
        CPoint p(1.5, 2.5);
        REQUIRE(p.ToString() == "(1.50, 2.50)");

        CPoint p2(0, 0);
        REQUIRE(p2.ToString() == "(0.00, 0.00)");
    }
}

TEST_CASE("CLineSegment")
{
    SECTION("Конструктор и основные свойства")
    {
        CLineSegment line(CPoint(0, 0), CPoint(3, 4), 0xFF0000);
        REQUIRE(line.GetArea() == 0.0);
        REQUIRE(line.GetPerimeter() == 5.0);
        REQUIRE(line.GetOutlineColor() == 0xFF0000);
    }

    SECTION("Граничные значения")
    {
        CLineSegment line(CPoint(1, 1), CPoint(1, 1), 0);
        REQUIRE(line.GetPerimeter() == 0.0);

        CLineSegment line2(CPoint(0, 0), CPoint(1000, 1000), 0xFFFFFF);
        REQUIRE(line2.GetPerimeter() > 0);
        REQUIRE(line2.GetOutlineColor() == 0xFFFFFF);
    }

    SECTION("ToString")
    {
        CLineSegment line(CPoint(0, 0), CPoint(3, 4), 0xFF0000);
        std::string str = line.ToString();
        REQUIRE(str.find("LineSegment") != std::string::npos);
        REQUIRE(str.find("(0.00, 0.00)") != std::string::npos);
        REQUIRE(str.find("(3.00, 4.00)") != std::string::npos);
        REQUIRE(str.find("5.00") != std::string::npos);
        REQUIRE(str.find("16711680") != std::string::npos);
    }
}

TEST_CASE("CTriangle")
{
    SECTION("Конструктор и площадь")
    {
        CTriangle triangle(
            CPoint(0, 0), CPoint(3, 0), CPoint(0, 4),
            0xFF0000, 0x00FF00
        );
        REQUIRE(triangle.GetArea() == 6.0);
        REQUIRE(triangle.GetPerimeter() == 12.0);
    }

    SECTION("Вырожденный треугольник")
    {
        CTriangle triangle(
            CPoint(0, 0), CPoint(1, 1), CPoint(2, 2),
            0, 0
        );
        REQUIRE(triangle.GetArea() == 0.0);
    }

    SECTION("Граничные значения")
    {
        CTriangle triangle(
            CPoint(0, 0), CPoint(1000, 0), CPoint(0, 1000),
            0xFFFFFF, 0x000000
        );
        REQUIRE(triangle.GetArea() == 500000.0);
        REQUIRE(triangle.GetOutlineColor() == 0xFFFFFF);
        REQUIRE(triangle.GetFillColor() == 0x000000);
    }

    SECTION("ToString")
    {
        CTriangle triangle(
            CPoint(0, 0), CPoint(3, 0), CPoint(0, 4),
            0xFF0000, 0x00FF00
        );
        std::string str = triangle.ToString();
        REQUIRE(str.find("Triangle") != std::string::npos);
        REQUIRE(str.find("(0.00, 0.00)") != std::string::npos);
        REQUIRE(str.find("(3.00, 0.00)") != std::string::npos);
        REQUIRE(str.find("(0.00, 4.00)") != std::string::npos);
        REQUIRE(str.find("6.00") != std::string::npos);
        REQUIRE(str.find("12.00") != std::string::npos);
    }
}

TEST_CASE("CRectangle")
{
    SECTION("Конструктор и площадь")
    {
        CRectangle rect(10, 20, 30, 40, 0xFF0000, 0x00FF00);
        REQUIRE(rect.GetArea() == 1200.0);
        REQUIRE(rect.GetPerimeter() == 140.0);
    }

    SECTION("Нулевые размеры")
    {
        CRectangle rect(0, 0, 0, 0, 0, 0);
        REQUIRE(rect.GetArea() == 0.0);
        REQUIRE(rect.GetPerimeter() == 0.0);
    }

    SECTION("Отрицательные размеры округляются до 0")
    {
        CRectangle rect(0, 0, -10, -20, 0, 0);
        REQUIRE(rect.GetWidth() == 0.0);
        REQUIRE(rect.GetHeight() == 0.0);
        REQUIRE(rect.GetArea() == 0.0);
    }

    SECTION("ToString")
    {
        CRectangle rect(10, 20, 30, 40, 0xFF0000, 0x00FF00);
        std::string str = rect.ToString();
        REQUIRE(str.find("Rectangle") != std::string::npos);
        REQUIRE(str.find("(10.00, 20.00)") != std::string::npos);
        REQUIRE(str.find("30.00") != std::string::npos);
        REQUIRE(str.find("40.00") != std::string::npos);
        REQUIRE(str.find("1200.00") != std::string::npos);
        REQUIRE(str.find("140.00") != std::string::npos);
    }
}

TEST_CASE("CCircle")
{
    SECTION("Конструктор и площадь")
    {
        CCircle circle(0, 0, 5, 0xFF0000, 0x00FF00);
        REQUIRE(std::abs(circle.GetArea() - 78.54) < 0.01);
        REQUIRE(std::abs(circle.GetPerimeter() - 31.42) < 0.01);
    }

    SECTION("Нулевой радиус")
    {
        CCircle circle(0, 0, 0, 0, 0);
        REQUIRE(circle.GetArea() == 0.0);
        REQUIRE(circle.GetPerimeter() == 0.0);
    }

    SECTION("Отрицательный радиус округляется до 0")
    {
        CCircle circle(0, 0, -5, 0, 0);
        REQUIRE(circle.GetRadius() == 0.0);
        REQUIRE(circle.GetArea() == 0.0);
    }

    SECTION("ToString")
    {
        CCircle circle(0, 0, 5, 0xFF0000, 0x00FF00);
        std::string str = circle.ToString();
        REQUIRE(str.find("Circle") != std::string::npos);
        REQUIRE(str.find("(0.00, 0.00)") != std::string::npos);
        REQUIRE(str.find("5.00") != std::string::npos);
        REQUIRE(str.find("16711680") != std::string::npos);
        REQUIRE(str.find("65280") != std::string::npos);
    }
}

TEST_CASE("ShapeCommandHandler - ParseColor")
{
    SECTION("Формат с #")
    {
        REQUIRE(ShapeCommandHandler::ParseColor("#FF0000") == 0xFF0000);
        REQUIRE(ShapeCommandHandler::ParseColor("#00FF00") == 0x00FF00);
        REQUIRE(ShapeCommandHandler::ParseColor("#0000FF") == 0x0000FF);
    }

    SECTION("Формат без #")
    {
        REQUIRE(ShapeCommandHandler::ParseColor("FF0000") == 0xFF0000);
        REQUIRE(ShapeCommandHandler::ParseColor("000000") == 0x000000);
    }

    SECTION("Граничные значения")
    {
        REQUIRE(ShapeCommandHandler::ParseColor("FFFFFF") == 0xFFFFFF);
        REQUIRE(ShapeCommandHandler::ParseColor("000000") == 0x000000);
    }

    SECTION("Ошибочные форматы")
    {
        REQUIRE(ShapeCommandHandler::ParseColor("") == 0);
        REQUIRE(ShapeCommandHandler::ParseColor("invalid") == 0);
        REQUIRE(ShapeCommandHandler::ParseColor("#GGGGGG") == 0);
    }
}

TEST_CASE("ShapeCommandHandler - IsDouble")
{
    SECTION("Валидные числа")
    {
        REQUIRE(ShapeCommandHandler::IsDouble("0"));
        REQUIRE(ShapeCommandHandler::IsDouble("123"));
        REQUIRE(ShapeCommandHandler::IsDouble("-5"));
        REQUIRE(ShapeCommandHandler::IsDouble("3.14"));
        REQUIRE(ShapeCommandHandler::IsDouble("-2.5"));
    }

    SECTION("Граничные значения")
    {
        REQUIRE(ShapeCommandHandler::IsDouble("0.0"));
        REQUIRE(ShapeCommandHandler::IsDouble("-0.0"));
        REQUIRE(ShapeCommandHandler::IsDouble("999999.999999"));
    }

    SECTION("Невалидные значения")
    {
        REQUIRE_FALSE(ShapeCommandHandler::IsDouble(""));
        REQUIRE_FALSE(ShapeCommandHandler::IsDouble("abc"));
        REQUIRE_FALSE(ShapeCommandHandler::IsDouble("12.3.4"));
        REQUIRE_FALSE(ShapeCommandHandler::IsDouble("-"));
    }
}

TEST_CASE("ShapeCommandHandler - ParseCommand")
{
    SECTION("Валидные команды")
    {
        auto cmd = ShapeCommandHandler::ParseCommand("LineSegment 0 0 5 5 FF0000");
        REQUIRE(cmd.type == ShapeCommandHandler::CommandType::LINE_SEGMENT);
        REQUIRE(cmd.arguments.size() == 5);
        REQUIRE(cmd.arguments[0] == "0");
        REQUIRE(cmd.arguments[4] == "FF0000");
    }

    SECTION("Регистронезависимость")
    {
        auto cmd = ShapeCommandHandler::ParseCommand("linesegment 0 0 5 5 FF0000");
        REQUIRE(cmd.type == ShapeCommandHandler::CommandType::LINE_SEGMENT);

        cmd = ShapeCommandHandler::ParseCommand("CIRCLE 0 0 5 FF0000 00FF00");
        REQUIRE(cmd.type == ShapeCommandHandler::CommandType::CIRCLE);
    }

    SECTION("Неизвестная команда")
    {
        auto cmd = ShapeCommandHandler::ParseCommand("unknown");
        REQUIRE(cmd.type == ShapeCommandHandler::CommandType::UNKNOWN);
    }

    SECTION("Пустая строка")
    {
        auto cmd = ShapeCommandHandler::ParseCommand("");
        REQUIRE(cmd.type == ShapeCommandHandler::CommandType::UNKNOWN);
    }

    SECTION("Команда с разным количеством аргументов")
    {
        auto cmd = ShapeCommandHandler::ParseCommand("LineSegment");
        REQUIRE(cmd.type == ShapeCommandHandler::CommandType::LINE_SEGMENT);
        REQUIRE(cmd.arguments.empty());

        cmd = ShapeCommandHandler::ParseCommand("Triangle 0 0 3 0 0 4 FF0000 00FF00 extra");
        REQUIRE(cmd.type == ShapeCommandHandler::CommandType::TRIANGLE);
        REQUIRE(cmd.arguments.size() == 9);
    }
}

TEST_CASE("ShapeCommandHandler - ProcessCommand")
{
    std::vector<std::unique_ptr<IShape>> shapes;

    SECTION("Обработка LineSegment")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 5 5 FF0000");
        REQUIRE(shapes.size() == 1);
        REQUIRE(shapes[0]->GetArea() == 0.0);
        REQUIRE(shapes[0]->GetPerimeter() > 0);
    }

    SECTION("Обработка Triangle")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "Triangle 0 0 3 0 0 4 FF0000 00FF00");
        REQUIRE(shapes.size() == 1);
        REQUIRE(shapes[0]->GetArea() == 6.0);
        REQUIRE(shapes[0]->GetPerimeter() == 12.0);
    }

    SECTION("Обработка Rectangle")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 10 20 30 40 FF0000 00FF00");
        REQUIRE(shapes.size() == 1);
        REQUIRE(shapes[0]->GetArea() == 1200.0);
        REQUIRE(shapes[0]->GetPerimeter() == 140.0);
    }

    SECTION("Обработка Circle")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 5 FF0000 00FF00");
        REQUIRE(shapes.size() == 1);
        REQUIRE(std::abs(shapes[0]->GetArea() - 78.54) < 0.01);
    }

    SECTION("Несколько фигур")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 5 5 FF0000");
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 5 FF0000 00FF00");
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 10 20 30 40 FF0000 00FF00");
        REQUIRE(shapes.size() == 3);
    }
}

TEST_CASE("ShapeCommandHandler - Ошибки валидации")
{
    std::vector<std::unique_ptr<IShape>> shapes;
    CaptureOutput capture;

    SECTION("Неверное количество аргументов")
    {
        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 5");
        REQUIRE(capture.getOutput().find("Invalid number of arguments") != std::string::npos);
        REQUIRE(capture.getOutput().find("got 3") != std::string::npos);

        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "Triangle 0 0 3 0 0 4");
        REQUIRE(capture.getOutput().find("Invalid number of arguments") != std::string::npos);

        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 10 20 30 40");
        REQUIRE(capture.getOutput().find("Invalid number of arguments") != std::string::npos);

        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 5");
        REQUIRE(capture.getOutput().find("Invalid number of arguments") != std::string::npos);
    }

    SECTION("Неверные координаты")
    {
        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment a b c d FF0000");
        REQUIRE(capture.getOutput().find("coordinates must be numbers") != std::string::npos);

        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "Triangle a b c d e f FF0000 00FF00");
        REQUIRE(capture.getOutput().find("coordinates must be numbers") != std::string::npos);

        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle a b c d FF0000 00FF00");
        REQUIRE(capture.getOutput().find("dimensions must be numbers") != std::string::npos);

        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "Circle a b c FF0000 00FF00");
        REQUIRE(capture.getOutput().find("coordinates and radius must be numbers") != std::string::npos);
    }

    SECTION("Неизвестная команда")
    {
        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "unknown");
        REQUIRE(capture.getOutput().find("Unknown command") != std::string::npos);
    }

    SECTION("Пустая команда")
    {
        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "");
        REQUIRE(capture.getOutput().find("Unknown command") != std::string::npos);
    }

    SECTION("Граничные значения аргументов")
    {
        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment -1 -2 -3 -4 FF0000");
        REQUIRE(shapes.size() == 1);
        REQUIRE(capture.getOutput().empty());

        shapes.clear();
        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 -5 FF0000 00FF00");
        REQUIRE(shapes.size() == 1);
        REQUIRE(shapes[0]->GetArea() == 0.0);

        shapes.clear();
        capture.clear();
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 999999 999999 999999 999999 FFFFFF");
        REQUIRE(shapes.size() == 1);
    }
}

TEST_CASE("ShapeCommandHandler - FindShapeWithMaxArea")
{
    std::vector<std::unique_ptr<IShape>> shapes;

    SECTION("Пустой массив")
    {
        REQUIRE(ShapeCommandHandler::FindShapeWithMaxArea(shapes) == nullptr);
    }

    SECTION("Одна фигура")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 5 FF0000 00FF00");
        const IShape* maxShape = ShapeCommandHandler::FindShapeWithMaxArea(shapes);
        REQUIRE(maxShape != nullptr);
        REQUIRE(std::abs(maxShape->GetArea() - 78.54) < 0.01);
    }

    SECTION("Несколько фигур")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 5 5 FF0000");
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 5 FF0000 00FF00");
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 10 20 30 40 FF0000 00FF00");

        const IShape* maxShape = ShapeCommandHandler::FindShapeWithMaxArea(shapes);
        REQUIRE(maxShape != nullptr);
        REQUIRE(maxShape->GetArea() == 1200.0);
    }

    SECTION("Фигуры с одинаковой площадью")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 0 0 10 10 FF0000 00FF00");
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 0 0 10 10 FF0000 00FF00");

        const IShape* maxShape = ShapeCommandHandler::FindShapeWithMaxArea(shapes);
        REQUIRE(maxShape != nullptr);
        REQUIRE(maxShape->GetArea() == 100.0);
    }
}

TEST_CASE("ShapeCommandHandler - FindShapeWithMinPerimeter")
{
    std::vector<std::unique_ptr<IShape>> shapes;

    SECTION("Пустой массив")
    {
        REQUIRE(ShapeCommandHandler::FindShapeWithMinPerimeter(shapes) == nullptr);
    }

    SECTION("Одна фигура")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 5 5 FF0000");
        const IShape* minShape = ShapeCommandHandler::FindShapeWithMinPerimeter(shapes);
        REQUIRE(minShape != nullptr);
        REQUIRE(std::abs(minShape->GetPerimeter() - 7.07) < 0.01);
    }

    SECTION("Несколько фигур")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 1 0 FF0000");
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 10 FF0000 00FF00");
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 10 20 30 40 FF0000 00FF00");

        const IShape* minShape = ShapeCommandHandler::FindShapeWithMinPerimeter(shapes);
        REQUIRE(minShape != nullptr);
        REQUIRE(minShape->GetPerimeter() == 1.0);
    }

    SECTION("Фигуры с нулевым периметром")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 0 0 FF0000");
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 5 FF0000 00FF00");

        const IShape* minShape = ShapeCommandHandler::FindShapeWithMinPerimeter(shapes);
        REQUIRE(minShape != nullptr);
        REQUIRE(minShape->GetPerimeter() == 0.0);
    }
}

TEST_CASE("Интеграционные сценарии")
{
    std::vector<std::unique_ptr<IShape>> shapes;

    SECTION("Полный цикл работы")
    {
        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 10 0 FF0000");
        ShapeCommandHandler::ProcessCommand(shapes, "Triangle 0 0 6 0 0 8 FF0000 00FF00");
        ShapeCommandHandler::ProcessCommand(shapes, "Rectangle 0 0 5 10 FF0000 00FF00");
        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 7 FF0000 00FF00");

        REQUIRE(shapes.size() == 4);

        const IShape* maxShape = ShapeCommandHandler::FindShapeWithMaxArea(shapes);
        REQUIRE(maxShape != nullptr);
        REQUIRE(std::abs(maxShape->GetArea() - 153.94) < 0.01);

        const IShape* minShape = ShapeCommandHandler::FindShapeWithMinPerimeter(shapes);
        REQUIRE(minShape != nullptr);
        REQUIRE(minShape->GetPerimeter() == 10.0);
    }

    SECTION("Обработка ошибок без прерывания")
    {
        std::vector<std::unique_ptr<IShape>> shapes;

        ShapeCommandHandler::ProcessCommand(shapes, "Circle 0 0 5 FF0000 00FF00");
        REQUIRE(shapes.size() == 1);

        CaptureOutput capture;
        ShapeCommandHandler::ProcessCommand(shapes, "invalid command");
        REQUIRE(capture.getOutput().find("Unknown command") != std::string::npos);
        REQUIRE(shapes.size() == 1);

        ShapeCommandHandler::ProcessCommand(shapes, "LineSegment 0 0 5 5 FF0000");
        REQUIRE(shapes.size() == 2);
    }
}