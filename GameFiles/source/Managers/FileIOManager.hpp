#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "absolute_path.h"

using ib = std::ios_base;
namespace fs = std::filesystem;

class FileIOManager
{
public:

    FileIOManager(int);

    FileIOManager(const FileIOManager&) = delete;

    FileIOManager(FileIOManager&&) noexcept = delete;

    FileIOManager& operator=(const FileIOManager&) = delete;

    FileIOManager& operator=(FileIOManager&&) noexcept = delete;

    std::vector<std::string> readRecordsFromFile() const;

    void writeRecordsToFile(const std::vector<std::string>&) const;

private:

    int maxRecords_;
    
    std::string file_;
    std::string dir_;
};
