// FindDuplicates.h
#pragma once

#include <string>
#include <vector>

struct DuplicateGroup
{
    std::vector<std::string> paths;
};

namespace FindDuplicates
{
    // Hash-Berechnung des Dateiinhalts
    std::string hashFile(const std::string& path);

    // Findet Duplikate anhand des Dateiinhalts
    std::vector<DuplicateGroup> findInFolder(const std::string& folderPath);
} // namespace FindDuplicates
