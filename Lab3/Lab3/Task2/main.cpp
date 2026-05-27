#include "Interpreter.h"
#include <iostream>

int main() {
    Interpreter interpreter([](const std::string& msg) {
        std::cout << msg << std::endl;
        });

    interpreter.Run(std::cin);

    return 0;
}