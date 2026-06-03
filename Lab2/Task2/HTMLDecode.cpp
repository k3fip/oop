//Вариант 5 – HTML Decode

#include "HTMLDecode.h"
#include <iostream>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, char> HTML_ENTITIES = {
    {"&quot;", '"'},
    {"&apos;", '\''},
    {"&lt;", '<'},
    {"&gt;", '>'},
    {"&amp;", '&'}
};

std::string GetString(std::istream& input)
{
    std::string val;
    std::string str;
    while (std::getline(input, val))
    {
        str += val;
    }
    return str;
}

std::string HTMLDecode(const std::string& str)
{
    std::string result;
    result.reserve(str.length());

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '&')
        {
            size_t semicolonPos = str.find(';', i);
            if (semicolonPos != std::string::npos)
            {
                std::string entity = str.substr(i, semicolonPos - i + 1);
                auto it = HTML_ENTITIES.find(entity);
                if (it != HTML_ENTITIES.end())
                {
                    result += it->second;
                    i = semicolonPos;
                    continue;
                }
            }
        }
        result += str[i];
    }

    return result;
}