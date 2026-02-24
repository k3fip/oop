//Вариант №2 – compare – 40 баллов 
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid number of arguments\n";
		return 0;
	}

	std::ifstream file1;
	std::ifstream file2;

	file1.open(argv[1]);
	file2.open(argv[2]);

	bool equal = true;
	if (!(file1.is_open() && file2.is_open()))
	{
		std::cout << "Incorrect input\n";
		return 0;
	}

	std::string str1;
	std::string str2;

	std::getline(file1, str1);
	std::getline(file2, str2);

	int strCounter = 1;

	while (equal)
	{
		if (file1.eof() && file2.eof())
		{
			break;
		}
		if ((str1 != str2) || ((!file1.eof() && file2.eof()) || (file1.eof() && !file2.eof())))
		{
			equal = false;
			break;
		}
		else
		{
			strCounter += 1;
			std::getline(file1, str1);
			std::getline(file2, str2);
		}

	}

	if (equal)
	{
		std::cout << "Files are equal\n";
	}
	else
	{
		std::cout << "Files are different. Line number is " << strCounter << "\n";
	}

	return 0;
}

