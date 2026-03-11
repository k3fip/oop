#include <catch2/catch_test_macros.hpp>
#include "../Task2/HTMLDecode.h"

#include <sstream>

TEST_CASE("HTMLDecode: Базовые сущности") {
    SECTION("Декодирование &quot;") {
        std::string input = "&quot;Hello&quot;";
        std::string expected = "\"Hello\"";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Декодирование &apos;") {
        std::string input = "&apos;World&apos;";
        std::string expected = "'World'";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Декодирование &lt;") {
        std::string input = "5 &lt; 10";
        std::string expected = "5 < 10";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Декодирование &gt;") {
        std::string input = "10 &gt; 5";
        std::string expected = "10 > 5";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Декодирование &amp;") {
        std::string input = "AT&amp;T";
        std::string expected = "AT&T";
        REQUIRE(HTMLDecode(input) == expected);
    }
}

TEST_CASE("Несколько сущностей в одной строке") {
    SECTION("Несколько одинаковых сущностей") {
        std::string input = "&quot;A&quot; &quot;B&quot; &quot;C&quot;";
        std::string expected = "\"A\" \"B\" \"C\"";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Несколько разных сущностей") {
        std::string input = "&lt;div&gt;Hello &amp; World&lt;/div&gt;";
        std::string expected = "<div>Hello & World</div>";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Сущности с текстом между ними") {
        std::string input = "Start &lt;middle&gt; End";
        std::string expected = "Start <middle> End";
        REQUIRE(HTMLDecode(input) == expected);
    }
}

TEST_CASE("Вложенные сущности") {
    SECTION("Вложенный &amp;") {
        std::string input = "&amp;quot;";
        std::string expected = "&quot;";
        REQUIRE(HTMLDecode(input) == "&quot;");
    }

    SECTION("Сложное вложение") {
        std::string input = "&amp;lt;tag&amp;gt;";
        std::string expected = "&lt;tag&gt;";
        REQUIRE(HTMLDecode(input) == expected);
    }
}

TEST_CASE("Нет сущностей") {
    SECTION("Пустая строка") {
        std::string input = "";
        std::string expected = "";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Обычный текст без сущностей") {
        std::string input = "Hello World";
        std::string expected = "Hello World";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Специальные символы, не являющиеся сущностями") {
        std::string input = "Hello & World";
        std::string expected = "Hello & World";
        REQUIRE(HTMLDecode(input) == expected);
    }
}

TEST_CASE("Граничные случаи") {
    SECTION("Сущность без закрывающей точки с запятой") {
        std::string input = "&quot";
        std::string expected = "&quot";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Неполная сущность") {
        std::string input = "&am";
        std::string expected = "&am";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Несколько символов &") {
        std::string input = "&&amp;&";
        std::string expected = "&&&";
        REQUIRE(HTMLDecode(input) == expected);
    }

    SECTION("Сущность в начале и конце") {
        std::string input = "&lt;Hello&gt;";
        std::string expected = "<Hello>";
        REQUIRE(HTMLDecode(input) == expected);
    }
}

TEST_CASE("Чтение из строкового потока") {
    SECTION("Одно слово") {
        std::stringstream ss("Hello");
        REQUIRE(GetString(ss) == "Hello");
    }

    SECTION("Несколько слов") {
        std::stringstream ss("Hello World");
        REQUIRE(GetString(ss) == "HelloWorld");
    }

    SECTION("Несколько строк") {
        std::stringstream ss("Hello\nWorld\nTest");
        REQUIRE(GetString(ss) == "HelloWorldTest");
    }

    SECTION("Пустой ввод") {
        std::stringstream ss("");
        REQUIRE(GetString(ss) == "");
    }

    SECTION("Только пробельные символы") {
        std::stringstream ss("   \n   \t   ");
        REQUIRE(GetString(ss) == "");
    }

    SECTION("Смешанное содержимое") {
        std::stringstream ss("Hello &lt;World&gt; 123");
        REQUIRE(GetString(ss) == "Hello&lt;World&gt;123");
    }
}

TEST_CASE("GetString + HTMLDecode") {
    SECTION("Простой конвейер") {
        std::stringstream ss("Hello &lt;World&gt;");
        std::string raw = GetString(ss);
        std::string decoded = HTMLDecode(raw);
        REQUIRE(decoded == "Hello<World>");
    }

    SECTION("Сложный конвейер") {
        std::stringstream ss("AT&amp;T &quot;Hello&quot; &apos;World&apos;");
        std::string raw = GetString(ss);
        std::string decoded = HTMLDecode(raw);
        REQUIRE(decoded == "AT&T\"Hello\"'World'");
    }
}