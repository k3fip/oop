#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <string>
#include "../lab7/sort2.h"

TEST_CASE("Сортировка const char*") {
    SECTION("Уже отсортированные строки") {
        const char* first = "apple";
        const char* second = "banana";

        Sort2(first, second);

        REQUIRE(std::strcmp(first, "apple") == 0);
        REQUIRE(std::strcmp(second, "banana") == 0);
    }

    SECTION("Неотсортированные строки") {
        const char* first = "banana";
        const char* second = "apple";

        Sort2(first, second);

        REQUIRE(std::strcmp(first, "apple") == 0);
        REQUIRE(std::strcmp(second, "banana") == 0);
    }

    SECTION("Одинаковые строки") {
        const char* first = "hello";
        const char* second = "hello";

        const char* original_first = first;
        const char* original_second = second;

        Sort2(first, second);

        REQUIRE(first == original_first);
        REQUIRE(second == original_second);
    }

    SECTION("Пустые строки") {
        const char* first = "";
        const char* second = "apple";

        Sort2(first, second);

        REQUIRE(std::strcmp(first, "") == 0);
        REQUIRE(std::strcmp(second, "apple") == 0);
    }
}

TEST_CASE("Сортировка с учетом регистра") {
    SECTION("Заглавные и строчные буквы") {
        const char* first = "Banana";
        const char* second = "apple";

        Sort2(first, second);

        REQUIRE(std::strcmp(first, "Banana") == 0);
        REQUIRE(std::strcmp(second, "apple") == 0);
    }
}

TEST_CASE("Целостность указателей") {
    SECTION("Указатели меняются местами, содержимое не копируется") {
        const char* str1 = "banana";
        const char* str2 = "apple";

        const char* original_p1 = str1;
        const char* original_p2 = str2;

        Sort2(str1, str2);

        REQUIRE(str1 == original_p2);
        REQUIRE(str2 == original_p1);
        REQUIRE(std::strcmp(str1, "apple") == 0);
        REQUIRE(std::strcmp(str2, "banana") == 0);
    }
}

TEST_CASE("Работа с c_str() из std::string") {
    SECTION("Строки через c_str()") {
        std::string s1 = "world";
        std::string s2 = "hello";

        const char* p1 = s1.c_str();
        const char* p2 = s2.c_str();

        Sort2(p1, p2);

        REQUIRE(std::strcmp(p1, "hello") == 0);
        REQUIRE(std::strcmp(p2, "world") == 0);
        REQUIRE(s1 == "world");
        REQUIRE(s2 == "hello");
    }
}