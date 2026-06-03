//Вариант №6 – radix

#include <limits.h>
#include <iostream>
#include <string>

const int MIN_RADIX = 2;
const int MAX_RADIX = 36;
const int DIGIT_ZERO = 0;
const int DIGIT_NINE = 9;
const int LETTER_A = 10;
const int ASCII_ZERO = '0';
const int ASCII_A = 'A';
const int ASCII_NINE = '9';
const int ASCII_Z_UPPER = 'Z';

const char* ERROR_INVALID_ARGS = "Invalid number of arguments\n";
const char* ERROR_INVALID_SOURCE_RADIX = "Invalid source radix\n";
const char* ERROR_INVALID_DEST_RADIX = "Invalid destination radix\n";
const char* ERROR_INVALID_VALUE_OVERFLOW = "Invalid input value or overflow\n";
const char* ERROR_BAD_CONVERSION = "Bad convertation\n";
const char* STRING_ZERO = "0";
const char MINUS_SIGN = '-';
const int EXPECTED_ARGS_COUNT = 4;

int CharToInt(char ch, int radix, bool& wasError)
{
    int val = 0;
    if (ch >= ASCII_ZERO && ch <= ASCII_NINE)
    {
        val = ch - ASCII_ZERO;
        if (val >= radix)
        {
            wasError = true;
            return 0;
        }
    }
    else if (ch >= ASCII_A && ch <= ASCII_Z_UPPER)
    {
        val = ch - ASCII_A + LETTER_A;
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
    char val = ASCII_ZERO;
    if (digit >= DIGIT_ZERO && digit <= DIGIT_NINE)
    {
        val = digit + ASCII_ZERO;
    }
    else
    {
        val = ASCII_A + (digit - LETTER_A);
    }
    return val;
}

int StringToInt(const std::string& str, int radix, bool& wasError)
{
    wasError = false;

    if (radix < MIN_RADIX || radix > MAX_RADIX || str.empty())
    {
        wasError = true;
        return 0;
    }

    bool isNegative = false;
    int startPos = 0;

    if (str[0] == MINUS_SIGN)
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

    if (radix < MIN_RADIX || radix > MAX_RADIX)
    {
        wasError = true;
        return STRING_ZERO;
    }

    if (n == 0)
    {
        return STRING_ZERO;
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
        result = MINUS_SIGN + result;
    }
    return result;
}

int main(int argc, char* argv[])
{
    if (argc != EXPECTED_ARGS_COUNT)
    {
        std::cout << ERROR_INVALID_ARGS;
        return 0;
    }
    bool wasError;

    int sourceRadix = StringToInt(argv[1], 10, wasError);

    if (wasError || sourceRadix < MIN_RADIX || sourceRadix > MAX_RADIX)
    {
        std::cout << ERROR_INVALID_SOURCE_RADIX;
        return 0;
    }

    int destRadix = StringToInt(argv[2], 10, wasError);
    if (wasError || destRadix < MIN_RADIX || destRadix > MAX_RADIX)
    {
        std::cout << ERROR_INVALID_DEST_RADIX;
        return 0;
    }

    int val = StringToInt(argv[3], sourceRadix, wasError);
    if (wasError)
    {
        std::cout << ERROR_INVALID_VALUE_OVERFLOW;
        return 0;
    }

    std::string result = IntToString(val, destRadix, wasError);
    if (wasError)
    {
        std::cout << ERROR_BAD_CONVERSION;
        return 0;
    }
    std::cout << result << "\n";
}