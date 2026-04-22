#include <vector>
#include <iostream>
#include <set>
#include "PrimeNumbers.h"

std::set<int> GeneratePrimeNumbersSet(int upperBound)
{
    if (upperBound < 2) return {};

    std::vector<bool> primes(upperBound + 1, true);

    for (int curr = 3; curr * curr <= upperBound; curr += 2)
    {
        if (primes[curr])
        {
            for (int div = curr * curr; div <= upperBound; div += curr) primes[div] = false;
        }
    }

    std::vector<int> result{};

    if (upperBound >= 2) result.push_back(2);

    for (int i = 3; i <= upperBound; i += 2)
    {
        if (primes[i]) result.push_back(i);
    }

    return std::set<int>(result.begin(), result.end());
}

void PrintSet(std::set<int>& primes, std::ostream& output)
{
    for (auto num : primes)
    {
        output << num << "\n";
    }
}
