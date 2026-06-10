#include <catch2/catch_test_macros.hpp>
#include <string>
#include <array>
#include <algorithm>
#include "../lab6/SortStrings3.h"

TEST_CASE("Сортировка трёх строк в порядке возрастания") {
    SECTION("Уже отсортированные") {
        std::string a = "apple", b = "banana", c = "cherry";
        SortStrings3(a, b, c);
        REQUIRE(a == "apple");
        REQUIRE(b == "banana");
        REQUIRE(c == "cherry");
    }

    SECTION("Полностью обратный порядок") {
        std::string a = "cherry", b = "banana", c = "apple";
        SortStrings3(a, b, c);
        REQUIRE(a == "apple");
        REQUIRE(b == "banana");
        REQUIRE(c == "cherry");
    }

    SECTION("Частично неотсортированные") {
        std::string test_cases[][3] = {
            {"banana", "apple", "cherry"},
            {"cherry", "apple", "banana"},
            {"banana", "cherry", "apple"},
            {"apple", "cherry", "banana"},
            {"cherry", "banana", "apple"}
        };

        for (auto& [a, b, c] : test_cases) {
            std::string s1 = a, s2 = b, s3 = c;
            SortStrings3(s1, s2, s3);
            REQUIRE(s1 == "apple");
            REQUIRE(s2 == "banana");
            REQUIRE(s3 == "cherry");
        }
    }
}

TEST_CASE("Сортировка с одинаковыми строками") {
    SECTION("Все строки одинаковы") {
        std::string a = "same", b = "same", c = "same";
        SortStrings3(a, b, c);
        REQUIRE(a == "same");
        REQUIRE(b == "same");
        REQUIRE(c == "same");
    }

    SECTION("Две одинаковые и одна другая") {
        struct TestCase { std::string a, b, c; std::string expected[3]; } cases[] = {
            {"same", "same", "aaa", {"aaa", "same", "same"}},
            {"same", "aaa", "same", {"aaa", "same", "same"}},
            {"aaa", "same", "same", {"aaa", "same", "same"}},
            {"same", "same", "zzz", {"same", "same", "zzz"}},
            {"same", "zzz", "same", {"same", "same", "zzz"}},
            {"zzz", "same", "same", {"same", "same", "zzz"}}
        };

        for (auto& tc : cases) {
            std::string a = tc.a, b = tc.b, c = tc.c;
            SortStrings3(a, b, c);
            REQUIRE(a == tc.expected[0]);
            REQUIRE(b == tc.expected[1]);
            REQUIRE(c == tc.expected[2]);
        }
    }
}

TEST_CASE("Сортировка с пустыми строками") {
    struct TestCase { std::string a, b, c; std::string expected[3]; } cases[] = {
        {"", "", "", {"", "", ""}},
        {"", "cherry", "apple", {"", "apple", "cherry"}},
        {"cherry", "", "apple", {"", "apple", "cherry"}},
        {"cherry", "apple", "", {"", "apple", "cherry"}},
        {"", "", "q", {"", "", "q"}},
        {"", "q", "", {"", "", "q"}},
        {"q", "", "", {"", "", "q"}}
    };

    for (auto& tc : cases) {
        std::string a = tc.a, b = tc.b, c = tc.c;
        SortStrings3(a, b, c);
        REQUIRE(a == tc.expected[0]);
        REQUIRE(b == tc.expected[1]);
        REQUIRE(c == tc.expected[2]);
    }
}

TEST_CASE("Сортировка строк с различными символами") {
    SECTION("Строки с пробелами") {
        std::string a = "hello world", b = "hello", c = "hello world!";
        SortStrings3(a, b, c);
        REQUIRE(a == "hello");
        REQUIRE(b == "hello world");
        REQUIRE(c == "hello world!");
    }

    SECTION("Строки с цифрами и спецсимволами") {
        std::string a = "123", b = "45", c = "6";
        SortStrings3(a, b, c);
        REQUIRE(a == "123");
        REQUIRE(b == "45");
        REQUIRE(c == "6");
    }
}

TEST_CASE("Сортировка с учётом регистра") {
    SECTION("Разные регистры") {
        std::string a = "Apple", b = "banana", c = "CHERRY";
        SortStrings3(a, b, c);
        REQUIRE(a == "Apple");
        REQUIRE(b == "CHERRY");
        REQUIRE(c == "banana");
    }

    SECTION("Одинаковые слова в разных регистрах") {
        std::string a = "hello", b = "Hello", c = "HELLO";
        SortStrings3(a, b, c);
        REQUIRE(a == "HELLO");
        REQUIRE(b == "Hello");
        REQUIRE(c == "hello");
    }
}