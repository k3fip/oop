#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using Dictionary = std::map<std::string, std::vector<std::string>>;

const std::string EXIT_COMMAND = "...";

void PrintRules()
{
    std::cout << "Enter a word to get its translation or add it to the dictionary,\n"
        << "to stop working with the dictionary enter three dots (...).\n";
}

std::string Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

Dictionary FetchDictionary(const std::string& filename)
{
    Dictionary dict;

    std::ifstream dictFile(filename);

    if (!dictFile.is_open())
    {
        return dict;
    }

    std::string line;
    while (std::getline(dictFile, line))
    {
        line = Trim(line);
        if (line.empty())
            continue;

        size_t delimiterPos = line.find(':');
        if (delimiterPos != std::string::npos)
        {
            std::string key = Trim(line.substr(0, delimiterPos));
            std::string valuesStr = Trim(line.substr(delimiterPos + 1));

            std::vector<std::string> values;
            size_t start = 0;
            size_t end = valuesStr.find(',');

            while (end != std::string::npos)
            {
                std::string value = Trim(valuesStr.substr(start, end - start));
                if (!value.empty())
                    values.push_back(value);
                start = end + 1;
                end = valuesStr.find(',', start);
            }

            std::string lastValue = Trim(valuesStr.substr(start));
            if (!lastValue.empty())
                values.push_back(lastValue);

            if (!key.empty() && !values.empty())
                dict[key] = values;
        }
    }
    dictFile.close();
    return dict;
}

bool SaveDictionary(const std::string& filename, const Dictionary& dict)
{
    std::ofstream dictFile(filename);
    if (!dictFile.is_open())
        return false;

    for (auto it = dict.begin(); it != dict.end(); ++it)
    {
        dictFile << it->first << ":";
        const std::vector<std::string>& translations = it->second;
        for (size_t i = 0; i < translations.size(); ++i)
        {
            if (i > 0)
                dictFile << ",";
            dictFile << " " << translations[i];
        }
        dictFile << "\n";
    }
    return true;
}

void PrintTranslation(const std::string& word, const std::vector<std::string>& translations)
{
    std::cout << word << " -> ";
    for (size_t i = 0; i < translations.size(); ++i)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << translations[i];
    }
    std::cout << "\n";
}

void ProcessDictionary(const std::string& filename)
{
    Dictionary dict = FetchDictionary(filename);
    bool dictionaryChanged = false;

    PrintRules();

    std::string input;

    while (std::getline(std::cin, input))
    {
        input = Trim(input);

        if (input == EXIT_COMMAND)
        {
            break;
        }

        if (input.empty())
        {
            continue;
        }

        Dictionary::iterator it = dict.find(input);
        if (it != dict.end())
        {
            PrintTranslation(input, it->second);
        }
        else
        {
            std::cout << "Unknown word \"" << input << "\". Enter translation or empty line to skip.\n";

            std::string translation;
            std::getline(std::cin, translation);
            translation = Trim(translation);

            if (!translation.empty())
            {
                dict[input] = std::vector<std::string>{ translation };
                dictionaryChanged = true;
                std::cout << "Word \"" << input << "\" saved in dictionary as \"" << translation << "\".\n";
            }
            else
            {
                std::cout << "Word \"" << input << "\" ignored.\n";
            }
        }
    }

    if (dictionaryChanged)
    {
        std::cout << "Dictionary was modified. Enter Y or y to save before exit.\n";
        std::string answer;
        std::getline(std::cin, answer);
        answer = Trim(answer);

        if (answer == "Y" || answer == "y")
        {
            if (SaveDictionary(filename, dict))
            {
                std::cout << "Changes saved. Goodbye.\n";
            }
            else
            {
                std::cout << "Error saving dictionary. Goodbye.\n";
            }
        }
        else
        {
            std::cout << "Changes not saved. Goodbye.\n";
        }
    }
    else
    {
        std::cout << "Goodbye.\n";
    }
}
