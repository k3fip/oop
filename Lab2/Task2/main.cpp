#include "HTMLDecode.h"
#include <iostream>

int main()
{
	std::string str = GetString(std::cin);
	
	std::string decoded = HTMLDecode(str);

	std::cout << decoded << "\n";
}