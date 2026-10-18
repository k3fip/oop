#pragma once
#include <string>
#include <vector>
#include <memory>
#include "IShape.h"

class FileProcessor {
public:
    static std::vector<std::unique_ptr<IShape>> readShapes(const std::string& filename);
    static void writeResults(const std::string& filename, const std::vector<std::unique_ptr<IShape>>& shapes);
};