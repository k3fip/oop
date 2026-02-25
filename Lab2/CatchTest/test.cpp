#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <sstream>
#include <limits>
#include "ProcessNumbers.h"

using Catch::Matchers::WithinRel;

TEST_CASE("CheckOverflow с обычными числами")
{
    REQUIRE_FALSE(CheckOverflow(0.0f));
    REQUIRE_FALSE(CheckOverflow(1.0f));
    REQUIRE_FALSE(CheckOverflow(-1.0f));
    REQUIRE_FALSE(CheckOverflow(3.14f));
    REQUIRE_FALSE(CheckOverflow(1e10f));
    REQUIRE_FALSE(CheckOverflow(1e-10f));
}

TEST_CASE("CheckOverflow с граничными значениями")
{
    SECTION("Бесконечность")
    {
        REQUIRE(CheckOverflow(std::numeric_limits<float>::infinity()));
    }

    SECTION("Минус бесконечность")
    {
        REQUIRE(CheckOverflow(-std::numeric_limits<float>::infinity()));
    }

    SECTION("NaN (не число)")
    {
        REQUIRE(CheckOverflow(std::numeric_limits<float>::quiet_NaN()));
    }

    SECTION("Максимальное значение float")
    {
        REQUIRE_FALSE(CheckOverflow(std::numeric_limits<float>::max()));
    }

    SECTION("Минимальное положительное значение")
    {
        REQUIRE_FALSE(CheckOverflow(std::numeric_limits<float>::min()));
    }
}

TEST_CASE("ReadNumbers читает корректные числа")
{
    std::istringstream input("1.5 2.5 3.7 4.2");
    bool wasError;

    Vector result = ReadNumbers(input, wasError);

    SECTION("Ошибки нет")
    {
        REQUIRE_FALSE(wasError);
    }

    SECTION("Прочитаны все числа")
    {
        REQUIRE(result.size() == 4);
        REQUIRE_THAT(result[0], WithinRel(1.5f, 0.001f));
        REQUIRE_THAT(result[1], WithinRel(2.5f, 0.001f));
        REQUIRE_THAT(result[2], WithinRel(3.7f, 0.001f));
        REQUIRE_THAT(result[3], WithinRel(4.2f, 0.001f));
    }
}

TEST_CASE("ReadNumbers с пустым вводом")
{
    std::istringstream input("");
    bool wasError;

    Vector result = ReadNumbers(input, wasError);

    SECTION("Пустой вектор")
    {
        REQUIRE(result.empty());
    }

}

TEST_CASE("ReadNumbers с нечисловым вводом")
{
    std::istringstream input("1 2 abc 3");
    bool wasError;

    Vector result = ReadNumbers(input, wasError);

    SECTION("Должна быть ошибка")
    {
        REQUIRE(wasError);
    }

}

TEST_CASE("ProcessNumbers с обычными числами")
{
    Vector input = { 1.0f, 2.0f, 3.0f, 4.0f };
    bool wasError;

    Vector result = ProcessNumbers(input, wasError);

    SECTION("Ошибки нет")
    {
        REQUIRE_FALSE(wasError);
    }

    SECTION("Результат умножен на минимум (1.0)")
    {
        REQUIRE(result.size() == 4);
        REQUIRE_THAT(result[0], WithinRel(1.0f));
        REQUIRE_THAT(result[1], WithinRel(2.0f));
        REQUIRE_THAT(result[2], WithinRel(3.0f));
        REQUIRE_THAT(result[3], WithinRel(4.0f));
    }
}

TEST_CASE("ProcessNumbers с отрицательными числами")
{
    Vector input = { 2.0f, -3.0f, 5.0f, -1.0f };
    bool wasError;

    Vector result = ProcessNumbers(input, wasError);

    SECTION("Минимум = -3.0")
    {
        REQUIRE_FALSE(wasError);
        REQUIRE(result.size() == 4);
        REQUIRE_THAT(result[0], WithinRel(-6.0f));
        REQUIRE_THAT(result[1], WithinRel(9.0f));
        REQUIRE_THAT(result[2], WithinRel(-15.0f));
        REQUIRE_THAT(result[3], WithinRel(3.0f));
    }
}

TEST_CASE("ProcessNumbers с пустым вектором")
{
    Vector input;
    bool wasError;

    Vector result = ProcessNumbers(input, wasError);

    SECTION("Результат пустой")
    {
        REQUIRE(result.empty());
    }
}

TEST_CASE("ProcessNumbers с переполнением")
{
    float huge = std::numeric_limits<float>::max() / 2.0f;
    Vector input = { huge, huge };
    bool wasError;

    Vector result = ProcessNumbers(input, wasError);

    SECTION("Должна быть ошибка из-за переполнения")
    {
        REQUIRE(wasError);
    }
}

TEST_CASE("PrintSortedNumbers печатает отсортированные числа")
{
    Vector input = { 3.14159f, 2.71828f, 1.41421f, 1.61803f };
    std::ostringstream output;

    PrintSortedNumbers(input, output);

    std::string expected = "1.414 1.618 2.718 3.142 \n";
    REQUIRE(output.str() == expected);
}

TEST_CASE("PrintSortedNumbers с отрицательными числами")
{
    Vector input = { -5.0f, 10.0f, -1.0f, 0.0f, 3.0f };
    std::ostringstream output;

    PrintSortedNumbers(input, output);

    std::string expected = "-5.000 -1.000 0.000 3.000 10.000 \n";
    REQUIRE(output.str() == expected);
}

TEST_CASE("PrintNumbers с пустым вектором")
{
    Vector input;
    std::ostringstream output;

    PrintSortedNumbers(input, output);

    REQUIRE(output.str() == "\n");
}

TEST_CASE("Полный цикл обработки")
{
    std::istringstream input("3.0 -2.0 5.0 1.0");
    std::ostringstream output;
    bool wasError;

    Vector numbers = ReadNumbers(input, wasError);
    REQUIRE_FALSE(wasError);
    REQUIRE(numbers.size() == 4);

    Vector processed = ProcessNumbers(numbers, wasError);
    REQUIRE_FALSE(wasError);

    REQUIRE(processed.size() == 4);
    REQUIRE_THAT(processed[0], WithinRel(-6.0f));
    REQUIRE_THAT(processed[1], WithinRel(4.0f));
    REQUIRE_THAT(processed[2], WithinRel(-10.0f));
    REQUIRE_THAT(processed[3], WithinRel(-2.0f));

    PrintSortedNumbers(processed, output);
    std::string expected = "-10.000 -6.000 -2.000 4.000 \n";
    REQUIRE(output.str() == expected);
}