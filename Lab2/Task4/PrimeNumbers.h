#pragma once
#include <vector>
#include <set>
#include <iostream>

int const MAX_UPPER_BOUND = 100000000;

std::set<int> GeneratePrimeNumbersSet(int upperBound);
void PrintSet(std::set<int>& primes, std::ostream& output);
	