#include "ProcessNumbers.h"
#include <cfenv>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <iostream>

bool CheckOverflow(float value)
{
    int fp_exception = std::fetestexcept(FE_OVERFLOW | FE_UNDERFLOW | FE_INVALID);
    std::feclearexcept(FE_ALL_EXCEPT);

    return (fp_exception & (FE_OVERFLOW | FE_UNDERFLOW | FE_INVALID)) ||
        std::isinf(value) || std::isnan(value);
}

Vector ReadNumbers(std::istream& input, bool& wasError)
{
    wasError = false;
    Vector vector;
    float val;

    while (input >> val)
    {
        if (CheckOverflow(val))
        {
            wasError = true;
            return vector;
        }

        vector.push_back(val);
    }

    if (!input.eof())
    {
        wasError = true;
        return vector;
    }

    return vector;
}

Vector ProcessNumbers(const Vector& vector, bool& wasError)
{
    wasError = false;

    float minimum = vector.empty() ? 0 : *std::min_element(vector.begin(), vector.end());

    Vector result;
    result.reserve(vector.size());

    for (float val : vector)
    {
        float new_val = val * minimum;
        if (CheckOverflow(new_val))
        {
            wasError = true;
            return result;
        }
        result.push_back(new_val);
    }

    return result;
}

void PrintSortedNumbers(const Vector& vector, std::ostream& output)
{
    Vector sorted = vector;
    std::sort(sorted.begin(), sorted.end());

    for (const auto elem : sorted)
    {
        output << std::fixed << std::setprecision(3) << elem << " ";
    }
    output << "\n";
}