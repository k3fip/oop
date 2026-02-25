#include <iostream>
#include <vector>
#include <cfenv>
#include <algorithm>

using Vector = std::vector<float>;

bool CheckOverflow(float value)
{
	int fp_exception = std::fetestexcept(FE_OVERFLOW | FE_UNDERFLOW | FE_INVALID);
	std::feclearexcept(FE_ALL_EXCEPT);

	return (fp_exception & (FE_OVERFLOW | FE_UNDERFLOW | FE_INVALID)) ||
		std::isinf(value) || std::isnan(value);
}

Vector ReadNumbers(bool& wasError)
{
	Vector vector;
	float val;
	while (!std::cin.eof())
	{
		std::cin >> val;
		if (std::cin.fail() || CheckOverflow(val))
		{
			wasError = true;
			return vector;
		}
		vector.push_back(val);
	}

	return vector;
}

Vector ProcessNumbers(Vector vector)
{
	auto minimum = std::min_element(vector.begin(), vector.end());
	
	for (auto iter { vector.begin() }; iter != vector.end(); ++iter)
	{
		float val = *iter * *minimum;
		*iter = val;
	}
}

void PrintSortedNumbers(Vector vector)
{

}

int main()
{

}