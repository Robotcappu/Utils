// modules/cleaners/FindDuplicates.cpp
#include "FindDuplicates.h"
#include "modules/core/Logger.h"
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace FindDuplicates
{
    static std::string hashFile(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            return "";

        std::ostringstream oss;
        char buffer[4096];
        while (file.read(buffer, sizeof(buffer)))
        {
            oss.write(buffer, file.gcount());
        }
        oss.write(buffer, file.gcount());

        // primitive Hash-Variante (kann später ersetzt werden)
        std::hash<std::string> hasher;
        return std::to_string(hasher(oss.str()));
    }

    std::vector<DuplicateGroup> findInFolder(const std::string& folderPath)
    {
        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CLEANING,
            "Starte Duplikat-Suche in: " + folderPath, __func__, __FILE__, __LINE__);

        std::unordered_map<uintmax_t, std::vector<std::string>> sizeMap;
        std::vector<DuplicateGroup> results;

        try
        {
            for (auto& entry : fs::recursive_directory_iterator(folderPath))
            {
                if (!entry.is_regular_file()) continue;
                uintmax_t fileSize = entry.file_size();
                sizeMap[fileSize].push_back(entry.path().string());
            }

            for (const auto& [size, files] : sizeMap)
            {
                if (files.size() < 2) continue; // keine Duplikate möglich

                std::unordered_map<std::string, std::string> hashToPath;

                for (const auto& path : files)
                {
                    std::string hash = hashFile(path);
                    if (hash.empty())
                    {
                        Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_FILES,
                            "Konnte Datei nicht hashen: " + path, __func__, __FILE__, __LINE__);
                        continue;
                    }

                    if (hashToPath.find(hash) != hashToPath.end())
                    {
                        std::string other = hashToPath[hash];
                        bool sameName = fs::path(other).filename() == fs::path(path).filename();
                        results.push_back({other, path, sameName});

                        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_FILES,
                            "Duplikat gefunden: " + other + " <==> " + path, __func__, __FILE__, __LINE__);
                    }
                    else
                    {
                        hashToPath[hash] = path;
                    }
                }
            }
        }
        catch (const std::exception& ex)
        {
            Logger::instance().log(LogLevel::LOG_ERROR, LogCategory::LOG_FILES,
                std::string("Fehler beim Durchsuchen: ") + ex.what(), __func__, __FILE__, __LINE__);
        }

        return results;
    }
} // namespace FindDuplicates