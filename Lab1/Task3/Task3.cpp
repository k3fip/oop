//Вариант №2 – invert – 80 баллов

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <cmath>
#include <iomanip>
#include <limits>
#include <cfenv>

const int ORDER = 3;

using Matrix3x3 = std::array<std::array<float, ORDER>, ORDER>;
using Matrix2x2 = std::array<std::array<float, 2>, 2>;

void PrintHelp()
{
	std::cout << "Program for inverting 3x3 matrix\n"
		<< "Usage:\n"
		<< "  program.exe -h        - show help\n"
		<< "  program.exe           - input matrix from console\n"
		<< "  program.exe <file>    - read matrix from file\n";
}

void PrintMatrix(Matrix3x3 matrix)
{
	for (int i = 0; i < ORDER; i++) {
		for (int j = 0; j < ORDER; j++) {
			std::cout << std::fixed << std::setprecision(3) << matrix[i][j];
			if (j < ORDER - 1) {
				std::cout << "	";
			}
		}
		std::cout << "\n";
	}
}

bool CheckOverflow(float value)
{
	int fp_exception = std::fetestexcept(FE_OVERFLOW | FE_UNDERFLOW | FE_INVALID);
	std::feclearexcept(FE_ALL_EXCEPT);

	return (fp_exception & (FE_OVERFLOW | FE_UNDERFLOW | FE_INVALID)) ||
		std::isinf(value) || std::isnan(value);
}

Matrix3x3 GetMatrix(bool& wasError, std::string fileName = "")
{
	wasError = false;
	Matrix3x3 matrix;

	if (fileName != "")
	{
		std::ifstream file;
		file.open(fileName);

		if (!file.is_open())
		{
			wasError = true;
			return { {0} };
		}
		for (int i = 0; i < ORDER; i++) {
			for (int j = 0; j < ORDER; j++)
			{
				file >> matrix[i][j];
				if (file.fail() || CheckOverflow(matrix[i][j]))
				{
					wasError = true;
					return { {0} };
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < ORDER; i++) {
			for (int j = 0; j < ORDER; j++)
			{
				std::cin >> matrix[i][j];
				if (std::cin.fail() || CheckOverflow(matrix[i][j]))
				{
					wasError = true;
					return { {0} };
				}
			}
		}
	}

	return matrix;
}

Matrix2x2 GetCofactor(Matrix3x3 matrix, int row, int column)
{
	Matrix2x2 cofactor = { {{0}} };
	int r = 0, c = 0;

	for (int i = 0; i < ORDER; i++) {
		if (i == row) continue;

		c = 0;
		for (int j = 0; j < ORDER; j++) {
			if (j == column) continue;

			cofactor[r][c] = matrix[i][j];
			c++;
		}
		r++;
	}
	return cofactor;
}

float Determinant2x2(Matrix2x2 matrix, bool& wasError)
{
	std::feclearexcept(FE_ALL_EXCEPT);

	float result = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];

	if (CheckOverflow(result)) {
		wasError = true;
		return 0;
	}

	return result;
}

float Determinant3x3(Matrix3x3 matrix, bool& wasError)
{
	std::feclearexcept(FE_ALL_EXCEPT);

	bool localError = false;

	float term1 = matrix[0][0] * (Determinant2x2(GetCofactor(matrix, 0, 0), localError));
	if (localError) {
		wasError = true;
		return 0;
	}

	float term2 = matrix[0][1] * (Determinant2x2(GetCofactor(matrix, 0, 1), localError));
	if (localError) {
		wasError = true;
		return 0;
	}

	float term3 = matrix[0][2] * (Determinant2x2(GetCofactor(matrix, 0, 2), localError));
	if (localError) {
		wasError = true;
		return 0;
	}

	float result = term1 - term2 + term3;

	if (CheckOverflow(result)) {
		wasError = true;
		return 0;
	}

	return result;
}

Matrix3x3 AlgAdd(Matrix3x3 matrix, bool& wasError)
{
	Matrix3x3 addMatrix;

	for (int i = 0; i < ORDER; i++)
	{
		for (int j = 0; j < ORDER; j++)
		{
			std::feclearexcept(FE_ALL_EXCEPT);

			Matrix2x2 cofactor = GetCofactor(matrix, i, j);
			bool localError = false;
			float det = Determinant2x2(cofactor, localError);

			if (localError) {
				wasError = true;
				return { {0} };
			}

			float sign = static_cast<float>(std::pow(-1, i + j));
			float result = sign * det;

			if (CheckOverflow(result)) {
				wasError = true;
				return { {0} };
			}

			addMatrix[j][i] = result;
		}
	}

	return addMatrix;
}

Matrix3x3 GetInvertedMatrix(Matrix3x3 matrix, bool& wasError)
{
	wasError = false;

	Matrix3x3 inverted;
	Matrix3x3 algAdd = AlgAdd(matrix, wasError);

	if (wasError) {
		return { {0} };
	}

	float det = Determinant3x3(matrix, wasError);
	if (wasError || det == 0)
	{
		wasError = true;
		return { {0} };
	}

	std::feclearexcept(FE_ALL_EXCEPT);

	float coef = 1.0f / det;

	if (CheckOverflow(coef)) {
		wasError = true;
		return { {0} };
	}

	for (int i = 0; i < ORDER; i++)
	{
		for (int j = 0; j < ORDER; j++)
		{
			std::feclearexcept(FE_ALL_EXCEPT);

			float value = coef * algAdd[i][j];

			if (CheckOverflow(value)) {
				wasError = true;
				return { {0} };
			}

			if (std::abs(value) < std::numeric_limits<float>::min()) {
				inverted[i][j] = 0;
			}
			else {
				inverted[i][j] = value;
			}
		}
	}

	return inverted;
}

int main(int argc, char* argv[])
{
	bool wasError;
	Matrix3x3 matrix;

	std::feclearexcept(FE_ALL_EXCEPT);

	if (argc > 2)
	{
		std::cout << "Incorrect input\n";
		return 0;
	}

	if (argc == 2 && strcmp(argv[1], "-h") == 0)
	{
		PrintHelp();
		return 0;
	}

	matrix = argc == 2 ? GetMatrix(wasError, argv[1]) : GetMatrix(wasError);
	if (wasError)
	{
		std::cout << "Incorrect matrix format or overflow\n";
		return 0;
	}

	Matrix3x3 inverted = GetInvertedMatrix(matrix, wasError);
	if (wasError)
	{
		std::cout << "Non-invertible or overflow\n";
		return 0;
	}

	PrintMatrix(inverted);
	return 0;
}