#include <vector>
#include <iostream>
#include <set>
#include "PrimeNumbers.h"

std::set<int> GeneratePrimeNumbersSet(int upperBound)
{
    if (upperBound < 2) return {};

    std::vector<bool> primes(upperBound + 1, true);

    for (int curr = 2; curr * curr <= upperBound; curr++)
    {
        if (primes[curr])
        {
            for (int div = curr * curr; div <= upperBound; div += curr) { primes[div] = false; }
        }
    }

    std::set<int> result{};

    for (int i = 2; i <= upperBound; i++)
    {
        if (primes[i]) result.insert(i);
    }

    return result;
}

void PrintSet(std::set<int>& primes, std::ostream& output)
{
    for (auto num : primes)
    {
        output << num << "\n";
    }
}
