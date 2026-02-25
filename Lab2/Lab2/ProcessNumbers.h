#pragma once
#include <vector>
#include <iostream>

using Vector = std::vector<float>;

bool CheckOverflow(float value);
Vector ReadNumbers(std::istream& input, bool& wasError);
Vector ProcessNumbers(const Vector& vector, bool& wasError);
void PrintSortedNumbers(const Vector& vector, std::ostream& output);