// modules/cleaners/FindDuplicates.h
#pragma once

#include <string>
#include <vector>

struct DuplicateGroup
{
    std::string path1;
    std::string path2;
    bool sameName;
};

namespace FindDuplicates
{
    // Scannt einen Ordner rekursiv und liefert doppelte Dateien zurück
    std::vector<DuplicateGroup> findInFolder(const std::string& folderPath);
} // namespace FindDuplicates
