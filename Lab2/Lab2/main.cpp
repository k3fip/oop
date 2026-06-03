#include "ProcessNumbers.h"
#include <iostream>

const char* ERROR = "ERROR\n";

int main()
{
    bool wasError;

    Vector vector = ReadNumbers(std::cin, wasError);
    if (wasError)
    {
        std::cout << ERROR;
        return 0;
    }

    Vector processedVector = ProcessNumbers(vector, wasError);
    if (wasError)
    {
        std::cout << ERROR;
        return 0;
    }

    PrintSortedNumbers(processedVector, std::cout);
    return 0;
}