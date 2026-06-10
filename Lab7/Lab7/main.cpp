#include <iostream>
#include <string>
#include "Sort2.h"

int main() 
{
    std::string s1, s2;
    std::getline(std::cin, s1);
    std::getline(std::cin, s2);

    const char* p1 = s1.c_str();
    const char* p2 = s2.c_str();

    Sort2(p1, p2);
    std::cout << p1 << "\n" << p2 << std::endl;

    return 0;
}