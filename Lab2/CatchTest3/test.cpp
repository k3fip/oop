#include <catch2/catch_test_macros.hpp>
#include "../Task3/Dictionary.h"
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using Dictionary = std::map<std::string, std::vector<std::string>>;

TEST_CASE("Trim удаление пробельных символов")
{
    SECTION("Строка без пробелов")
    {
        std::string input = "hello";
        REQUIRE(Trim(input) == "hello");
    }

    SECTION("Пробелы в начале")
    {
        std::string input = "   hello";
        REQUIRE(Trim(input) == "hello");
    }

    SECTION("Пробелы в конце")
    {
        std::string input = "hello   ";
        REQUIRE(Trim(input) == "hello");
    }

    SECTION("Пробелы с обеих сторон")
    {
        std::string input = "   hello   ";
        REQUIRE(Trim(input) == "hello");
    }

    SECTION("Разные пробельные символы")
    {
        std::string input = " \t\n\rhello \t\n\r";
        REQUIRE(Trim(input) == "hello");
    }

    SECTION("Пустая строка")
    {
        std::string input = "";
        REQUIRE(Trim(input) == "");
    }

    SECTION("Строка только из пробелов")
    {
        std::string input = "   \t\n\r   ";
        REQUIRE(Trim(input) == "");
    }
}

TEST_CASE("FetchDictionary чтение словаря из файла")
{
    SECTION("Чтение пустого файла")
    {
        std::string testFile = "test_dict_empty.txt";
        std::ofstream(testFile).close();

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.empty());

        std::remove(testFile.c_str());
    }

    SECTION("Чтение файла с одной записью")
    {
        std::string testFile = "test_dict_single.txt";
        std::ofstream out(testFile);
        out << "cat: кошка\n";
        out.close();

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.size() == 1);
        REQUIRE(dict.find("cat") != dict.end());
        REQUIRE(dict["cat"].size() == 1);
        REQUIRE(dict["cat"][0] == "кошка");

        std::remove(testFile.c_str());
    }

    SECTION("Чтение файла с несколькими переводами")
    {
        std::string testFile = "test_dict_multiple.txt";
        std::ofstream out(testFile);
        out << "cat: кошка, кот\n";
        out << "dog: собака, пес\n";
        out.close();

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.size() == 2);

        REQUIRE(dict["cat"].size() == 2);
        REQUIRE(dict["cat"][0] == "кошка");
        REQUIRE(dict["cat"][1] == "кот");

        REQUIRE(dict["dog"].size() == 2);
        REQUIRE(dict["dog"][0] == "собака");
        REQUIRE(dict["dog"][1] == "пес");

        std::remove(testFile.c_str());
    }

    SECTION("Пропуск пустых строк и строк без разделителя")
    {
        std::string testFile = "test_dict_skip.txt";
        std::ofstream out(testFile);
        out << "\n";
        out << "cat: кошка\n";
        out << "   \n";
        out << "invalid line\n";
        out << "dog: собака\n";
        out.close();

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.size() == 2);
        REQUIRE(dict.find("cat") != dict.end());
        REQUIRE(dict.find("dog") != dict.end());

        std::remove(testFile.c_str());
    }

    SECTION("Обработка пробелов в ключах и значениях")
    {
        std::string testFile = "test_dict_spaces.txt";
        std::ofstream out(testFile);
        out << "  cat  :  кошка  ,  кот  \n";
        out.close();

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.size() == 1);
        REQUIRE(dict.find("cat") != dict.end());
        REQUIRE(dict["cat"][0] == "кошка");
        REQUIRE(dict["cat"][1] == "кот");

        std::remove(testFile.c_str());
    }

    SECTION("Файл не существует")
    {
        Dictionary dict = FetchDictionary("nonexistent_file.txt");
        REQUIRE(dict.empty());
    }
}

TEST_CASE("SaveDictionary сохранение словаря в файл")
{
    SECTION("Сохранение пустого словаря")
    {
        std::string testFile = "test_save_empty.txt";
        Dictionary dict;

        bool result = SaveDictionary(testFile, dict);
        REQUIRE(result == true);

        std::ifstream in(testFile);
        REQUIRE(in.good());
        in.close();

        std::remove(testFile.c_str());
    }

    SECTION("Сохранение словаря с одной записью")
    {
        std::string testFile = "test_save_single.txt";
        Dictionary dict;
        dict["cat"] = { "кошка" };

        bool result = SaveDictionary(testFile, dict);
        REQUIRE(result == true);

        Dictionary loaded = FetchDictionary(testFile);
        REQUIRE(loaded.size() == 1);
        REQUIRE(loaded["cat"][0] == "кошка");

        std::remove(testFile.c_str());
    }

    SECTION("Сохранение словаря с несколькими переводами")
    {
        std::string testFile = "test_save_multiple.txt";
        Dictionary dict;
        dict["cat"] = { "кошка", "кот" };
        dict["dog"] = { "собака" };

        bool result = SaveDictionary(testFile, dict);
        REQUIRE(result == true);

        Dictionary loaded = FetchDictionary(testFile);
        REQUIRE(loaded.size() == 2);
        REQUIRE(loaded["cat"].size() == 2);
        REQUIRE(loaded["cat"][0] == "кошка");
        REQUIRE(loaded["cat"][1] == "кот");
        REQUIRE(loaded["dog"][0] == "собака");

        std::remove(testFile.c_str());
    }

    SECTION("Невозможно открыть файл для записи")
    {
        Dictionary dict;
        dict["test"] = { "value" };

        bool result = SaveDictionary("Z:\\invalid\\path\\file.txt", dict);
        REQUIRE(result == false);
    }
}

TEST_CASE("PrintTranslation форматирование вывода переводов")
{
    SECTION("Один перевод")
    {
        std::string word = "cat";
        std::vector<std::string> translations = { "кошка" };

        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        PrintTranslation(word, translations);

        std::cout.rdbuf(old);
        REQUIRE(buffer.str() == "cat -> кошка\n");
    }

    SECTION("Несколько переводов")
    {
        std::string word = "cat";
        std::vector<std::string> translations = { "кошка", "кот" };

        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        PrintTranslation(word, translations);

        std::cout.rdbuf(old);
        REQUIRE(buffer.str() == "cat -> кошка, кот\n");
    }
}

TEST_CASE("ProcessDictionary интеграционное тестирование")
{
    SECTION("Поиск существующего слова")
    {
        std::string testFile = "test_process_existing.txt";
        {
            std::ofstream out(testFile);
            out << "cat: кошка\n";
        }

        std::stringstream input("cat\n...\n");
        std::stringstream output;
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        ProcessDictionary(testFile);

        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);

        std::string outputStr = output.str();
        REQUIRE(outputStr.find("cat -> кошка") != std::string::npos);
        REQUIRE(outputStr.find("Goodbye") != std::string::npos);

        std::remove(testFile.c_str());
    }

    SECTION("Добавление нового слова без сохранения")
    {
        std::string testFile = "test_process_add_nosave.txt";
        std::ofstream(testFile).close();

        std::stringstream input("dog\nсобака\n...\nn\n");
        std::stringstream output;
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        ProcessDictionary(testFile);

        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.empty());

        std::remove(testFile.c_str());
    }

    SECTION("Добавление нового слова с сохранением")
    {
        std::string testFile = "test_process_add_save.txt";
        std::ofstream(testFile).close();

        std::stringstream input("dog\nсобака\n...\nY\n");
        std::stringstream output;
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        ProcessDictionary(testFile);

        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.size() == 1);
        REQUIRE(dict["dog"][0] == "собака");

        std::remove(testFile.c_str());
    }

    SECTION("Пустая строка игнорируется")
    {
        std::string testFile = "test_process_empty.txt";
        std::ofstream(testFile).close();

        std::stringstream input("\n...\n");
        std::stringstream output;
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        ProcessDictionary(testFile);

        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.empty());

        std::remove(testFile.c_str());
    }

    SECTION("Пропуск добавления нового слова при пустом переводе")
    {
        std::string testFile = "test_process_skip.txt";
        std::ofstream(testFile).close();

        std::stringstream input("dog\n\n...\nn\n");
        std::stringstream output;
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        ProcessDictionary(testFile);

        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.empty());

        std::remove(testFile.c_str());
    }

    SECTION("Команда выхода без изменений")
    {
        std::string testFile = "test_process_exit.txt";
        std::ofstream out(testFile);
        out << "cat: кошка\n";
        out.close();

        std::stringstream input("...\n");
        std::stringstream output;
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        ProcessDictionary(testFile);

        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);

        Dictionary dict = FetchDictionary(testFile);
        REQUIRE(dict.size() == 1);
        REQUIRE(dict["cat"][0] == "кошка");

        std::remove(testFile.c_str());
    }
}