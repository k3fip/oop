#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <stdexcept>
#include "../lab5/CTime.h"

TEST_CASE("Конструкторы CTime")
{
    SECTION("Корректные параметры")
    {
        CTime t1(14, 30, 25);
        REQUIRE(t1.GetHours() == 14);
        REQUIRE(t1.GetMinutes() == 30);
        REQUIRE(t1.GetSeconds() == 25);

        CTime t2(14, 30);
        REQUIRE(t2.GetSeconds() == 0);

        CTime t3(3600);
        REQUIRE(t3.GetHours() == 1);
        REQUIRE(t3.GetMinutes() == 0);
        REQUIRE(t3.GetSeconds() == 0);

        CTime t4;
        REQUIRE(t4 == CTime(0, 0, 0));
    }

    SECTION("Граничные значения")
    {
        REQUIRE_NOTHROW(CTime(0, 0, 0));
        REQUIRE_NOTHROW(CTime(23, 59, 59));
        REQUIRE_NOTHROW(CTime(0));
        REQUIRE_NOTHROW(CTime(86399));
    }

    SECTION("Некорректные параметры")
    {
        REQUIRE_THROWS_AS(CTime(24, 0, 0), std::logic_error);
        REQUIRE_THROWS_AS(CTime(0, 60, 0), std::logic_error);
        REQUIRE_THROWS_AS(CTime(0, 0, 60), std::logic_error);
        REQUIRE_THROWS_AS(CTime(86400), std::logic_error);
        REQUIRE_THROWS_AS(CTime(99, 99, 99), std::logic_error);
    }
}

TEST_CASE("Геттеры CTime")
{
    CTime time(12, 34, 56);
    REQUIRE(time.GetHours() == 12);
    REQUIRE(time.GetMinutes() == 34);
    REQUIRE(time.GetSeconds() == 56);

    CTime fromTimestamp(3661);
    REQUIRE(fromTimestamp.GetHours() == 1);
    REQUIRE(fromTimestamp.GetMinutes() == 1);
    REQUIRE(fromTimestamp.GetSeconds() == 1);
}

TEST_CASE("Операторы инкремента и декремента")
{
    SECTION("Инкремент")
    {
        CTime time(0, 0, 0);
        CTime old = time++;
        REQUIRE(old == CTime(0, 0, 0));
        REQUIRE(time == CTime(0, 0, 1));

        ++time;
        REQUIRE(time == CTime(0, 0, 2));
    }

    SECTION("Декремент")
    {
        CTime time(0, 0, 2);
        CTime old = time--;
        REQUIRE(old == CTime(0, 0, 2));
        REQUIRE(time == CTime(0, 0, 1));

        --time;
        REQUIRE(time == CTime(0, 0, 0));
    }

    SECTION("Цикличность")
    {
        CTime time(23, 59, 59);
        ++time;
        REQUIRE(time == CTime(0, 0, 0));

        --time;
        REQUIRE(time == CTime(23, 59, 59));
    }
}

TEST_CASE("Арифметические операции")
{
    SECTION("Сложение и вычитание")
    {
        CTime t1(14, 30, 25);
        CTime t2(3, 18, 44);
        REQUIRE(t1 + t2 == CTime(17, 49, 9));
        REQUIRE(t1 - t2 == CTime(11, 11, 41));
    }

    SECTION("Переполнение при сложении")
    {
        REQUIRE(CTime(23, 59, 59) + CTime(0, 0, 3) == CTime(0, 0, 2));
        REQUIRE(CTime(23, 59, 59) + CTime(0, 0, 1) == CTime(0, 0, 0));
    }

    SECTION("Антипереполнение при вычитании")
    {
        REQUIRE(CTime(0, 0, 5) - CTime(0, 0, 10) == CTime(23, 59, 55));
        REQUIRE(CTime(0, 0, 0) - CTime(0, 0, 1) == CTime(23, 59, 59));
    }

    SECTION("Составные операторы")
    {
        CTime time(1, 0, 0);
        time += CTime(2, 0, 0);
        REQUIRE(time == CTime(3, 0, 0));

        time -= CTime(1, 30, 0);
        REQUIRE(time == CTime(1, 30, 0));

        time += CTime(23, 0, 0);
        REQUIRE(time == CTime(0, 30, 0));
    }
}

TEST_CASE("Умножение и деление")
{
    SECTION("Умножение")
    {
        CTime time(3, 5, 15);
        REQUIRE(time * 3 == CTime(9, 15, 45));
        REQUIRE(3 * time == CTime(9, 15, 45));
        REQUIRE(time * 0 == CTime(0, 0, 0));
        REQUIRE(time * 1 == time);
    }

    SECTION("Умножение с переполнением")
    {
        REQUIRE(CTime(12, 0, 0) * 2 == CTime(0, 0, 0));
        REQUIRE(CTime(1, 0, 0) * 24 == CTime(0, 0, 0));
    }

    SECTION("Деление на число")
    {
        REQUIRE(CTime(9, 15, 45) / 3 == CTime(3, 5, 15));
        REQUIRE(CTime(9, 15, 40) / 3 == CTime(3, 5, 13));
    }

    SECTION("Деление времени на время")
    {
        REQUIRE(CTime(9, 15, 40) / CTime(3, 5, 13) == 3);
    }

    SECTION("Деление на ноль")
    {
        REQUIRE((CTime(10, 0, 0) / CTime(0, 0, 0)) == 86399);
    }

    SECTION("Составные операторы")
    {
        CTime time(3, 5, 15);
        time *= 3;
        REQUIRE(time == CTime(9, 15, 45));
        time /= 3;
        REQUIRE(time == CTime(3, 5, 15));
    }
}

TEST_CASE("Операторы сравнения")
{
    CTime midnight(0, 0, 0);
    CTime noon(12, 0, 0);
    CTime endOfDay(23, 59, 59);

    REQUIRE(midnight == CTime(0, 0, 0));
    REQUIRE(midnight != noon);
    REQUIRE(midnight < noon);
    REQUIRE(noon > midnight);
    REQUIRE(midnight <= midnight);
    REQUIRE(noon >= midnight);
    REQUIRE(midnight <= noon);
    REQUIRE(noon >= noon);
    REQUIRE(midnight < endOfDay);
    REQUIRE_FALSE(midnight > noon);
    REQUIRE_FALSE(noon < midnight);
}

TEST_CASE("Операторы ввода-вывода")
{
    SECTION("Вывод")
    {
        std::ostringstream oss;
        oss << CTime(0, 0, 0);
        REQUIRE(oss.str() == "00:00:00");

        oss.str("");
        oss << CTime(14, 30, 25);
        REQUIRE(oss.str() == "14:30:25");

        oss.str("");
        oss << CTime(23, 59, 59);
        REQUIRE(oss.str() == "23:59:59");
    }

    SECTION("Ввод в формате ЧЧ:ММ:СС")
    {
        std::istringstream iss("14:30:25");
        CTime time;
        iss >> time;
        REQUIRE(time == CTime(14, 30, 25));
    }

    SECTION("Ввод временной метки")
    {
        std::istringstream iss("3661");
        CTime time;
        iss >> time;
        REQUIRE(time == CTime(1, 1, 1));
    }

    SECTION("Некорректный ввод")
    {
        std::istringstream iss("24:00:00");
        CTime time;
        iss >> time;
        REQUIRE(iss.fail());

        std::istringstream iss2("12:60:00");
        iss2 >> time;
        REQUIRE(iss2.fail());

        std::istringstream iss3("86400");
        iss3 >> time;
        REQUIRE(iss3.fail());

        std::istringstream iss4("invalid");
        iss4 >> time;
        REQUIRE(iss4.fail());
    }

    SECTION("Граничные значения ввода")
    {
        std::istringstream iss("0");
        CTime time;
        iss >> time;
        REQUIRE(time == CTime(0, 0, 0));

        std::istringstream iss2("86399");
        iss2 >> time;
        REQUIRE(time == CTime(23, 59, 59));
    }
}

TEST_CASE("Комплексные операции")
{
    CTime t1(23, 59, 59);
    CTime t2(0, 0, 2);

    CTime result = t1 + t2;
    REQUIRE(result == CTime(0, 0, 1));
    REQUIRE(result - t2 == CTime(23, 59, 59));

    CTime time(3, 0, 0);
    REQUIRE((time * 3) / 2 == CTime(4, 30, 0));
}

TEST_CASE("Полный цикл")
{
    CTime time(0, 0, 0);
    for (int i = 0; i < 86400; ++i) ++time;
    REQUIRE(time == CTime(0, 0, 0));

    for (int i = 0; i < 86400; ++i) --time;
    REQUIRE(time == CTime(0, 0, 0));
}