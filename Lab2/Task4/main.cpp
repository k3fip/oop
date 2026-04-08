#include <iostream>
#include <string>
#include "PrimeNumbers.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " < upper_bound > \n";
		return 0;
	}

    int upperBound;
    try
    {
        upperBound = std::stoi(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cout << "Invalid number format. Please provide an integer.\n";
        return 0;
    }

    if (upperBound < 0 || upperBound > MAX_UPPER_BOUND)
    {
        std::cout << "Invalid number.\n";
    }

    std::set<int> primes = GeneratePrimeNumbersSet(upperBound);
    PrintSet(primes, std::cout);

}