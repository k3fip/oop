//Вариант №2 – compare 
#include <iostream>
#include <fstream>
#include <string>

const int EXPECTED_ARGS_COUNT = 3;

const char* ERROR_INVALID_ARGS = "Invalid number of arguments\n";
const char* ERROR_INCORRECT_INPUT = "Incorrect input\n";
const char* MESSAGE_FILES_EQUAL = "Files are equal\n";
const char* MESSAGE_FILES_DIFFERENT = "Files are different. Line number is ";

const int START_LINE_NUMBER = 1;

int main(int argc, char* argv[])
{
    if (argc != EXPECTED_ARGS_COUNT)
    {
        std::cout << ERROR_INVALID_ARGS;
        return 0;
    }

    std::ifstream file1;
    std::ifstream file2;

    file1.open(argv[1]);
    file2.open(argv[2]);

    bool equal = true;
    if (!(file1.is_open() && file2.is_open()))
    {
        std::cout << ERROR_INCORRECT_INPUT;
        return 0;
    }

    std::string str1;
    std::string str2;

    std::getline(file1, str1);
    std::getline(file2, str2);

    int strCounter = START_LINE_NUMBER;

    while (equal)
    {
        if (file1.eof() && file2.eof())
        {
            break;
        }
        if ((str1 != str2) || ((!file1.eof() && file2.eof()) || (file1.eof() && !file2.eof())))
        {
            equal = false;
            break;
        }
        else
        {
            strCounter += START_LINE_NUMBER;
            std::getline(file1, str1);
            std::getline(file2, str2);
        }

    }

    if (equal)
    {
        std::cout << MESSAGE_FILES_EQUAL;
    }
    else
    {
        std::cout << MESSAGE_FILES_DIFFERENT << strCounter << "\n";
    }

    return 0;
}