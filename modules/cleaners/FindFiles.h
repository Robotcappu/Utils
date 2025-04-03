// modules/cleaners/FindFiles.h
#pragma once

#include <string>
#include <vector>

struct LargeFileEntry
{
    std::string path;
    size_t size;
};

namespace FindFiles
{
    // Findet große Dateien in einem Verzeichnis (rekursiv)
    std::vector<LargeFileEntry> findLargeFiles(const std::string& folderPath, size_t minSizeBytes);
} // namespace FindFiles
