#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <set>
#include <sstream>
#include <chrono>
#include "../Task4/PrimeNumbers.h"

TEST_CASE("Пустой результат для upperBound < 2")
{
    REQUIRE(GeneratePrimeNumbersSet(-10).empty());
    REQUIRE(GeneratePrimeNumbersSet(0).empty());
    REQUIRE(GeneratePrimeNumbersSet(1).empty());
}

TEST_CASE("Верхняя граница 2")
{
    std::set<int> expected = { 2 };
    REQUIRE(GeneratePrimeNumbersSet(2) == expected);
}

TEST_CASE("Простые числа до 10")
{
    std::set<int> expected = { 2, 3, 5, 7 };
    REQUIRE(GeneratePrimeNumbersSet(10) == expected);
}

TEST_CASE("Простые числа до 20")
{
    std::set<int> expected = { 2, 3, 5, 7, 11, 13, 17, 19 };
    REQUIRE(GeneratePrimeNumbersSet(20) == expected);
}

TEST_CASE("Простые числа до 30")
{
    std::set<int> expected = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
    REQUIRE(GeneratePrimeNumbersSet(30) == expected);
}

TEST_CASE("Верхняя граница — простое число")
{
    std::set<int> expected = { 2, 3, 5, 7, 11, 13 };
    REQUIRE(GeneratePrimeNumbersSet(13) == expected);
}

TEST_CASE("Количество простых чисел до 1000")
{
    REQUIRE(GeneratePrimeNumbersSet(1000).size() == 168);
}

TEST_CASE("Количество простых чисел до 100000")
{
    REQUIRE(GeneratePrimeNumbersSet(100000).size() == 9592);
}

TEST_CASE("MAX_UPPER_BOUND не вызывает исключений")
{
    REQUIRE_NOTHROW(GeneratePrimeNumbersSet(MAX_UPPER_BOUND));
}

TEST_CASE("Печать пустого множества")
{
    std::set<int> emptySet;
    std::ostringstream oss;
    PrintSet(emptySet, oss);
    REQUIRE(oss.str().empty());
}

TEST_CASE("Печать множества с одним элементом")
{
    std::set<int> singleSet = { 42 };
    std::ostringstream oss;
    PrintSet(singleSet, oss);
    REQUIRE(oss.str() == "42\n");
}

TEST_CASE("Печать множества с несколькими элементами")
{
    std::set<int> testSet = { 2, 3, 5, 7, 11 };
    std::ostringstream oss;
    PrintSet(testSet, oss);
    REQUIRE(oss.str() == "2\n3\n5\n7\n11\n");
}

TEST_CASE("Печать сохраняет порядок сортировки")
{
    std::set<int> testSet = { 11, 2, 7, 3, 5 };
    std::ostringstream oss;
    PrintSet(testSet, oss);
    REQUIRE(oss.str() == "2\n3\n5\n7\n11\n");
}

TEST_CASE("Печать результата GeneratePrimeNumbersSet")
{
    auto primes = GeneratePrimeNumbersSet(10);
    std::ostringstream oss;
    PrintSet(primes, oss);
    REQUIRE(oss.str() == "2\n3\n5\n7\n");
}

TEST_CASE("Полный цикл для 20")
{
    auto primes = GeneratePrimeNumbersSet(20);
    std::ostringstream oss;
    PrintSet(primes, oss);

    std::set<int> expectedSet = { 2, 3, 5, 7, 11, 13, 17, 19 };
    REQUIRE(primes == expectedSet);
    REQUIRE(oss.str() == "2\n3\n5\n7\n11\n13\n17\n19\n");
}

TEST_CASE("Производительность для MAX_UPPER_BOUND")
{
    auto start = std::chrono::high_resolution_clock::now();
    auto primes = GeneratePrimeNumbersSet(MAX_UPPER_BOUND);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    REQUIRE(duration.count() < 10000);
    REQUIRE(primes.size() == 5761455);
}