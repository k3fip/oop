#include <iostream>
#include <string>
#include <chrono>
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
    
    // Таймер для проверки

    /*auto start = std::chrono::high_resolution_clock::now();

    auto primes = GeneratePrimeNumbersSet(100000000);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Время выполнения: " << duration.count() << " мс\n";*/

    auto primes = GeneratePrimeNumbersSet(upperBound);

    PrintSet(primes, std::cout);
}