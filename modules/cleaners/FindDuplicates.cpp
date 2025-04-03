#include "FindDuplicates.h"
#include "modules/core/Logger.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace FindDuplicates
{
    std::string hashFile(const std::string &path)
    {
        try
        {
            const uintmax_t maxSize = 3ULL * 1024 * 1024 * 1024; // 3 GB
            if (std::filesystem::file_size(path) > maxSize)
            {
                Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_FILES,
                                       "Datei übersprungen (zu groß für Hash): " + path, __func__, __FILE__, __LINE__);
                return ""; // kein Hash – wird ignoriert
            }

            std::ifstream file(path, std::ios::binary);
            if (!file.is_open())
                return "";

            const size_t bufferSize = 1024 * 1024;
            std::vector<char> buffer(bufferSize);
            std::hash<std::string> hasher;
            size_t combinedHash = 0;

            while (file.read(buffer.data(), bufferSize) || file.gcount())
            {
                std::string chunk(buffer.data(), file.gcount());
                combinedHash ^= hasher(chunk) + 0x9e3779b9 + (combinedHash << 6) + (combinedHash >> 2);
            }

            return std::to_string(combinedHash);
        }
        catch (const std::exception &e)
        {
            Logger::instance().log(LogLevel::LOG_ERROR, LogCategory::LOG_FILES,
                                   "Hashing-Fehler bei Datei: " + path + " - " + e.what(), __func__, __FILE__, __LINE__);
            return "";
        }
    }

    std::vector<DuplicateGroup> findInFolder(const std::string &folderPath)
    {
        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CLEANING, "Starte Duplikat-Suche in: " + folderPath, __func__, __FILE__, __LINE__);

        std::vector<DuplicateGroup> results;
        std::vector<std::string> files;

        for (const auto &entry : fs::recursive_directory_iterator(folderPath))
        {
            if (entry.is_regular_file())
                files.push_back(entry.path().string());
        }

        std::unordered_map<std::string, std::vector<std::string>> hashGroups;

        for (const auto &path : files)
        {
            std::string hash = hashFile(path);
            if (!hash.empty())
                hashGroups[hash].push_back(path);
        }

        for (const auto &[hash, group] : hashGroups)
        {
            if (group.size() > 1)
            {
                results.push_back({group});

                for (size_t i = 0; i < group.size(); ++i)
                {
                    for (size_t j = i + 1; j < group.size(); ++j)
                    {
                        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_FILES,
                                               "Duplikat gefunden: " + group[i] + " <==> " + group[j], __func__, __FILE__, __LINE__);
                    }
                }
            }
        }

        return results;
    }
}
