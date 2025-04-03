// modules/cleaners/FindFiles.cpp
#include "FindFiles.h"
#include "modules/core/Logger.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace FindFiles
{
    std::vector<LargeFileEntry> findLargeFiles(const std::string& folderPath, size_t minSizeBytes)
    {
        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CLEANING,
            "Suche nach großen Dateien in: " + folderPath, __func__, __FILE__, __LINE__);

        std::vector<LargeFileEntry> result;

        try
        {
            for (const auto& entry : fs::recursive_directory_iterator(folderPath))
            {
                if (!entry.is_regular_file()) continue;

                uintmax_t fileSize = entry.file_size();
                if (fileSize >= minSizeBytes)
                {
                    result.push_back({ entry.path().string(), fileSize });

                    Logger::instance().log(LogLevel::LOG_DEBUG, LogCategory::LOG_FILES,
                        "Große Datei gefunden: " + entry.path().string() + " (" + std::to_string(fileSize) + " Bytes)",
                        __func__, __FILE__, __LINE__);
                }
            }
        }
        catch (const std::exception& ex)
        {
            Logger::instance().log(LogLevel::LOG_ERROR, LogCategory::LOG_FILES,
                std::string("Fehler beim Durchsuchen: ") + ex.what(), __func__, __FILE__, __LINE__);
        }

        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CLEANING,
            "Anzahl großer Dateien: " + std::to_string(result.size()), __func__, __FILE__, __LINE__);

        return result;
    }
} // namespace FindFiles
