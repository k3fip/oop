#pragma once
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>

using Dictionary = std::map<std::string, std::vector<std::string>>;

Dictionary FetchDictionary(const std::string& filename);
void PrintRules();
std::string Trim(const std::string& str);
bool SaveDictionary(const std::string& filename, const Dictionary& dict);
void PrintTranslation(const std::string& word, const std::vector<std::string>& translations);
void ProcessDictionary(const std::string& filename);
