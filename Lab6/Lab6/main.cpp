#include <iostream>
#include <string>
#include <vector>
#include "SortStrings3.h"

int main() {
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(std::cin, line)) {
        lines.push_back(line);
    }

    for (size_t i = 0; i + 2 < lines.size(); i += 3) {
        std::string s1 = lines[i];
        std::string s2 = lines[i + 1];
        std::string s3 = lines[i + 2];

        SortStrings3(s1, s2, s3);

        std::cout << s1 << '\n'
            << s2 << '\n'
            << s3 << '\n';
    }

    return 0;
}