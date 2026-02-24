//Вариант №6 – radix – 100 баллов

#include <limits.h>
#include <iostream>
#include <string>

int CharToInt(char ch, int radix, bool& wasError)
{
	int val = 0;
	if (ch >= '0' && ch <= '9')
	{
		val = ch - '0';
		if (val >= radix)
		{
			wasError = true;
			return 0;
		}
	}
	else if (ch >= 'A' && ch <= 'Z')
	{
		val = ch - 'A' + 10;
		if (val >= radix)
		{
			wasError = true;
			return 0;
		}
	}
	else
	{
		wasError = true;
		return 0;
	}

	return val;
}

char IntToChar(int digit)
{
	char val = '0';
	if (digit >= 0 && digit <= 9)
	{
		val = digit + '0';
	}
	else
	{
		val = 'A' + (digit - 10);
	}
	return val;
}

int StringToInt(const std::string& str, int radix, bool& wasError)
{
	wasError = false;

	if (radix < 2 || radix > 36 || str.empty())
	{
		wasError = true;
		return 0;
	}

	bool isNegative = false;
	int startPos = 0;
	
	if (str[0] == '-')
	{
		if (str.length() > 1)
		{
			isNegative = true;
			startPos = 1;
		}
		else
		{
			wasError = true;
			return 0;
		}
	}

	long long result = 0;

	for (int i = startPos; i < str.length(); i++)
	{
		int val = 0;
		
		val = CharToInt(str[i], radix, wasError);
		
		if (wasError)
		{
			return 0;
		}

		if (result > (LLONG_MAX - val) / radix)
		{
			wasError = true;
			return 0;
		}

		result = result * radix + val;
	}
	
	if (isNegative) 
	{
		if (-result < INT_MIN)
		{
			wasError = true;
			return 0;
		}
		result = -result;
	}
	else if (result > INT_MAX)
	{
		wasError = true;
		return 0;
	}

	return static_cast<int>(result);
}

std::string IntToString(int n, int radix, bool& wasError)
{
	wasError = false;

	if (radix < 2 || radix > 36)
	{
		wasError = true;
		return "0";
	}

	if (n == 0)
	{
		return "0";
	}

	bool isNegative = false;
	unsigned int num;

	if (n < 0)
	{
		isNegative = true;
		num = static_cast<unsigned int>(-(n + 1)) + 1;
	}
	else
	{
		num = n;
	}

	std::string result = "";
	int digit = 0;

	while (num > 0)
	{
		digit = num % radix;
		result = IntToChar(digit) + result;
		num /= radix;
	}

	if (isNegative)
	{
		result = '-' + result;
	}
	return result;
} 

int main(int argc, char* argv[])
{
	if (argc != 4) 
	{
		std::cout << "Invalid number of arguments\n";
		return 0;
	}
	bool wasError;

	int sourceRadix = StringToInt(argv[1], 10, wasError);
	
	if (wasError || sourceRadix < 2 || sourceRadix > 36)
	{
		std::cout << "Invalid source radix\n";
		return 0;
	}

	int destRadix = StringToInt(argv[2], 10, wasError);
	if (wasError || destRadix < 2 || destRadix > 36)
	{
		std::cout << "Invalid destination radix\n";
		return 0;
	}

	int val = StringToInt(argv[3], sourceRadix, wasError);
	if (wasError)
	{
		std::cout << "Invalid input value or overflow\n";
		return 0;
	}

	std::string result = IntToString(val, destRadix, wasError);
	if (wasError)
	{
		std::cout << "Bad convertation\n";
		return 0;
	}
	std::cout << result << "\n";
}
