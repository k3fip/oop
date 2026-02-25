//Вариант №1 – live – 150 баллов

#include <iostream>
#include <string>
#include <array>
#include <fstream>

const int MAX_SIZE = 256;

struct Field
{
    std::array<std::array<char, MAX_SIZE>, MAX_SIZE> data = { {'0'} };
    int rows = 0;
    int cols = 0;
};

void PrintField(Field& field, std::string fileName = "")
{
    if (fileName != "")
    {
        std::ofstream file(fileName);

        for (int i = 0; i < field.rows; i++)
        {
            for (int j = 0; j < field.cols; j++)
            {
                file << field.data[i][j];
            }
            file << "\n";
        }
    }
    else
    {
        for (int i = 0; i < field.rows; i++)
        {
            for (int j = 0; j < field.cols; j++)
            {
                std::cout << field.data[i][j];
            }
            std::cout << "\n";
        }
    }
    
}

bool GetField(std::string fileName, Field& field)
{
    std::string line;

    std::ifstream file;
    file.open(fileName);

    if (!file.is_open())
    {
        return true;
    }

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        return true;
    }

    int currRow = 0;
    int expectedCols = 0;

    while (currRow < MAX_SIZE && std::getline(file, line))
    {
        if (line.empty() && file.eof())
        {
            break;
        }

        int currCol = 0;

        if (line.size() > MAX_SIZE)
        {
            return true;
        }

        if (line[0] != '*' && line[line.size() - 1] != '*')
        {
            return true;
        }

        for (char val : line)
        {
            if (val != '#' && val != '*' && val != ' ')
            {
                return true;
            }

            field.data[currRow][currCol] = val;
            currCol++;

        }
        

        if (currCol > 0)
        {
            if (expectedCols == 0)
            {
                expectedCols = currCol;
            }

            if (expectedCols != currCol)
            {
                return true;
            }

        }

        currRow++;
    }

    if (currRow > MAX_SIZE)
    {
        return true;
    }

    field.rows = currRow;
    field.cols = expectedCols;

    return false;
}

int LiveSections(Field& field, int row, int col)
{
    int result = 0;
    for (int i = row - 1; i < row + 2; i++)
    {
        for (int j = col - 1; j < col + 2; j++)
        {
            if ((i != row || j != col) && field.data[i][j] == '#')
            {
                result++;
            }
        }
    }
    return result;
}

void Live(Field& startField, Field& endField)
{
    endField = startField;
    int liveSections;
    char section;
    for (int i = 1; i < startField.rows - 1; i++)
    {
        for (int j = 1; j < startField.cols - 1; j++)
        {
            section = startField.data[i][j];

            liveSections = LiveSections(startField, i, j);

            if (section == '#' && (liveSections >= 4 || liveSections <= 1))
            {
                endField.data[i][j] = ' ';
            }
            else if (section == ' ' && liveSections == 3)
            {
                endField.data[i][j] = '#';
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc > 3 || argc < 2)
    {
        std::cout << "Incorrect input\n";
        return 0;
    }

    Field startField, endField;
    
    bool wasError = GetField(argv[1], startField);
    if (wasError)
    {
        std::cout << "Incorrect data format\n";
        return 0;
    }

    Live(startField, endField);

    if (argc == 3)
    {
        PrintField(endField, argv[2]);
        return 0;
    }

    PrintField(endField);

    return 0;
}


