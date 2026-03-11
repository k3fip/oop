#include <iostream>
#include "Dictionary.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <dictionary_file_name>\n";
        return 1;
    }

    std::string dictionaryFile = argv[1];
    ProcessDictionary(dictionaryFile);

    return 0;
}
